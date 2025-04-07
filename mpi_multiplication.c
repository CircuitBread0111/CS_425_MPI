///////////////////////////|
//|File: mpi_addition.c
//|Author: Jerrin C. Redmon
//|Language: C
//|Version: 1.0.0
//|Date: March 31, 2025
///////////////////////////|

/* Descriptiion:
Write an MPI program to perform matrix multiplication with a 10000 x 10000 matrix of random numbers. 
The program should distribute the rows of the first matrix among the processes and compute the resulting matrix in parallel. 
Calculate the result with serial code to confirm your results. How much faster was the parallel program?
*/

//----------------------------------------------------------------

// Includes //
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MATRIX_SIZE 5000


/// Main //
int main(int argc, char *argv[]) {
   
    // Initialize MPI //
    int rank, size;
    MPI_Init(&argc, &argv);                                                                                                     // initialize the MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);                                                                                       // get the rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size);                                                                                       // get the number of processes

    // Variables //
    int *matrix1 = NULL, *matrix2 = NULL, *result = NULL, *serial_result = NULL;                                                // holds the matrices
    double start, end, serial_time, parallel_time;                                                                              // holds the start and end time
    int rows_per_proc = MATRIX_SIZE / size;                                                                                     // number of rows per process
    int *local_matrix1 = malloc(rows_per_proc * MATRIX_SIZE * sizeof(int));                                                     // holds the local matrix
    int *local_result = malloc(rows_per_proc * MATRIX_SIZE * sizeof(int));                                                      // holds the local result
    matrix2 = malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));                                                                  // holds the second matrix

    // Initialize matrices //
    if (rank == 0) {
        matrix1 = malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));                                                              // allocate memory for the first matrix
        result = malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));                                                               // allocate memory for the result matrix
        serial_result = malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));                                                        // allocate memory for the serial result matrix
        srand(time(NULL));                                                                                                      // seed the random number generator

        for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
            matrix1[i] = rand() % 100;                                                                                          // fill the first matrix with random numbers
            matrix2[i] = rand() % 100;                                                                                          // fill the second matrix with random numbers
            serial_result[i] = 0;                                                                                               // initialize the serial result matrix to 0
        }


        start = MPI_Wtime();                                                                                                    // start the timer
        for (int i = 0; i < MATRIX_SIZE; i++) {                                                                                 // loop through each row
            for (int j = 0; j < MATRIX_SIZE; j++) {                                                                             // loop through each column
                for (int k = 0; k < MATRIX_SIZE; k++) {                                                                         // loop through each element
                    serial_result[i * MATRIX_SIZE + j] += matrix1[i * MATRIX_SIZE + k] * matrix2[k * MATRIX_SIZE + j];          // serial multiplication
                }
            }
        }
        end = MPI_Wtime();                                                                                                      // end the timer    
        serial_time = end - start;                                                                                              // calculate the serial time
        printf("Serial Time: %.4f seconds\n", serial_time);
    }


    // Broadcast matrix2 //
    MPI_Bcast(matrix2, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Scatter matrix1 rows //
    MPI_Scatter(matrix1, rows_per_proc * MATRIX_SIZE, MPI_INT,
                local_matrix1, rows_per_proc * MATRIX_SIZE, MPI_INT,
                0, MPI_COMM_WORLD);

    // Parallel //
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();                                                                                                        // start the timer

    for (int i = 0; i < rows_per_proc; i++) {                                                                                   // loop through each row                                          
        for (int j = 0; j < MATRIX_SIZE; j++) {                                                                                 // loop through each column 
            int sum = 0;                                                                                                        // initialize the sum to 0                                
            for (int k = 0; k < MATRIX_SIZE; k++) {                                                                             // loop through each element  
                sum += local_matrix1[i * MATRIX_SIZE + k] * matrix2[k * MATRIX_SIZE + j];                                       // parallel multiplication 
            }
            local_result[i * MATRIX_SIZE + j] = sum;            
        }
    }

    // Gather results //
    MPI_Gather(local_result, rows_per_proc * MATRIX_SIZE, MPI_INT,                                                              // gather the local results                   
               result, rows_per_proc * MATRIX_SIZE, MPI_INT,                                                    
               0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);                                                                                                // synchronize processes
    end = MPI_Wtime();                                                                                                          // end the timer    
    parallel_time = end - start;                                                                                                // calculate the parallel time  

    if (rank == 0) {
        // Compare results //
        int correct = 1;
        for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {                                                                   // loop through each element
            if (result[i] != serial_result[i]) {                                                                                // check if the result matches the serial result
                correct = 0;
                break;
            }
        }
        printf("Parallel Time: %.4f seconds\n", parallel_time);
        printf("Speedup: %.2fx\n", serial_time / parallel_time);
        printf("Result: %s\n", correct ? "Match" : "Mismatch!");

        // Free memory //
        free(matrix1);                                                                                                          // free the first matrix                                                          
        free(result);                                                                                                           // free the result matrix                      
        free(serial_result);                                                                                                    // free the serial result matrix
    }

    free(matrix2);                                                                                                              // free the second matrix
    free(local_matrix1);                                                                                                        // free the local matrix
    free(local_result);                                                                                                         // free the local result matrix

    MPI_Finalize();                                                                                                             // finalize the MPI environment
    return 0;                                                                                                                   // return 0   
}
