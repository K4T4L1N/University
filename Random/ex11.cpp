/*====
In acest program se distrubuie liniile  unei matrici de dimensiuni arbitrare proceselor din comunicatorul MPI_COMM_WORLD. Elementele matricei sunt initializate de procesul cu rankul 0.
====*/
#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

double *GetTransposedMatrix(double *originalMatrix, int rowAmount, int columnAmount)
{
    cout << endl;

    double *transposedMatrix = (double *) malloc(rowAmount * columnAmount * sizeof(double));

    for(int i = 0; i < columnAmount; i++)
    {
        for(int j = 0; j < rowAmount; j++)
        {
            transposedMatrix[i * rowAmount + j] = originalMatrix[j * columnAmount + i];
        }
    }

    return transposedMatrix;
}

void PrintMatrix(double *matrix, int rowAmount, int columnAmount)
{
    for(int i = 0; i < rowAmount; i++)
    {
        for(int j = 0; j < columnAmount; j++)
        {
            printf("M[%i, %i] = %4.4f; ", i, j, matrix[i * columnAmount + j]); 
        }

        cout << endl;
    }
}

int main(int argc, char *argv[])
{
    int size, receiveCount;
    int currentRank, rootRank = 0;
    
    int rowAmount, columnAmount;
    double *matrix;

    int transposedRowAmount, transposedColumnAmount;
    double *transposedMatrix;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int displacements[size];
    int elementsToSend[size];

    double *dataToReceive;

    if (currentRank == rootRank)
    {
        printf("\n========== Program '%s' ==========\n", argv[0]);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(currentRank == rootRank)
    {
        cout << "Enter line amount: ";
        cin >> rowAmount;
        cout << "Enter column amount: ";
        cin >> columnAmount;

        matrix = (double *) malloc(rowAmount * columnAmount * sizeof(double));
        
        for (int i = 0; i < rowAmount * columnAmount; i++)
            matrix[i] = (double)i;

        cout << "The original matrix:" << endl;
        PrintMatrix(matrix, rowAmount, columnAmount);


        transposedMatrix = GetTransposedMatrix(matrix, rowAmount, columnAmount);

        transposedColumnAmount = rowAmount;
        transposedRowAmount = columnAmount;

        cout << "Transposed matrix:" << endl;
        PrintMatrix(transposedMatrix, transposedRowAmount, transposedColumnAmount);
    }

    sleep(2 * currentRank);
    
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(&transposedRowAmount, 1, MPI_INT, rootRank, MPI_COMM_WORLD);
    MPI_Bcast(&transposedColumnAmount, 1, MPI_INT, rootRank, MPI_COMM_WORLD);

    int rowsPerProcess = transposedRowAmount / size;
    int rowsLeft = transposedRowAmount % size;

    //printf("Columns per process: %i\n", rowsPerProcess);
    //printf("Columns left: %i\n", rowsLeft);

    int displacement = 0;
    for (int i = 0; i < size; i++)
    {
        displacements[i] = displacement;

        if (i < rowsLeft)
            elementsToSend[i] = (rowsPerProcess + 1) * transposedColumnAmount;
        else
            elementsToSend[i] = rowsPerProcess * transposedColumnAmount;
        
        displacement += elementsToSend[i];
    }

    receiveCount = elementsToSend[currentRank];
    dataToReceive = new double[receiveCount];

    MPI_Scatterv(transposedMatrix, 
        elementsToSend, 
        displacements, 
        MPI_DOUBLE, 
        dataToReceive,
        receiveCount, 
        MPI_DOUBLE, 
        rootRank, 
        MPI_COMM_WORLD);

    printf("\nThe process with rank %i has received %i elements", 
        currentRank, receiveCount);
    printf("\nThe received column(s):\n");

    for (int i = 0; i < receiveCount; i++)
    {
        cout << "M[" << i << "] = " << dataToReceive[i] << "; ";

        if(i > 0 && (i + 1) % transposedColumnAmount == 0)
        {
            cout << endl;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int sendLength = receiveCount;
    int *elementsToReceive = elementsToSend;

    double *finalTransposedMatrix = (double *) malloc(rowAmount * columnAmount * sizeof(double));

    MPI_Gatherv(dataToReceive, 
        sendLength, 
        MPI_DOUBLE, 
        finalTransposedMatrix, 
        elementsToReceive, 
        displacements,
        MPI_DOUBLE,
        rootRank, 
        MPI_COMM_WORLD);

    sleep(2 * currentRank);
    MPI_Barrier(MPI_COMM_WORLD);

    if(currentRank == rootRank)
    {
        double *finalMatrix = GetTransposedMatrix(finalTransposedMatrix, transposedRowAmount, transposedColumnAmount);

        cout << "The root rank receives the columns:" << endl;

        for (int i = 0; i < rowAmount; i++)
        {
            for(int j = 0; j < columnAmount; j++)
            {
                printf("FinalMatrix[%i, %i] = %4.4f; ", i, j, finalMatrix[i * columnAmount + j]);
            }

            cout << endl;
        }
    }


    MPI_Finalize();

    return 0;
}