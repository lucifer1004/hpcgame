#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run(const char *command) {
  FILE *fp = popen(command, "r");
  char path[PATH_MAX];
  while (fgets(path, PATH_MAX, fp) != NULL)
    printf("%s", path);
  int status = pclose(fp);
  if (status != 0) {
    printf("Error: %d\n", status);
    exit(status);
  }
}

int main() {
  run("module load pyenv");

  FILE *py;
  if ((py = fopen("./out.py", "w"))) {
    fprintf(py, "%s\n", R"V0G0N(
import numpy as np
import taichi as ti
import struct

ti.init(arch=ti.gpu, default_fp=ti.f32)

with open("conf.data", mode="rb") as bfile:
    contents = bfile.read()
    ray_density = struct.unpack("<i", contents[:4])[0]
    sensor_density = struct.unpack("<i", contents[4:8])[0]
    pane_count = struct.unpack("<i", contents[8:12])[0]
    pane_pos = np.zeros(pane_count)
    ptr = 12
    for i in range(pane_count):
        pane_pos[i] = struct.unpack("<f", contents[ptr:ptr+4])[0]
        ptr += 4
    lens_index = np.zeros(pane_count + 1, dtype=np.int32)
    for i in range(pane_count + 1):
        lens_index[i] = struct.unpack("<i", contents[ptr:ptr+4])[0]
        ptr += 4
    lens = np.zeros(4 * lens_index[-1])
    for i in range(0, 4 * lens_index[-1], 4):
        lens[i] = struct.unpack("<f", contents[ptr:ptr+4])[0]
        lens[i + 1] = struct.unpack("<f", contents[ptr+4:ptr+8])[0]
        lens[i + 2] = struct.unpack("<f", contents[ptr+8:ptr+12])[0]
        lens[i + 3] = struct.unpack("<f", contents[ptr+12:ptr+16])[0]
        ptr += 16

sensor_data = ti.field(dtype=ti.i32, shape=(sensor_density, sensor_density))
lens_data = ti.field(dtype=ti.f32, shape=len(lens))
lens_data.from_numpy(lens)
pos = ti.field(dtype=ti.f32, shape=(pane_count))
pos.from_numpy(pane_pos)
index = ti.field(dtype=ti.i32, shape=(pane_count + 1))
index.from_numpy(lens_index)


@ti.kernel
def causcal():
    rs = 1.0 / ray_density

    ti.loop_config(block_dim=128)
    ti.block_local(lens_data)
    ti.block_local(pos)
    ti.block_local(index)
    for i, j in ti.ndrange(ray_density, ray_density):
        y = (i + 0.5) * rs
        x = (j + 0.5) * rs
        z = 0.0
        kx = 0.0
        ky = 0.0
        invkz = 1.0
        broken = False

        for k in range(pane_count):
            x += (pos[k] - z) * kx * invkz
            y += (pos[k] - z) * ky * invkz

            if 0.0 < x < 1.0 and 0.0 < y < 1.0:
                z = pos[k]
                gx = 0.0
                gy = 0.0
                s = index[k]
                e = index[k + 1]
                for l in range(s, e):
                    rx = x - lens_data[4 * l]
                    ry = y - lens_data[4 * l + 1]
                    rr = rx * rx + ry * ry
                    invr02 = lens_data[4 * l + 2] ** 2
                    if rr < invr02 * 0.99999:
                        invr02 = 1.0 / invr02
                        rr *= invr02
                        rr = 4.0 / (1.0 - rr)
                        rr = -0.5 * ti.exp(4.0 - rr) * rr * \
                            rr * invr02 * lens_data[4 * l + 3]
                        gx += rx * rr
                        gy += ry * rr

                kx += gx
                ky += gy
                kp = kx * kx + ky * ky
                if kp < 1.0:
                    invkz = 1.0 / ti.sqrt(1.0 - kp)
                else:
                    broken = True
                    break
            else:
                broken = True
                break

        if not broken:
            x += (1.0 - z) * kx * invkz
            y += (1.0 - z) * ky * invkz

            if 0.0 < x < 1.0 and 0.0 < y < 1.0:
                sensor_data[int(y * sensor_density),
                            int(x * sensor_density)] += 1


causcal()
out = sensor_data.to_numpy()

with open("out.data", mode="wb") as bfile:
    for i in range(sensor_density):
        for j in range(sensor_density):
            bfile.write(struct.pack("<i", out[i, j]))
)V0G0N");
    fclose(py);
  }

  run("/data/software/tools/anaconda/3/envs/pyenv/bin/python3 ./out.py");
  return 0;
}