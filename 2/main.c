#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mpi.h>

#define MASTER_ID 0
#define TAG 29

#define BATCH_SIZE 1000
#define ELEMENTS_SIZE 1000000

#define swap(x, y) { int temp = x; x = y; y = temp; }

int main(int argc, char **argv)
{

  int rank;
  MPI_Init(&argc, &argv);

  MPI_Comm one_dimensional_topology;
  int size_of_dimension;
  int periods = false, if_reorder = false;
  MPI_Comm_size(MPI_COMM_WORLD, &size_of_dimension);

  MPI_Cart_create(MPI_COMM_WORLD, 1, &size_of_dimension, &periods, if_reorder, &one_dimensional_topology);
  int my_cordinates;
  MPI_Comm_rank(one_dimensional_topology, &rank);
  MPI_Cart_coords(one_dimensional_topology, rank, 1, &my_cordinates);
  char * vector = malloc(ELEMENTS_SIZE * sizeof(*vector));
  if( rank == MASTER_ID){
    char value = 1;
    memset(vector, value,ELEMENTS_SIZE * sizeof(*vector));
    printf("I'm %d, filling vector with %d\n", rank, value);
    int sum = 0;
    for(int i = 0; i < ELEMENTS_SIZE; i++)
      sum += vector[i];
    printf("I'm %d, sum of my elements is %d\n", rank, sum);
  } 
  int number_of_batches = ELEMENTS_SIZE / BATCH_SIZE; 
  int source, dest;
  char* incoming = malloc(BATCH_SIZE * sizeof(*vector));
  int offset = 0;

  MPI_Cart_shift(one_dimensional_topology, 0, 1, &source, &dest);
  for(int i = 0; i < number_of_batches; i++){
    MPI_Recv(incoming, BATCH_SIZE, MPI_CHAR, source, TAG, one_dimensional_topology, MPI_STATUS_IGNORE);
    if( rank != MASTER_ID){
      memcpy(vector + offset, incoming, BATCH_SIZE);
    }
    MPI_Send(vector + offset, BATCH_SIZE, MPI_CHAR, dest, TAG, one_dimensional_topology);
    offset += BATCH_SIZE;
  }
  if ( rank == size_of_dimension - 1){
    int sum = 0;
    for(int i = 0; i < ELEMENTS_SIZE; i++){
      sum += vector[i];
    }
    printf("I'm %d, sum of my elements is %d\n", rank, sum);
  }
  MPI_Finalize();
  return 0;
}