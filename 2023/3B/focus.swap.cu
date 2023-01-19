#include <ctime>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXIMUM_PANES 32
#define TEMPLATE_PER_PANE 256
#define GRID_DIM 256
#define BLOCK_DIM 256

struct pane_t {
  int panecount;
  float *panepos;
  int *lensindex;
  float *lensdata;
};

struct result_t {
  int raydensity;
  int sensordensity;
  int *sensordata;
};

int *cudata, *lensindex;
float *panepos, *lensdata;

#define MAKE_CAUSCAL_KERNEL(N)                                                 \
  __global__ void causcal_kernel_##N(int *cudata, float *panepos,              \
                                     float *lensdata, int raydensity,          \
                                     int sensordensity, int panecount) {       \
    float rs = 1.0f / raydensity;                                              \
    float y, x, z, kx, ky, invkz;                                              \
    __shared__ float panepos_[MAXIMUM_PANES];                                  \
                                                                               \
    if (threadIdx.x < panecount) {                                             \
      panepos_[threadIdx.x] = panepos[threadIdx.x];                            \
    }                                                                          \
    __syncthreads();                                                           \
                                                                               \
    __shared__ float lensdata_[TEMPLATE_PER_PANE * 4];                         \
                                                                               \
    float xx[N * N], yy[N * N], kxx[N * N], kyy[N * N];                        \
    bool flag[N * N];                                                          \
    for (int i = 0; i < N; i++) {                                              \
      for (int j = 0; j < N; j++) {                                            \
        xx[i * N + j] = rs * (0.5f + blockIdx.x + i * GRID_DIM);               \
        yy[i * N + j] = rs * (0.5f + threadIdx.x + j * BLOCK_DIM);             \
        kxx[i * N + j] = 0.0f;                                                 \
        kyy[i * N + j] = 0.0f;                                                 \
        flag[i * N + j] = false;                                               \
      }                                                                        \
    }                                                                          \
                                                                               \
    for (int k = 0; k < panecount; k++) {                                      \
      for (int j = threadIdx.x; j < TEMPLATE_PER_PANE; j += BLOCK_DIM) {       \
        lensdata_[j * 4] = lensdata[k * TEMPLATE_PER_PANE * 4 + j * 4];        \
        lensdata_[j * 4 + 1] =                                                 \
            lensdata[k * TEMPLATE_PER_PANE * 4 + j * 4 + 1];                   \
        lensdata_[j * 4 + 2] =                                                 \
            lensdata[k * TEMPLATE_PER_PANE * 4 + j * 4 + 2] *                  \
            lensdata[k * TEMPLATE_PER_PANE * 4 + j * 4 + 2];                   \
        lensdata_[j * 4 + 3] =                                                 \
            lensdata[k * TEMPLATE_PER_PANE * 4 + j * 4 + 3];                   \
      }                                                                        \
      __syncthreads();                                                         \
                                                                               \
      for (int i = 0; i < N; i++) {                                            \
        for (int j = 0; j < N; j++) {                                          \
          if (!flag[i * N + j]) {                                              \
            kx = kxx[i * N + j];                                               \
            ky = kyy[i * N + j];                                               \
            invkz = 1.0f / sqrt(1.0f - kx * kx - ky * ky);                     \
            x = xx[i * N + j] + (panepos_[k] - z) * kx * invkz;                \
            y = yy[i * N + j] + (panepos_[k] - z) * ky * invkz;                \
            if (x <= 0.0f || x >= 1.0f || y <= 0.0f || y >= 1.0f) {            \
              flag[i * N + j] = true;                                          \
              break;                                                           \
            }                                                                  \
                                                                               \
            if (!flag[i * N + j]) {                                            \
              float gx = 0.0f, gy = 0.0f;                                      \
              for (int l = 0; l < TEMPLATE_PER_PANE * 4; l += 4) {             \
                float rx = x - lensdata_[l];                                   \
                float ry = y - lensdata_[l + 1];                               \
                float r = rx * rx + ry * ry;                                   \
                float invr02 = lensdata_[l + 2];                               \
                if (r < invr02 * 0.99999f) {                                   \
                  invr02 = 1.0f / invr02;                                      \
                  r = r * invr02;                                              \
                  r = 4.0f / (1.0f - r);                                       \
                  r = -0.5f * exp(4.0f - r) * r * r * invr02 *                 \
                      lensdata_[l + 3];                                        \
                  gx += r * rx;                                                \
                  gy += r * ry;                                                \
                }                                                              \
              }                                                                \
                                                                               \
              kx += gx;                                                        \
              ky += gy;                                                        \
              float kp = kx * kx + ky * ky;                                    \
              if (kp >= 1.0f) {                                                \
                flag[i * N + j] = true;                                        \
                break;                                                         \
              }                                                                \
            }                                                                  \
          }                                                                    \
                                                                               \
          xx[i * N + j] = x;                                                   \
          yy[i * N + j] = y;                                                   \
          kxx[i * N + j] = kx;                                                 \
          kyy[i * N + j] = ky;                                                 \
        }                                                                      \
      }                                                                        \
                                                                               \
      z = panepos_[k];                                                         \
    }                                                                          \
                                                                               \
    for (int i = 0; i < N; i++) {                                              \
      for (int j = 0; j < N; j++) {                                            \
        if (!flag[i * N + j]) {                                                \
          kx = kxx[i * N + j];                                                 \
          ky = kyy[i * N + j];                                                 \
          invkz = 1.0f / sqrt(1.0f - kx * kx - ky * ky);                       \
          x = xx[i * N + j] + (1.0f - z) * kx * invkz;                         \
          y = yy[i * N + j] + (1.0f - z) * ky * invkz;                         \
          if (x > 0.0f && x < 1.0f && y > 0.0f && y < 1.0f) {                  \
            int pixelindex = int(x * sensordensity) +                          \
                             sensordensity * int(y * sensordensity);           \
            atomicAdd(&cudata[pixelindex], 1);                                 \
          }                                                                    \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

MAKE_CAUSCAL_KERNEL(16)
MAKE_CAUSCAL_KERNEL(64)

void causcal_cuda(pane_t pane, result_t result) {
  cudaMalloc((void **)&cudata,
             sizeof(int) * result.sensordensity * result.sensordensity);

  cudaMalloc((void **)&panepos, sizeof(float) * pane.panecount);
  cudaMemcpy(panepos, pane.panepos, sizeof(float) * pane.panecount,
             cudaMemcpyHostToDevice);

  cudaMalloc((void **)&lensdata,
             sizeof(float) * pane.lensindex[pane.panecount] * 4);
  cudaMemcpy(lensdata, pane.lensdata,
             sizeof(float) * pane.lensindex[pane.panecount] * 4,
             cudaMemcpyHostToDevice);

  if (result.raydensity == 4096) {
    causcal_kernel_16<<<GRID_DIM, BLOCK_DIM,
                        sizeof(float) *
                            (MAXIMUM_PANES + TEMPLATE_PER_PANE * 4)>>>(
        cudata, panepos, lensdata, result.raydensity, result.sensordensity,
        pane.panecount);
  } else {
    causcal_kernel_64<<<GRID_DIM, BLOCK_DIM,
                        sizeof(float) *
                            (MAXIMUM_PANES + TEMPLATE_PER_PANE * 4)>>>(
        cudata, panepos, lensdata, result.raydensity, result.sensordensity,
        pane.panecount);
  }

  memset(result.sensordata, 0,
         result.sensordensity * result.sensordensity * sizeof(int));

  cudaMemcpy(result.sensordata, cudata,
             sizeof(int) * result.sensordensity * result.sensordensity,
             cudaMemcpyDeviceToHost);

  cudaFree(cudata);
  cudaFree(panepos);
  cudaFree(lensdata);
}

int loadconf(const char *fn, pane_t &pane, result_t &result) {
  size_t n;
  FILE *fi;
  if ((fi = fopen(fn, "rb"))) {
    n = fread(&result.raydensity, 4, 1, fi);
    n = fread(&result.sensordensity, 4, 1, fi);
    result.sensordata = (int *)malloc(result.sensordensity *
                                      result.sensordensity * sizeof(int));

    n = fread(&pane.panecount, 4, 1, fi);
    pane.panepos = (float *)malloc(pane.panecount * sizeof(float));
    n = fread(pane.panepos, 4, pane.panecount, fi);
    pane.lensindex = (int *)malloc((pane.panecount + 1) * sizeof(int));
    n = fread(pane.lensindex, 4, pane.panecount + 1, fi);
    pane.lensdata =
        (float *)malloc(pane.lensindex[pane.panecount] * 4 * sizeof(float));
    n = fread(pane.lensdata, 4, pane.lensindex[pane.panecount] * 4, fi);

    fclose(fi);
  }
  printf("%lu\n", n);
  return 0;
}

int main() {
  clock_t before = clock();
  result_t result;
  pane_t pane;
  loadconf("./conf.data", pane, result);
  printf("Loading configuration takes %ld ms.\n",
         (clock() - before) * 1000 / CLOCKS_PER_SEC);
  before = clock();

  causcal_cuda(pane, result);
  cudaDeviceSynchronize();
  printf("Processing data takes %ld ms.\n",
         (clock() - before) * 1000 / CLOCKS_PER_SEC);
  before = clock();

  FILE *fi;
  if ((fi = fopen("./out.data", "wb"))) {
    fwrite(result.sensordata, 1,
           result.sensordensity * result.sensordensity * sizeof(int), fi);
    fclose(fi);
  }
  printf("Writing output takes %ld ms.\n",
         (clock() - before) * 1000 / CLOCKS_PER_SEC);
  before = clock();

  return 0;
}
