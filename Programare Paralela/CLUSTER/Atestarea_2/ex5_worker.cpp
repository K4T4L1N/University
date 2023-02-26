#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char** argv)
{
int rank, size, namelen, version, subversion, psize,l;
int parent_rank;
int globalrank,sumrank;
MPI_Comm parent,allcom;
char processor_name[MPI_MAX_PROCESSOR_NAME];

int *vectors;

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Comm_size(MPI_COMM_WORLD,&size);
MPI_Get_processor_name(processor_name,&namelen);
MPI_Get_version(&version,&subversion);
printf("I'm worker %d of %d on %s running MPI %d.%d\n", rank, size, processor_name, version,subversion);
MPI_Comm_get_parent(&parent);
if (parent == MPI_COMM_NULL)
{
printf("Error: no parent process found!\n");
exit(1);
}
MPI_Comm_remote_size(parent,&psize);
if
(psize!=1)
{
printf("Error: number of parents (%d) should be 1.\n", psize);
exit(2);
}

MPI_Barrier(MPI_COMM_WORLD);
MPI_Bcast(&l,1,MPI_INT,0,parent);
MPI_Barrier(MPI_COMM_WORLD);
struct {
int norm;
int rank;
	   } in, out;
printf("I am %d my l = %d\t",rank,l);
int vector[l];

MPI_Scatter(vectors, l, MPI_INT,vector, l, MPI_INT, 0, parent);
printf("and my vector is ");
for(int i=0;i<l;i++) printf("v[%d]=%d  ",i,vector[i]);

int norm = 0;
for(int i=0;i<l;i++){
	norm += vector[i] * vector[i];
}
printf(" norm = %d\n",norm);


MPI_Intercomm_merge(parent,1,&allcom);
MPI_Comm_rank(allcom, &globalrank);

in.norm = norm;
in.rank = rank;
MPI_Reduce(&in,&out,1,MPI_2INT,MPI_MINLOC,0,allcom);
MPI_Comm_disconnect(&parent);
MPI_Finalize();
return 0;
} 