import numpy as np
import torch.nn.functional as F
import torch

def conv2d(input, kernel):
    # should be [1, 1, N, M]
    input = torch.from_numpy(input.reshape(1, 1, input.shape[0], input.shape[1]))
    # should be [1, 1, C, D]
    kernel = torch.from_numpy(kernel.reshape(1, 1, kernel.shape[0], kernel.shape[1]))
    output = F.conv2d(input, kernel, padding='valid')
    return output[0][0]



def main():
    # read the input and kernel from the files
    # input from input.txt
    # kernel from kernel.txt

    # input: first two numbers are the dimensions of the input
    # the rest of the numbers are the input values
    with open('input.txt', 'r') as f:
        line = f.readline().split(" ")
        N = int(line[0])
        M = int(line[1])
        input = np.loadtxt(f)
    input = input.reshape(N, M)

    # kernel: first two numbers are the dimensions of the kernel
    # the rest of the numbers are the kernel values
    # read and convert to torch tensor
    with open('weight.txt', 'r') as f:
        line = f.readline().split(" ")
        C = int(line[0])
        D = int(line[1])
        kernel = np.loadtxt(f)
    kernel = kernel.reshape(C, D)


    output = conv2d(input, kernel)

    out_n = N - C + 1
    out_m = M - D + 1
    output = output.reshape(out_n, out_m)

    # write to output.txt, with out_n,
    # out_m as the first two numbers
    with open('output.txt', 'w') as f:
        f.write(str(out_n) + " " + str(out_m) + "\n")
        np.savetxt(f, output)

if __name__ == '__main__':
    main()