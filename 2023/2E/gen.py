print('float lut[15][10]{')
for i in range(15):
    print('{', end='')
    print(','.join(f'{j}e-{i+1}' for j in range(10)), end='}')
    print(',')
print('};')
