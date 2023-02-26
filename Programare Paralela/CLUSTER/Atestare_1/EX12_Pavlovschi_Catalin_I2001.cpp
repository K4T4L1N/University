#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[])
{
  	int numtask,sendcount,reccount,source;
	double *Matr_Init,*Matr_Fin;         //matricea initiala si finala
	int i, myrank, root=0;               //myrank - rankul, root - numarul procesului care distribuie datele

	MPI_Init(&argc,&argv);                //initializam mediu

	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);        // comunicatorul cu identificatorul procesului myrank
	MPI_Comm_size(MPI_COMM_WORLD, &numtask);       // numarul de procese(numtask) ale comunicatorului
	MPI_Status status;
	double Rows[numtask];                       // Vectorul liniilor cu dimensiunea nr de procese
	sendcount=numtask;
	reccount=numtask;                      // initializam sendcount si reccount cu nr de procese
if (myrank ==0)
printf("\n=====REZULTATUL PROGRAMULUI '%s' \n",argv[0]);
MPI_Barrier(MPI_COMM_WORLD);                                  //blocam apelul pana cand toate procesele ajung la bariera

//Procesul cu rankul root aloca spatiul si initializeaza matrice
	if(myrank==root)
	{                                                                // daca rankul este egal cu numarul procesului root
	Matr_Init=(double*)malloc(numtask*numtask*sizeof(double));
         Matr_Fin=(double*)malloc(numtask*numtask*sizeof(double));   // se aloca spatiul
	for(int i=0;i<numtask*numtask;i++)
  	Matr_Init[i]=rand()/1000000000.0;       // se initializeaza matricea
	printf("Tipar datele initiale\n");
	for(int i=0;i<numtask;i++)
	{
	    // se afiseaza datele matricii initiale
	printf("\n");
	for(int j=0;j<numtask;j++)
	printf("Matr_Init[%d,%d]=%5.2f ",i,j,Matr_Init[i*numtask+j]);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	}
	else MPI_Barrier(MPI_COMM_WORLD);
MPI_Scatter(Matr_Init, sendcount, MPI_DOUBLE,Rows, reccount, MPI_DOUBLE, root, MPI_COMM_WORLD);
// distribuirea colectiva a datelor
for(int i=0; i<numtask; i++){
        for(int j=0; j<numtask; j++){
                if(myrank==root){
                        // daca rankul este egal cu nr procesului radacina se transmite mesajul
                MPI_Send(&Matr_Init[i*numtask+j], 1, MPI_DOUBLE, i, 10, MPI_COMM_WORLD);
            }
                if(myrank==i){
                        // daca rankul este egal cu i se receptioneaza mesajele in vectorul Rows
                     MPI_Recv(&Rows[j], 1, MPI_DOUBLE, root, 10, MPI_COMM_WORLD, &status);
                }
        }
}


double Rows2[numtask];

for(int j=0; j<numtask; j++){                             // se parcurge doar un ciclu
    if(myrank==root){

        MPI_Sendrecv(&Matr_Init[j], 1, MPI_DOUBLE, root+1, 10,&Rows[j], 1, MPI_DOUBLE, root+1, 10, MPI_COMM_WORLD, &status);
        // ^ daca rankul este egal cu nr procesului radacina prin sendrecv se transmite mesajul destinatarului + 1
    }
    if(myrank==root+1){
	        MPI_Sendrecv(&Rows2[j], 1, MPI_DOUBLE, root, 10,&Rows2[j], 1, MPI_DOUBLE, root, 10, MPI_COMM_WORLD, &status);
	        // ^ daca rankul este egal cu nr procesului radacina + 1 prin sendrecv se transmite mesajul destinatarului     }


    }
}
                 // se afiseaza rezultatele
printf("\n");
printf("Resultatele f-tiei Send & recv pentru procesul cu rankul %d \n", myrank);
for (i=0; i<numtask; ++i)
printf("Rows[%d]=%5.2f ",i, Rows[i]);
printf("\n");
MPI_Barrier(MPI_COMM_WORLD);

 MPI_Finalize();
return 0;
}
