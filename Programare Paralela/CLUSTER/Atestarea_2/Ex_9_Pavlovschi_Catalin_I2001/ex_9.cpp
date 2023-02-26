#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <cstdlib>
typedef struct
{
float diagonala[3];
} Particle;

MPI_Datatype MPI_Particle;

void construct_datatypes(){
    Particle p;
    int blens[1];
    MPI_Aint displ[1];
    MPI_Datatype types[1];
    blens[0]=3;
    types[0]=MPI_FLOAT;
    displ[0]=(MPI_Aint)&p.diagonala-(MPI_Aint)&p;
    MPI_Type_struct(1,blens,displ,types,&MPI_Particle);
    MPI_Type_commit(&MPI_Particle);
    return;
}

int main(int argc, char *argv[]){
    int nProc,myRank,i;
    Particle* myP = (Particle*)std::calloc(1,sizeof(Particle));
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nProc);
    MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
    construct_datatypes();
    if (myRank ==0) printf("\n=====REZULTATUL PROGRAMULUI '%s' \n",argv[0]);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    if(myRank == 0){
        float matrix[3][3] = {{0,1,2}, {2,3,4}, {5,6,7}};
        printf("== MATRIX ==\n");
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++){
                printf("%.2f ", matrix[i][j]);
                if(i==j) myP[0].diagonala[i] = matrix[i][j];
            }
            printf("\n");
        }
        
    }

    MPI_Bcast(myP,1,MPI_Particle,0, MPI_COMM_WORLD);
    printf("Proces rank %d: diagonala particuleei (%f, %f, %f) \n", myRank, myP[0].diagonala[0], myP[0].diagonala[1], myP[0].diagonala[2]);
    MPI_Finalize();
    return 0;
}



