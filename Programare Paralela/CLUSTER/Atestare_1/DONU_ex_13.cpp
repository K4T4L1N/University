/* AM FIXAT PROGRAMUL SA FUNCTIONEZE PE 8 PROCESE*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int INDEX = 2; // PROGRAMUL TRANSMITE LINIA "INDEX" catre procesul cu RANKUL "INDEX"
const int MATRIX_SIZE = 8;

using namespace std;
int main(int argc, char *argv[])
{
  int size, rank, t, namelen;

  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(processor_name, &namelen);

  int matrice[MATRIX_SIZE][MATRIX_SIZE] = {{1, 2, 3, 4, 5, 6, 7, 8}, {9, 10, 11, 12, 13, 14, 15, 16}, {17, 18, 19, 20, 21, 22, 23, 24}, {25, 26, 27, 28, 29, 30, 31, 32}, {33, 34, 35, 36, 37, 38, 39, 40}, {41, 42, 43, 44, 45, 46, 47, 48}, {49, 50, 51, 52, 53, 54, 55, 56}, {57, 58, 59, 60, 61, 62, 63, 64}};
  int linie[MATRIX_SIZE];

  int j;
  //
  if (rank == 0)
  {
    printf("Matricea :\n");
    fflush(stdout);
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
      printf("Linia %d : ", i + 1);
      fflush(stdout);
      for (int j = 0; j < MATRIX_SIZE; j++)
      {
        printf("%d ", matrice[i][j]);
        fflush(stdout);
      }

      printf("\n");
      fflush(stdout);
    }
    printf("\n=====REZULTATUL PROGRAMULUI======'%s' \n", argv[0]);
    fflush(stdout);

    MPI_Sendrecv(&matrice[INDEX - 1][0], MATRIX_SIZE, MPI_INT, INDEX, 10, &linie, 1, MPI_INT, INDEX, 10, MPI_COMM_WORLD, &status);
  }
  else if (rank == INDEX)
  {
    MPI_Sendrecv(&linie, 1, MPI_INT, 0, 10, &linie[0], MATRIX_SIZE, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == INDEX)
  {
    printf("A FOST RECEPTIONATA LINIA %d \n", INDEX);
    fflush(stdout);
    for (int j = 0; j < MATRIX_SIZE; j++)
    {
      printf("%d ", linie[j]);
      fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
  }

  MPI_Finalize();
  return 0;
}
