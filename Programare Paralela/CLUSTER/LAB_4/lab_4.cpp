#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

const int ROOT_RANK = 0;           // rangul procesului radacina
const int TAG_DEFAULT = 111;       // tag-ul pentru MPI_Send si MPI_Recv
const int TAG_MAX_VALUE = 222;     // tag-ul pentru MPI_Send si MPI_Recv, pentru a trimite maximul
const int N_DIMS = 2;              // dimensiunea matricei procesului cutare
char* FILE_NAME = "matrice.dat";   //numele de fisier

int rows, cols; // nr de randuri si coloane din matricea originala

// creeaza un nou tip care defineste distributia matricei pentru un proces specific
// [INPUT]  grila_size	    - dimensiunea matricei procesului cutare
// [INPUT]  grila_rank	    - rangul procesului cutare din matricea proceselor, pentru care ar trebui calculata partitia matricei
// [OUTPUT] MPI_Darray	    - tipul rezultat, descriind partitia matricei intr-un proces specific
void createDArray(int grila_size, int grila_rank, MPI_Datatype *MPI_Darray) {

	// functia MPI_Type_create_darray este utilizata pentru a distribui matricea
	// catre procesele care se afla in matricea bidimensionala a proceselor

	int array_of_gsizes[N_DIMS] = { rows, cols };
	int array_of_distribs[N_DIMS] = { MPI_DISTRIBUTE_CYCLIC, MPI_DISTRIBUTE_BLOCK };
	int array_of_dargs[N_DIMS] = { MPI_DISTRIBUTE_DFLT_DARG, MPI_DISTRIBUTE_DFLT_DARG };
	int array_of_psizes[N_DIMS] = { N_DIMS, grila_size / N_DIMS };
	MPI_Type_create_darray(grila_size, grila_rank, N_DIMS, array_of_gsizes, array_of_distribs, array_of_dargs, array_of_psizes, MPI_ORDER_C, MPI_INT, MPI_Darray);
	MPI_Type_commit(MPI_Darray);
}

int main(int argc, char **argv) {
	srand(time(NULL));

	int rank, size, grila_rank, read_rank;
	int *grila_ranks;
	int *matrix;

    MPI_File file_out;
	MPI_Group MPI_GROUP_WORLD, grila_group, read_group;
	MPI_Comm grila_group_com, grila_com, read_group_com, read_com;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// verifica existenta fisierului si il sterge, daca acesta exista
	MPI_File_open(MPI_COMM_WORLD, FILE_NAME, MPI_MODE_DELETE_ON_CLOSE, MPI_INFO_NULL, &file_out);
    MPI_File_close(&file_out);


	if (rank == ROOT_RANK) {
		printf("Enter number of rows:\n");
		scanf("%d", &rows);
		printf("Enter number of cols:\n");
		scanf("%d", &cols);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&rows, 1, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);
	MPI_Bcast(&cols, 1, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);

	// calculeaza numarul necesar de procese din matricea procesului
	int grila_group_size = ceil(cols / (float)N_DIMS) * N_DIMS;
	grila_ranks = (int*)malloc(grila_group_size * sizeof(int));
	int read_group_size = grila_group_size;

	if (rank == ROOT_RANK) {
		// defineste rangurile de procese necesare care vor fi incluse in matricea procesului
		printf("Grila size = %d\n", grila_group_size);
		for (int i = 0; i < grila_group_size; i++) {
			grila_ranks[i] = i;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(grila_ranks, grila_group_size, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);

	// creaza grupul care include procesele care vor fi incluse in matricea proceselor pentru scriere
    MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);
	MPI_Group_incl(MPI_GROUP_WORLD, grila_group_size, grila_ranks, &grila_group);
	MPI_Comm_create(MPI_COMM_WORLD, grila_group, &grila_group_com);

	// creaza grupul, care include procesele care vor fi incluse in matricea procesului pentru citire
	MPI_Group_incl(MPI_GROUP_WORLD, grila_group_size, grila_ranks, &read_group);
	MPI_Comm_create(MPI_COMM_WORLD, read_group, &read_group_com);

	// determina rangul procesului curent in fiecare grup
    MPI_Group_rank(grila_group, &grila_rank);
    MPI_Group_rank(read_group, &read_rank);

    if (grila_rank != MPI_UNDEFINED) {
		// daca procesul apartine primului grup =>
		int dim_size[N_DIMS] = { N_DIMS, grila_group_size / N_DIMS };
		int periods[N_DIMS] = { 1, 0 };
		int reorder = 1;
		int coords[N_DIMS];

		// => creaza o matrice de proces 2D, ...
		MPI_Cart_create(grila_group_com, N_DIMS, dim_size, periods, reorder, &grila_com);
		// ... defineste coordonatele procesului ...
		MPI_Cart_coords(grila_com, rank, N_DIMS, coords);

		// ... deschide fisierul pentru scriere, ...
		MPI_File_open(grila_com, FILE_NAME, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file_out);

		// defineste un tip care descrie toate submatricele care apartin acestui proces
		MPI_Datatype MPI_Darray;
		int darray_size;
		createDArray(grila_group_size, grila_rank, &MPI_Darray);
		MPI_Type_size(MPI_Darray, &darray_size);
		// numarul de elemente din toate submatricele pentru acest proces
		darray_size /= sizeof(int);
		int *buf = (int*)malloc(sizeof(int) * darray_size);
		// genereaza date
		printf("Process %d (%d,%d) generates next data: ", grila_rank, coords[0], coords[1]);
		for (int i = 0; i < darray_size; i++) {
			buf[i] = grila_rank + rand() % 50;
			printf("%3d", buf[i]);
		}
		printf("\n");
		// scrie datele generate intr-un fisier
		MPI_File_set_view(file_out, 0, MPI_INT, MPI_Darray, "native", MPI_INFO_NULL);
		MPI_File_write(file_out, buf, darray_size, MPI_INT, MPI_STATUS_IGNORE);

		// inchide fisierul
		MPI_File_close(&file_out);
	}

	if (read_rank != MPI_UNDEFINED) {
		// daca procesul apartine celui de-al doilea grup (grup de citire)
		int dim_size[N_DIMS] = { N_DIMS, read_group_size / N_DIMS };
		int periods[N_DIMS] = { 1, 0 };
		int reorder = 1;
		int coords[N_DIMS];

		// creaza o matrice de proces 2D
		MPI_Cart_create(read_group_com, N_DIMS, dim_size, periods, reorder, &read_com);
		// defineste coordonatele procesului
		MPI_Cart_coords(read_com, rank, N_DIMS, coords);

		// deschide fisierul pentru citire
		MPI_File_open(read_com, FILE_NAME, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file_out);

		// defineste un tip care descrie toate submatricele care apartin acestui proces
		MPI_Datatype MPI_Darray;
		int darray_size;
		createDArray(read_group_size, read_rank, &MPI_Darray);
		MPI_Type_size(MPI_Darray, &darray_size);
		darray_size /= sizeof(int);

		// citeste toate elementele tuturor submatricelor care apartin acestui proces
		int *buf = (int*)malloc(sizeof(int) * darray_size);
		MPI_File_set_view(file_out, 0, MPI_INT, MPI_Darray, "native", MPI_INFO_NULL);
		MPI_File_read(file_out, buf, darray_size, MPI_INT, MPI_STATUS_IGNORE);

		// defineste elementul maxim in submatrice
		int max_value = buf[0];
		for (int i = 1; i < darray_size; i++) {
			if (buf[i] > max_value)
				max_value = buf[i];
		}
		printf("Process %d (%d,%d) max value = %d\n", read_rank, coords[0], coords[1], max_value);

		// trimite submatricele citite la procesul radacina pentru a colecta intreaga matrice
		MPI_Send(buf, darray_size, MPI_INT, ROOT_RANK, TAG_DEFAULT, read_com);
		// trimite valoarea maxima a submatricelor la procesul cu radacina de rang
		MPI_Send(&max_value, 1, MPI_INT, ROOT_RANK, TAG_MAX_VALUE, read_com);

		if (read_rank == ROOT_RANK) {
			int total = rows * cols;
			matrix = (int*)malloc(total * sizeof(int));

			// toate elementele maxime primite din alte procese
			int *maxs = (int*)malloc(grila_group_size * sizeof(int));

			for (int i = 0; i < grila_group_size; i++) {
				int recv_rank = grila_ranks[i];
				// defineste un tip care descrie toate submatricele care apartin unui proces cu rang recv_rank
				MPI_Datatype MPI_Darray_sub;
				createDArray(read_group_size, recv_rank, &MPI_Darray_sub);
				// obtine toate submatricele din proces cu rang recv_rank
				MPI_Recv(matrix, 1, MPI_Darray_sub, recv_rank, TAG_DEFAULT, read_com, MPI_STATUS_IGNORE);
				// obtine valoarea maxima din procesul cu rangul recv_rank
				MPI_Recv(maxs + recv_rank, 1, MPI_INT, recv_rank, TAG_MAX_VALUE, read_com, MPI_STATUS_IGNORE);
			}

			// gaseste valoarea maxima in matrice
			int max_value = maxs[0];
			for (int i = 1; i < grila_group_size; i++) {
				if (maxs[i] > max_value)
					max_value = maxs[i];
			}

			// tipareste matricea si valoarea maxima
			printf("== MATRIX (max value = %d) ==\n", max_value);
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++)
					printf("%3d", matrix[i * cols + j]);
				printf("\n");
			}
		}

		// inchide fisierul
		MPI_File_close(&file_out);
	}

	if (read_rank == MPI_UNDEFINED && grila_rank == MPI_UNDEFINED)
		printf("Process %d is idle\n", rank);

	MPI_Finalize();
	return 0;
}

