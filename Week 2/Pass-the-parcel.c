#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank, size, parcel;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size < 3) {
        if (rank == 0) {
            printf("This program requires at least 3 processes to run.\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    if (rank == 0) {
        parcel = 1; // Initialize parcel
        MPI_Send(&parcel, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&parcel, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Final parcel value at Process %d: %d\n", rank, parcel);
    } else {
        MPI_Recv(&parcel, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        parcel += 1;
        int next = (rank + 1) % size;
        MPI_Send(&parcel, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
}
