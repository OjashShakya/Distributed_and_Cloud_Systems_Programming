#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;

    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    
    // Get the rank and size of the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Print hello world message
    printf("Hello, World! I am process %d of %d\n", rank, size);
   // Finalize MPI environment
    MPI_Finalize();
    return 0;
}

//To Run:
//mpicc T1.c -o T1
//mpirun ./T1 