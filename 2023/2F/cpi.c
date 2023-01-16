#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

const long long N = 10000000000;

int main(int argc, char **argv) {
  int process_Rank, size_Of_Cluster;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);
  MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

  double *data = NULL;
  if (process_Rank == 0) {
    data = (double *)malloc(size_Of_Cluster * sizeof(double));
  }
  double h = 1.0 / N;
  double acc = 0.0;
  for (long long i = process_Rank; i < N; i += size_Of_Cluster) {
    double xi = (i + 0.5) * h;
    acc += sqrt(1 - xi * xi) * h * 4;
  }

  MPI_Gather(&acc, 1, MPI_DOUBLE, data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (process_Rank == 0) {
    double ans = 0.0;
    for (int i = 0; i < size_Of_Cluster; ++i)
      ans += data[i];

    FILE *of = fopen("output.txt", "w");
    fprintf(of, "%.15f\n", ans);
    fclose(of);
  }

  MPI_Finalize();
  return 0;
}