#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, ball = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        while (ball < 10) {
            MPI_Send(&ball, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            printf("Process 0 sent ball = %d to Process 1\n", ball);
            MPI_Recv(&ball, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process 0 received ball = %d from Process 1\n", ball);
        }
        printf("Process 0: Ball reached 10, stopping.\n");
    } else if (rank == 1) {
        while (1) {
            MPI_Recv(&ball, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (ball >= 10) break;
            ball++;
            printf("Process 1 incremented ball to %d and sent it back\n", ball);
            MPI_Send(&ball, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
