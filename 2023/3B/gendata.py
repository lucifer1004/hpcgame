import struct
from random import random

with open("conf.data", mode="wb") as bfile:
    ray_density = 16384
    sensor_density = 1024
    
    pane_count = 32
    pane_pos = [random() for _ in range(pane_count)]
    pane_pos.sort()
    
    lens_index = [i * 256 for i in range(pane_count + 1)]
    
    bfile.write(struct.pack("<i", ray_density))
    bfile.write(struct.pack("<i", sensor_density))
    bfile.write(struct.pack("<i", pane_count))
    for pos in pane_pos:
        bfile.write(struct.pack("<f", pos))
    for index in lens_index:
        bfile.write(struct.pack("<i", index))
        
    for i in range(lens_index[-1]):
        x = random()
        y = random()
        r = random() * 0.05 + 0.05
        d = random() * 0.0001
        bfile.write(struct.pack("<ffff", x, y, r, d))
    