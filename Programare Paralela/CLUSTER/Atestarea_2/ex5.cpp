#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
int main(int argc, char** argv)
{

int rank, size, namelen, version, subversion, l = 5;
int globalrank, sumrank;

struct {
int norm;
int rank;
} in, out;

int *vectors;
int *vector;

vectors=(int*)malloc(l*l*sizeof(int));
vector=(int*)malloc(l*sizeof(int));

for(int i=0;i<l*l;i++)
  vectors[i]=rand()%10;

for(int i=0;i<l;i++)
{
	printf("\n");
	for(int j=0;j<l;j++) printf("v[%d,%d]=%d ",i,j,vectors[i*l+j]);
}
printf("\n");
	
MPI_Comm family_comm,allcomm;
char processor_name[MPI_MAX_PROCESSOR_NAME],
worker_program[100] = "ex5_worker.exe";
int rank_from_child,ich;

MPI_Status status;
MPI_Info hostinfo;
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Get_processor_name(processor_name,&namelen);
MPI_Get_version(&version,&subversion);

if (size != 1) printf("Error: Only one manager process should be running, but %d were started.\n", size);

MPI_Info_create(&hostinfo);
MPI_Info_set(hostinfo, (char*)"host", (char*)"compute-0-3,compute-0-10,compute-0-11" );
printf("Spawning %d worker processes running %s\n", l, worker_program);
MPI_Comm_spawn(worker_program, MPI_ARGV_NULL,l,hostinfo,0,MPI_COMM_SELF,&family_comm, MPI_ERRCODES_IGNORE);


MPI_Barrier(MPI_COMM_WORLD);
MPI_Bcast(&l,1,MPI_INT,MPI_ROOT, family_comm);
MPI_Scatter(vectors, l, MPI_INT,vector, l, MPI_INT, MPI_ROOT, family_comm);


MPI_Intercomm_merge(family_comm,1, &allcomm);
MPI_Comm_rank(allcomm, &globalrank);


MPI_Reduce(&in,&out,1,MPI_2INT,MPI_MINLOC,0,allcomm);
printf("===Result===\n");

printf("rank = %d norm = %d\n",out.rank,out.norm);
for(int i=0;i<l;i++) printf("v[%d]=%d ",i,vectors[out.rank * l + i]);


MPI_Comm_disconnect(&family_comm);
MPI_Finalize();
return 0;
}
