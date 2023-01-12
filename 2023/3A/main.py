from cgsolver import CGPoissonSolver
import taichi as ti
import numpy as np

ti.init(arch=ti.gpu, default_fp=ti.f64, kernel_profiler=True)  # Use device_memory_GB=X to allocate more memory if necessary

eps = 1e-8
problem_size = [256, 512, 1024, 2048]
repeats = 5

for psize in problem_size:
    print(f'\n>>> Current problem size: {psize} x {psize}')
    n_iter = 0
    kernel_time = 0.0
    cgsolver = CGPoissonSolver(psize, eps, quiet=True)    
    while n_iter < repeats:
        ti.profiler.clear_kernel_profiler_info()
        print(f'>>> Solving Poisson\'s equation using CG [{n_iter + 1}/{repeats}]')
        cgsolver.solve()
        ti.sync()
        kernel_time += ti.profiler.get_kernel_profiler_total_time() * 1000
        n_iter += 1
    print(f'*** Average kernel time: {kernel_time/repeats:6.3e} msec')

    # Check the residuals: norm(r) where r = Ax - b
    residual_cg = cgsolver.check_solution()  # Calculate the residual using explicit evaluation
    print('>>> Checking the residual norm(Ax-b)...')
    print(f'>>> Residual CGPoissonSolver: {residual_cg:4.2e}\n')
    assert(residual_cg < 1e-6)  # See reference: https://en.wikipedia.org/wiki/Conjugate_gradient_method
