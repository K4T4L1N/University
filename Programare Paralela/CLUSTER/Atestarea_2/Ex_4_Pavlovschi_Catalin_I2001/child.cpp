#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char** argv)
{

int rank, size, namelen, version, subversion, psize;
int parent_rank;
int globalrank,sumrank;

MPI_Comm parent,allcom;

char processor_name[MPI_MAX_PROCESSOR_NAME];

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Get_processor_name(processor_name,&namelen);
MPI_Get_version(&version,&subversion);

printf("I'm child %d of %d on %s running MPI %d.%d\n", rank, size, processor_name, version,subversion);

MPI_Comm_get_parent(&parent);

if (parent == MPI_COMM_NULL)
{
printf("!!! no parent process found !!!\n");
exit(1);
}

MPI_Comm_remote_size(parent,&psize);

if (psize!=1)
{
printf("!!! number of parents (%d) should be 1 !!!\n", psize);
exit(2);
}

MPI_Intercomm_merge(parent,1,&allcom);
MPI_Comm_rank(allcom, &globalrank);

printf("CHILD: my globalrank is %d,rank is %d \n",globalrank, rank);

MPI_Allreduce(&globalrank,&sumrank,1, MPI_INT,MPI_SUM,allcom); //functia allreduce ca la procesul parinte

printf("sumrank after reduce on process %d is %d \n", rank,sumrank);
MPI_Comm_disconnect(&parent);
MPI_Finalize();
return 0;
}
