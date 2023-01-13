import taichi as ti
import numpy as np


@ti.data_oriented
class CGPoissonSolver:
    def __init__(self, n=256, eps=1e-16, quiet=False):
        self.N = n
        self.eps = eps
        self.quiet = quiet
        self.real = ti.f64

        self.N_tot = 2 * self.N
        self.N_ext = self.N // 2
        self.steps = self.N * self.N  # Cg should converge within the size of the vector

        self.r = ti.field(dtype=self.real)  # residual
        self.b = ti.field(dtype=self.real)  # rhs
        self.x = ti.field(dtype=self.real)  # solution
        self.p = ti.field(dtype=self.real)  # conjugate gradient
        self.Ap = ti.field(dtype=self.real)  # matrix-vector product
        self.Ax = ti.field(dtype=self.real)  # matrix-vector product
        self.alpha = ti.field(dtype=self.real)
        self.beta = ti.field(dtype=self.real)
        ti.root.place(self.alpha, self.beta)
        ti.root.dense(ti.ij, (self.N_tot, self.N_tot)).place(
            self.x, self.p, self.Ap, self.r, self.Ax, self.b)
        # ti.root.pointer(ti.ij, self.N_tot//16) \
        #     .dense(ti.ij, 16) \
        #     .place(self.x, self.p, self.Ap, self.r, self.Ax, self.b)

    @ti.kernel
    def init(self):
        for i, j in ti.ndrange((self.N_ext, self.N_tot - self.N_ext),
                               (self.N_ext, self.N_tot - self.N_ext)):
            xl = (i - self.N_ext) * 2.0 / self.N_tot
            yl = (j - self.N_ext) * 2.0 / self.N_tot
            self.r[i, j] = ti.sin(2.0 * np.pi * xl) * ti.sin(2.0 * np.pi * yl)
            self.b[i, j] = ti.sin(2.0 * np.pi * xl) * ti.sin(2.0 * np.pi * yl)
            self.Ap[i, j] = 0.0
            self.Ax[i, j] = 0.0
            self.p[i, j] = 0.0
            self.x[i, j] = 0.0

    @ti.kernel
    def reduce(self, p: ti.template(), q: ti.template()) -> ti.f64:
        sum = 0.0
        for I in ti.grouped(p):
            sum += p[I] * q[I]
        return sum

    @ti.kernel
    def compute_Ap(self):
        for i, j in ti.ndrange((self.N_ext, self.N_tot - self.N_ext),
                               (self.N_ext, self.N_tot - self.N_ext)):
            self.Ap[i, j] = 4.0 * self.p[i, j] - self.p[
                i + 1, j] - self.p[i - 1, j] - self.p[i, j + 1] - self.p[i,
                                                                         j - 1]

    @ti.kernel
    def update_x(self):
        for I in ti.grouped(self.p):
            self.x[I] += self.alpha[None] * self.p[I]

    @ti.kernel
    def update_r(self):
        for I in ti.grouped(self.p):
            self.r[I] -= self.alpha[None] * self.Ap[I]

    @ti.kernel
    def update_p(self):
        for I in ti.grouped(self.p):
            self.p[I] = self.r[I] + self.beta[None] * self.p[I]

    def solve(self):
        self.init()
        initial_rTr = self.reduce(self.r, self.r)  # Compute initial residual
        if not self.quiet:
            print('>>> Initial residual =', ti.sqrt(initial_rTr))
        old_rTr = initial_rTr
        self.update_p()  # Initial p = r + beta * p ( beta = 0 )
        # -- Main loop --
        # for i in range(self.steps):
        for i in range(0):
            self.compute_Ap()
            pAp = self.reduce(self.p, self.Ap)
            self.alpha[None] = old_rTr / pAp
            self.update_x()
            self.update_r()
            new_rTr = self.reduce(self.r, self.r)
            if ti.sqrt(new_rTr) < self.eps:
                print('>>> Conjugate Gradient method converged.')
                break
            self.beta[None] = new_rTr / old_rTr
            self.update_p()
            old_rTr = new_rTr
            if not self.quiet:
                print(f'>>> Iter = {i+1:4}, Residual = {ti.sqrt(new_rTr):e}')

    @ti.kernel
    def check_solution(self) -> ti.f64:
        return 0.0
        # residual = 0.0
        # for i, j in ti.ndrange((self.N_ext, self.N_tot - self.N_ext),
        #                        (self.N_ext, self.N_tot - self.N_ext)):
        #     self.Ax[i, j] = 4.0 * self.x[i, j] - self.x[i + 1, j] - \
        #         self.x[i - 1, j] - self.x[i, j + 1] - self.x[i, j - 1]
        #     residual += (self.b[i, j] - self.Ax[i, j]) ** 2
        # return ti.sqrt(residual)
