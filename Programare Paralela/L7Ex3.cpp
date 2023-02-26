#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char* argv[])
{
    int i, k, p, size, rank, size_new, rank_new, sr, des;
    int rank_gr;
    char process_name[MPI_MAX_PROCESS_NAME];

    MPI_Status status;
    MPI_Comm comm_new, circle1;
    MPI_Group MPI_GROUP_WORLD, newgrupp;

    int* ranks;
    int namelen;
    int D1 = 123, D2;
    int dims[1], per[1], reor;

        MPI_Init(&argc, &argv);
            MPI_Comm_size(MPI_COMM_WORLD, &size);
                MPI_Comm_rank(MPI_COMM_WORLD, &rank);
                    MPI_Get_process_name(process_name, &namelen);

            if (rank == 0)
        printf("\nРЕЗУЛЬТАТ ПРОГРАММЫ '%s' \n", argv[0]);
        MPI_Barrier(MPI_COMM_WORLD);
        srand(time(0));

    k = size / 2;
    ranks = (int*)malloc(k * sizeof(int));
        if (rank == 0) {
            int rN = 0;
            int repeat;
                for (i = 0; i < k; i++) {
                    do {
                        repeat = 0;
                        rN = rand() % size;
                        for (int j = 0; j < i; ++j) {
                             if (rN == ranks[j]) {
                                    repeat = 1;
                                        break;
                        }
                    }
             } while (repeat == 1);

            ranks[i] = rN;
         }

        printf("Были взяты наугад %d числа следующим образом:\n", k);
        for (i = 0; i < k; i++)

            printf(" %d ", ranks[i]);
        printf(" \n");
        MPI_Barrier(MPI_COMM_WORLD);
    }
    else

        MPI_Barrier(MPI_COMM_WORLD);

    MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);
        MPI_Bcast(ranks, k, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Group_incl(MPI_GROUP_WORLD, k, ranks, &newgrupp);
                MPI_Group_rank(newgrupp, &rank_gr);
                    MPI_Comm_create(MPI_COMM_WORLD, newgrupp, &comm_new);

        if (rank_gr != MPI_UNDEFINED) {

		  MPI_Comm_size(comm_new, &size_new);
            MPI_Comm_rank(comm_new, &rank_new);

        dims[0] = size_new;
        per[0] = 1;
        reor = 1;
      
            MPI_Barrier(comm_new);
                MPI_Cart_create(comm_new, 1, dims, per, reor, &circle1);
                    MPI_Cart_shift(circle, 1, 2, &sr, &des);

        D1 = D1 + rank;
        MPI_Sendrecv(&D1, 1, MPI_INT, des, 12, &D2, 1, MPI_INT, sr, 12, circle1, &status);

        MPI_Barrier(comm_new);

        if (rank_new == 0) {

       		printf("Результат MPI_Sendrecv:\n");
            MPI_Barrier(comm_new);
        }
        else MPI_Barrier(comm_new);

		  printf ("Процесс %d (%d из %s), получает из процесса %d значение %d и отправялет процессу %d значение %d\n",rank_new,rank,process_name,sr,D2,des,D1);
            MPI_Barrier(comm_new);
                MPI_Group_free(&newgrupp);
                    MPI_Comm_free(&circle1);
                        MPI_Comm_free(&comm_new);
	}
    MPI_Finalize();
    return 0;
}
