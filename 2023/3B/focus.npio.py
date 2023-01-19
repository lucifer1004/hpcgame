import numpy as np
import taichi as ti
import struct

ti.init(arch=ti.gpu, default_fp=ti.f32)

data = np.fromfile("conf.data", dtype=np.float32)
ray_density = struct.unpack("i", data[0])[0]
sensor_density = struct.unpack("i", data[1])[0]
pane_count = struct.unpack("i", data[2])[0]
pane_pos = data[3:3 + pane_count]
lens_index = np.frombuffer(data[3 + pane_count:3 +
                                pane_count + pane_count + 1].tobytes(), dtype=np.int32)
lens_data = data[3 + pane_count + pane_count + 1:]

sensor_data = ti.field(dtype=ti.i32, shape=(sensor_density, sensor_density))
l_data = ti.field(dtype=ti.f32, shape=(len(lens_data)))
l_data.from_numpy(lens_data)
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
                    rx = x - l_data[l * 4]
                    ry = y - l_data[l * 4 + 1]
                    rr = rx * rx + ry * ry
                    invr02 = l_data[l * 4 + 2] ** 2
                    if rr < invr02 * 0.99999:
                        invr02 = 1.0 / invr02
                        rr *= invr02
                        rr = 4.0 / (1.0 - rr)
                        rr = -0.5 * ti.exp(4.0 - rr) * rr * \
                            rr * invr02 * l_data[l * 4 + 3]
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
out.tofile("out.data")
