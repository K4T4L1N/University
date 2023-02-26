#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char* argv[])
{
    int k, p, size, rank;
    int rankul_gr_cet, rankul_gr_necet;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;
	MPI_Comm com_cet, com_necet;
    MPI_Group MPI_GROUP_WORLD, cet, necet;
    int *ranks, *matrix;
    int namelen;

    	MPI_Init(&argc, &argv);
    			MPI_Comm_size(MPI_COMM_WORLD, &size);
    				MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    					MPI_Get_processor_name(processor_name, &namelen);

	int m_in[size], m_out[size];
	struct 
	{
		double val;
		int rank;
	} in[size], out_max[size], out_min[size];

    srand(time(0));
    k = size / 2;
    ranks = (int*)malloc(k * sizeof(int));

    if (rank == 0) {
        printf("\nРЕЗУЛЬТАТ ПРОГРАММЫ '%s' \n", argv[0]);
		matrix = (int*)malloc(size * size * sizeof(int));
			for (int i = 0; i < size * size; i++)
				matrix[i] = rand() % 20 + 1;

			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++)
					printf("%5d", matrix[i * size + j]);
						printf("\n");
		}
		printf("\n");
		
        int j = 0;
        for (int i = 0; i < size; i++) {
			if (i % 2 == 0)
				ranks[j++] = i;
        }
    	MPI_Barrier(MPI_COMM_WORLD);
	}
	else 
		
    	MPI_Barrier(MPI_COMM_WORLD);

    		MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);
   				MPI_Bcast(ranks, k, MPI_INT, 0, MPI_COMM_WORLD);
   					MPI_Group_incl(MPI_GROUP_WORLD, k, ranks, &cet);
    					MPI_Group_rank(cet, &rankul_gr_cet);
    						MPI_Group_excl(MPI_GROUP_WORLD, k, ranks, &necet);
    							MPI_Group_rank(necet, &rankul_gr_necet);
									MPI_Comm_create(MPI_COMM_WORLD, cet, &com_cet);
										MPI_Comm_create(MPI_COMM_WORLD, necet, &com_necet);
											MPI_Scatter(matrix, size, MPI_INT, m_in, size, MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 0; i < size; ++i) 
	{
		in[i].val = m_in[i];
		in[i].rank = rank;
	}
    if (rankul_gr_cet != MPI_UNDEFINED) {
		MPI_Reduce(in, out_max, size, MPI_DOUBLE_INT, MPI_MAXLOC, 0, com_cet);
			MPI_Barrier(com_cet);
	}


	if (rank == 0) {
		for (int i = 0; i < size; ++i) {
			int max = out_max[i].val;
			int max_line = out_max[i].rank;
			int min = 0; 
			int min_line = 0; 
			printf("Столбец %d, max = %d, max линия = %d, min = %d, min линия = %d\n", i, max, max_line, min, min_line); 
		}
	}

    MPI_Group_free(&cet);
    	MPI_Group_free(&necet);
    		MPI_Finalize();
    return 0;
}
