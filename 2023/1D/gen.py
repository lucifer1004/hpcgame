import numpy as np

z = 10 * np.ones(200, dtype=np.int32)
z[0] = 8
z[1] = 198
z.tofile("input.bin")
