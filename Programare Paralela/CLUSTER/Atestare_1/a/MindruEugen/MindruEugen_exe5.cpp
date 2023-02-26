#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[])
{
    MPI_Status status; 

    int err[4], currentRank;
    MPI_Comm everyone;
    char *worker_program = (char *)"./L10Ex5.exe";

    MPI_Info hostinfo;
    char *host = (char *)"host";
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);

    MPI_Info_create(&hostinfo);
    MPI_Info_set(hostinfo, host, (char *)"compute-0-1,compute-0-0,compute-0-3");

    int vectorAmount, vectorSize;
    //cout << "Enter the vector amount: ";
    //cin >> vectorAmount;
    vectorAmount = 4;

    MPI_Comm_spawn(worker_program,
        MPI_ARGV_NULL, 
        vectorAmount, 
        hostinfo, 
        0, 
        MPI_COMM_WORLD,
        &everyone,
        err);

   cout << "Enter the vector size: ";
   //cin >> vectorSize;
    vectorSize = 3;

    MPI_Bcast(&vectorSize, 1, MPI_INT, MPI_ROOT, everyone);
        
    int rank_from_child;
    for(int i = 0; i < vectorAmount; i++)
    {
        int* vector = new int[vectorSize];

        for(int j = 0; j < vectorSize; j++)
        {
            vector[j] = (i + j) * vectorSize;
        }

        MPI_Send(vector, vectorSize, MPI_INT, i, 0, everyone);
    }

    double* norms = new double[vectorAmount];
    for(int i = 0; i < vectorAmount; i++)
    {
        double currentNorm;
        MPI_Recv(&currentNorm, 1, MPI_DOUBLE, i, 0, everyone, &status);
        printf("Root process received norm: %f from process %i\n", currentNorm, i);

        norms[i] = currentNorm;
    }

    double minNorm = norms[0];
    int minNormVectorNumber = 0;
    for(int i = 1; i < vectorAmount; i++)
    {
        if(minNorm > norms[i])
        {
            minNorm = norms[i];
            minNormVectorNumber = i;
        } 
    }

    printf("\n");
    printf("Minimal norm: %f", minNorm);
    printf(" in vector %i", minNormVectorNumber);
    printf("\n");

    MPI_Comm_disconnect(&everyone);

    MPI_Finalize();
    return 0;
}