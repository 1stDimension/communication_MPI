#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/random.h>

#define MASTER_ID 0
#define RIGHT 0
#define LEFT 0
#define DISPLACEMENT 1
#define TAG 29

#define swap(x, y) { int temp = x; x = y; y = temp; }

int gcd( int a, int b);

int main(int argc, char **argv)
{

  int rank;
  MPI_Init(&argc, &argv);

  MPI_Comm one_dimensional_topology;
  int size_of_dimension;
  int periods = 1, if_reorder = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size_of_dimension);

  MPI_Cart_create(MPI_COMM_WORLD, 1, &size_of_dimension, &periods, if_reorder, &one_dimensional_topology);
  int my_cordinates;
  MPI_Comm_rank(one_dimensional_topology, &rank);
  MPI_Cart_coords(one_dimensional_topology, rank, 1, &my_cordinates);


  unsigned int my_number;
  getrandom(&my_number, sizeof(my_number), GRND_RANDOM);
  my_number >>= 16;
  #ifdef NOT_RANDOM 
  my_number = (rank*rank*rank + 1) * 4 + 100;
  #endif
  int outgoing = my_number;
  int incoming;

  printf("I am %d and my number is %d\n", rank, my_number); 
  const int number_of_sends = log2(size_of_dimension);
  int source, dest;
  for(int i = 0; i < number_of_sends ; i++){
    MPI_Cart_shift(one_dimensional_topology, 0, 1 << i ,&source, &dest);
    #ifdef DEBUG
      printf("Send phase number %d, I am %d, shift is %d\n", i ,rank, 1 << i);
    #endif
    MPI_Sendrecv(&outgoing, 1, MPI_INT, dest, TAG, &incoming, 1, MPI_INT, source, TAG, one_dimensional_topology, MPI_STATUS_IGNORE);
    #ifdef DEBUG
      printf("Send phase number %d, I am %d, received %d, send %d\n", i ,rank, incoming, outgoing);
    #endif
    outgoing = gcd(incoming, outgoing); 
    #ifdef DEBUG
      printf("Send phase number %d, I am %d, my gcd is %d\n", i, rank, outgoing);
    #endif
  }
  if(rank == 0){
    printf("GCD of all numbers is %d\n", outgoing);
  }

  MPI_Finalize();
  return 0;
}

int gcd(int a, int b){
  int greater, lower, remainder = 2;
  if (a < b){
    swap(a,b)
  }
  while(remainder != 0){
    remainder = a % b;
    swap(a,b);
    b = remainder;
  }
  return a;
}