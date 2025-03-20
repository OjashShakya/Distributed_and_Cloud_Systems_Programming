#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define CHUNK_SIZE 4096

void count_letters(const char *text, int *local_counts) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            local_counts[tolower(text[i]) - 'a']++;
        }
    }
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int local_counts[ALPHABET_SIZE] = {0};
    int global_counts[ALPHABET_SIZE] = {0};
    char *buffer = NULL;
    long file_size = 0;
    
    if (rank == 0) {
        FILE *file = fopen("WarAndPeace.txt", "r");
        if (!file) {
            printf("Error: Could not open file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = (char *)malloc(file_size + 1);
        fread(buffer, 1, file_size, file);
        buffer[file_size] = '\0';
        fclose(file);
    }
    
    MPI_Bcast(&file_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    int chunk_size = file_size / size;
    char *chunk = (char *)malloc((chunk_size + 1) * sizeof(char));
    
    MPI_Scatter(buffer, chunk_size, MPI_CHAR, chunk, chunk_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    chunk[chunk_size] = '\0';
    
    count_letters(chunk, local_counts);
    
    MPI_Reduce(local_counts, global_counts, ALPHABET_SIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("Letter frequencies:\n");
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            printf("%c: %d\n", 'a' + i, global_counts[i]);
        }
        free(buffer);
    }
    
    free(chunk);
    MPI_Finalize();
    return 0;
}
