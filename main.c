#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER_ID 0
#define RIGHT 0
#define LEFT 0
#define DISPLACEMENT 1

int main(int argc, char **argv)
{

  int world_size, rank;
  MPI_Init(&argc, &argv);

  MPI_Comm one_dimensional_topology;
  int size_of_dimension;
  int periods = 1, if_reorder = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size_of_dimension);
  MPI_Cart_create(MPI_COMM_WORLD, 1, &size_of_dimension, &periods, if_reorder, &one_dimensional_topology);
  int my_cordinates;
  MPI_Comm_rank(one_dimensional_topology, &rank);
  MPI_Cart_coords(one_dimensional_topology, rank, 1, &my_cordinates);

  int source, dest;
  MPI_Cart_shift(one_dimensional_topology, 0, DISPLACEMENT,&source, &dest);

  // printf("I am %d, source is %d, dest is %d, my coordinates are %d \n", rank, source, dest, my_cordinates);
  if (rank == MASTER_ID) {
  } else {
  }

  MPI_Finalize();
  return 0;
}