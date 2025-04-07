///////////////////////////|
//|File: mpi_hello.c
//|Author: Jerrin C. Redmon
//|Language: C
//|Version: 1.0.0
//|Date: March 31, 2025
///////////////////////////|

/* Descriptiion:
Write a simple MPI program that initializes the MPI environment, prints 
"Hello World" from 16 processes, and then finalizes the MPI environment.
*/

//----------------------------------------------------------------

// Includes //
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

// Main //
void main(int argc, char** argv){

    int size,rank;

    MPI_Init(&agrc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // print from processes
    printf("Hello from process %d of %d on host \n", world_rank, world_size,);

    MPI_Finalize()
    return 0;
}


