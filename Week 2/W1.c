#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMDATA 10000

void LoadData(int data[], int size) {
    for (int i = 0; i < size; i++) {
        data[i] = 1;
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int *data = NULL;
    int local_sum = 0, global_sum = 0;
    int local_size, remainder = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_size = NUMDATA / size;
    remainder = NUMDATA % size;

    int *local_data = (int *)malloc(local_size * sizeof(int));

    if (rank == 0) {
        data = (int *)malloc(NUMDATA * sizeof(int));
        LoadData(data, NUMDATA);
    }

    MPI_Scatter(data, local_size, MPI_INT, local_data, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_size; i++) {
        local_sum += local_data[i];
    }

    if (rank == 0) {
        for (int i = NUMDATA - remainder; i < NUMDATA; i++) {
            local_sum += data[i];
        }
    }

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("The total sum of data is %d\n", global_sum);
        free(data);
    }

    free(local_data);
    MPI_Finalize();

    return 0;
}
