//Sa se elaboreze si sa se execute pe clusterul USM un program MPI
//în limbajul C++, prin care sa se verifice daca procesul „parinte” si
//procesele „fiu” pot utiliza aceeasi operatie de reducere (am utilizat operatia ALLREDUCE)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


int main(int argc, char* argv[])
{

int rank, size, namelen, version, subversion, universe_size;
int globalrank,sumrank;

MPI_Comm family_comm,allcomm;

char processor_name[MPI_MAX_PROCESSOR_NAME], worker_program[100];
int rank_from_child,ich;

MPI_Status status;
MPI_Info hostinfo;
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Get_processor_name(processor_name,&namelen);
MPI_Get_version(&version,&subversion);

printf("I'm manager %d of %d on %s running MPI %d.%d\n", rank, size, processor_name, version,subversion);

if (size != 1)
printf("!!! Only one parent process should be running, but %d were initiated. !!!\n", size);

universe_size = 8;

MPI_Info_create(&hostinfo);
MPI_Info_set(hostinfo, "host", "compute-0-1,compute-0-6,compute-0-8" );

strcpy(worker_program,"./CHILD_EX4.exe");

printf("Giving birth to child %d processes & running %s\n", universe_size-1, worker_program);

MPI_Comm_spawn(worker_program, MPI_ARGV_NULL,universe_size-1,hostinfo,0,MPI_COMM_SELF,&family_comm, MPI_ERRCODES_IGNORE);
MPI_Intercomm_merge(family_comm,1, &allcomm);
MPI_Comm_rank(allcomm, &globalrank);

printf("PARENT: family rank is %d,rank is %d \n",globalrank,rank);

MPI_Allreduce(&globalrank,&sumrank,1, MPI_INT,MPI_SUM,allcomm); //functia allreduce ca la procesul copil
printf("sumrank after allreduce function on process %d is %d \n", rank,sumrank);

MPI_Comm_disconnect(&family_comm);
MPI_Finalize();
return 0;
}

