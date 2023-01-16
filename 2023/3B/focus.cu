#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_DIM 512
#define BLOCK_DIM 512

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

__global__ void causcal_kernel(int *cudata, int *lensindex, float *panepos,
                               float *lensdata, int raydensity,
                               int sensordensity, int panecount) {
  float rs = 1.0f / raydensity;
  float y, x, z, kx, ky, invkz;

  for (int i = blockIdx.x; i < raydensity; i += GRID_DIM) {
    for (int j = threadIdx.x; j < raydensity; j += BLOCK_DIM) {
      y = rs * (0.5f + i);
      x = rs * (0.5f + j);
      z = 0.0f;
      kx = 0.0f;
      ky = 0.0f;
      invkz = 1.0f;
      bool broken = false;

      for (int k = 0; k < panecount; k++) {
        x += (panepos[k] - z) * kx * invkz;
        y += (panepos[k] - z) * ky * invkz;
        if (x > 0.0f && x < 1.0f && y > 0.0f && y < 1.0f) {
          z = panepos[k];

          float gx = 0.0f, gy = 0.0f;
          int is = lensindex[k];
          int ie = lensindex[k + 1];
          for (int l = 4 * is; l < 4 * ie; l += 4) {
            float rx = x - lensdata[l];
            float ry = y - lensdata[l + 1];
            float r = rx * rx + ry * ry;
            float invr02 = lensdata[l + 2] * lensdata[l + 2];
            if (r < invr02 * 0.99999f) {
              invr02 = 1.0f / invr02;
              r = r * invr02;
              r = 4.0f / (1.0f - r);
              r = -0.5f * exp(4.0f - r) * r * r * invr02 * lensdata[l + 3];
              gx += r * rx;
              gy += r * ry;
            }
          }

          kx += gx;
          ky += gy;
          float kp = kx * kx + ky * ky;
          if (kp < 1.0f) {
            invkz = 1.0f / sqrt(1.0f - kp);
          } else {
            broken = true;
            break;
          }
        } else {
          broken = true;
          break;
        }
      }

      if (!broken) {
        x += (1.0f - z) * kx * invkz;
        y += (1.0f - z) * ky * invkz;

        if (x > 0.0f && x < 1.0f && y > 0.0f && y < 1.0f) {
          int pixelindex =
              int(x * sensordensity) + sensordensity * int(y * sensordensity);
          atomicAdd(&cudata[pixelindex], 1);
        }
      }
    }
  }
}

void causcal_cuda(pane_t pane, result_t result) {
  cudaMalloc((void **)&cudata,
             sizeof(int) * result.sensordensity * result.sensordensity);

  cudaMalloc((void **)&panepos, sizeof(float) * pane.panecount);
  cudaMemcpy(panepos, pane.panepos, sizeof(float) * pane.panecount,
             cudaMemcpyHostToDevice);

  cudaMalloc((void **)&lensindex, sizeof(int) * (pane.panecount + 1));
  cudaMemcpy(lensindex, pane.lensindex, sizeof(int) * (pane.panecount + 1),
             cudaMemcpyHostToDevice);

  cudaMalloc((void **)&lensdata,
             sizeof(float) * pane.lensindex[pane.panecount] * 4);
  cudaMemcpy(lensdata, pane.lensdata,
             sizeof(float) * pane.lensindex[pane.panecount] * 4,
             cudaMemcpyHostToDevice);

  causcal_kernel<<<GRID_DIM, BLOCK_DIM>>>(cudata, lensindex, panepos, lensdata,
                                          result.raydensity,
                                          result.sensordensity, pane.panecount);

  memset(result.sensordata, 0,
         result.sensordensity * result.sensordensity * sizeof(int));

  cudaMemcpy(result.sensordata, cudata,
             sizeof(int) * result.sensordensity * result.sensordensity,
             cudaMemcpyDeviceToHost);
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
  result_t result;
  pane_t pane;
  loadconf("./conf.data", pane, result);

  causcal_cuda(pane, result);

  FILE *fi;
  if ((fi = fopen("./out.data", "wb"))) {
    fwrite(result.sensordata, 1,
           result.sensordensity * result.sensordensity * sizeof(int), fi);
    fclose(fi);
  }

  return 0;
}
