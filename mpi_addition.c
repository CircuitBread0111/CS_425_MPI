///////////////////////////|
//|File: mpi_addition.c
//|Author: Jerrin C. Redmon
//|Language: C
//|Version: 1.0.0
//|Date: March 31, 2025
///////////////////////////|

/* Descriptiion:
Write an MPI program that distributes an array of the first 1000000, 1 through  1000000 integers
among 4 processes and calculates the sum of the array elements using MPI. 
Each process should compute the sum of its portion of the array and then use 
MPI to gather the results and compute the total sum. Capture the wall time.

Repeat with 30 processes and compare the wall time. Be sure and have your code confirm 
the correctness of the calculation using Euler’s formula: S = n(n+1)/2 where S is the sum
and n is the value of the last integer in the sequence.
*/

//----------------------------------------------------------------

// Includes //
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAIN_ARRAY 1000000  

// Main //
int main(int argc, char *argv[]) {

    // Initialize MPI //
    int rank;                                                                             // get the rank of the process
    int size;                                                                             // get the number of processes
    MPI_Init(&argc, &argv);                                                               // initialize the MPI environment
    MPI_Comm_size(MPI_COMM_WORLD, &size);             
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   
    


    // Variables //
    int *array_data = NULL;                                                               // holds the array data
    int array_element = MAIN_ARRAY / size;                                                // holds the number of elements for each process    
    int remainder = MAIN_ARRAY % size;                                                    // sets the remainder for each process          
    long long subprocess_sum = 0;                                                         // holds the sum of each process
    long long total_sum = 0;                                                              // holds the total sum
   // double start_time, end_time;                                                        // holds the start and end time
    int* element_total = NULL;                                                            // array to hold the number of elements to send to each process
    int* displacement = NULL;                                                             // array to hold the displacement of each process



    // Sub_Array //
    int sub_array_size = array_element + (rank < remainder ? 1 : 0);                      // size of each process's subarray
    int *sub_array = (int *)malloc(sizeof(int) * sub_array_size);                         // allocate memory for each process's subarray



    // Main_Array //
    if (rank == 0) {                                                                      // only the root process creates the main array
        array_data = (int *)malloc(sizeof(int) * MAIN_ARRAY);                             // allocate memory for the main array
        for (int i = 0; i < MAIN_ARRAY; i++) {                                            // fill the main array with values
            array_data[i] = i + 1;                                                        // fill the main array with values
        }
    }


    // Scatter //

    if (rank == 0) {
        element_total = (int*)malloc(sizeof(int) * size);                                 // allocate memory for the number of elements to send to each process
        displacement = (int*)malloc(sizeof(int) * size);                                  // allocate memory for the displacement of each process
        int offset = 0;                                                                   // set the offset to 0
        for (int i = 0; i < size; i++) {                                                  // loop through each process
            element_total[i] = array_element + (i < remainder ? 1 : 0);                   // set the number of elements to send to each process
            displacement[i] = offset;                                                     // set the displacement of each process
            offset += element_total[i];                                                   // increment the offset by the number of elements sent to each process
        }
        printf("Displacement: ", displacement[rank], "\n");                               // print the displacement of each process
        printf("Element Total: ", element_total[rank], "\n");                             // print the number of elements to send to each process
        printf("Sub Array Size: ", sub_array_size, "\n");                                 // print the size of each process's subarray
        printf("Rank: ", rank, "\n");                                                     // print the rank of each process
        printf("Size: ", size, "\n");                                                     // print the number of processes
    }
   
    double start_time = MPI_Wtime();                                                      // Start timing

    MPI_Scatterv(array_data, element_total, displacement, MPI_INT,                        // scatter the main array to each process
                 sub_array, sub_array_size, MPI_INT, 0, MPI_COMM_WORLD); 

  
    // Each process computes its local sum
    long long local_sum = 0;                                                              // Initialize local sum        
    for (int i = 0; i < sub_array_size; i++) {                                            // loop through each process's subarray
        local_sum += sub_array[i];                                                        // compute the local sum
    }
    // Reduce all local sums to a total sum on root
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);     // reduce the local sum to the total sum on root
    double end_time = MPI_Wtime();                                                        // End timing



    // Root process prints the result and checks correctness    
    if (rank == 0) {                                                                      // only the root process prints the result
        long long euler_sum = ((long long)MAIN_ARRAY * (MAIN_ARRAY + 1)) / 2;             // compute the expected sum using Euler's formula
        printf("Computed sum: %lld\n", total_sum);                                        // print the computed sum
        printf("Euler's Formula: %lld\n", euler_sum);                                     // print the expected sum
        printf("Wall time with %d processes: %f seconds\n", size, end_time - start_time); // print the wall time
    }


    // Free memory
    free(sub_array);                                                                      // free the subarray
    if (rank == 0) {                                                                      // only the root process frees the main array
        free(array_data);                                                                 // free the main array
        free(element_total);                                                              // free the number of elements to send to each process
        free(displacement);                                                               // free the displacement of each process
    }
    MPI_Finalize();                                                                       // finalize the MPI environment
    return 0;                                                                             // return 0
}   


