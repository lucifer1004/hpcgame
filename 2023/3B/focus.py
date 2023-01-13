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
    lens_x = np.zeros(lens_index[-1])
    lens_y = np.zeros(lens_index[-1])
    lens_r = np.zeros(lens_index[-1])
    lens_d = np.zeros(lens_index[-1])
    for i in range(lens_index[-1]):
        lens_x[i] = struct.unpack("<f", contents[ptr:ptr+4])[0]
        lens_y[i] = struct.unpack("<f", contents[ptr+4:ptr+8])[0]
        lens_r[i] = struct.unpack("<f", contents[ptr+8:ptr+12])[0]
        lens_d[i] = struct.unpack("<f", contents[ptr+12:ptr+16])[0]
        ptr += 16

sensor_data = ti.field(dtype=ti.i32, shape=(sensor_density, sensor_density))
lx = ti.field(dtype=ti.f32, shape=(len(lens_x)))
lx.from_numpy(lens_x)
ly = ti.field(dtype=ti.f32, shape=(len(lens_x)))
ly.from_numpy(lens_y)
lr = ti.field(dtype=ti.f32, shape=(len(lens_x)))
lr.from_numpy(lens_r)
ld = ti.field(dtype=ti.f32, shape=(len(lens_x)))
ld.from_numpy(lens_d)
pos = ti.field(dtype=ti.f32, shape=(pane_count))
pos.from_numpy(pane_pos)
index = ti.field(dtype=ti.i32, shape=(pane_count + 1))
index.from_numpy(lens_index)


@ti.kernel
def causcal():
    rs = 1.0 / ray_density

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
                    rx = x - lx[l]
                    ry = y - ly[l]
                    rr = rx * rx + ry * ry
                    invr02 = lr[l] * lr[l]
                    if rr < invr02 * 0.99999:
                        invr02 = 1.0 / invr02
                        rr *= invr02
                        rr = 4.0 / (1.0 - rr)
                        rr = -0.5 * ti.exp(4.0 - rr) * rr * rr * invr02 * ld[l]
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