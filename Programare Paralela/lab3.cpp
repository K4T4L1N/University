#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int ROOT_RANK = 0;   // rangul procesului root - 0
const int TAG = 111;       // tag pentru MPI_Send, MPI_Recv
const int N_DIMS = 2;      // dimensiunea matricii procesului dat

// dimensiunea matricei initiale va fi definita prin valorile rows si cols
// dimensiunea unui bloc matricial va fi definit prin valorile block_rows si block_cols
// nr total de blocuri in care matricea initiala va fi divizata se va defini cu valorile total_block_rows, total_block_cols
int rows,cols,block_rows,block_cols,total_block_rows,total_block_cols;

// descriem tipul MPI pentru divizarea unei submatrici cu ajutorul BBlockInfo
struct BBlockInfo {
	MPI_Datatype MPI_B; // Tipul MPI care descrie blocul matricial
	int B_rows;         // numarul de linii din blocul divizat
	int B_cols;         // numarul de coloane din blocul divizat
};


// functie ce creeaza un tip care descrie partitia matricei originale pentru un bloc specific al matricei de partitie
// [IN VALUE] blck_i	- indexul de rand de matrice de partitie
// [IN VALUE] blck_j	- indexul coloanei matricei partitiei
// [OUT VALUE] block	- variabila care descrie divizarea

void createSubBType(int blck_i, int blck_j, BBlockInfo *block) {

	// start_row, start_col - indicii de pornire ai blocului in matricea initiala
	int start_row = blck_i * block_rows,
        start_col =  blck_j * block_cols;

	// size_row, size_row - valorile ce determina dimensiunea blocului care va fi extras
	int size_row = block_rows,
        size_col = block_cols;

	// row_end, col_end - indicii de la sfarsitul blocului in matricea initiala
	int row_end = start_row + size_row,
        col_end = start_col + size_col;

	// daca blocul dat, tinand cont de indici initiali, este mai mare decat dimensiunea matricei originale => stabilim
	//dimensiunea maxima admisibila pentru acel blocbloc (de la indexul de start pana la sfarsitul dimensiunii matricei originale)
	if (row_end > rows) size_row = rows - start_row;

	if (col_end > cols)size_col = cols - start_col;

	int starts[N_DIMS] = { start_row, start_col };
	int bigsizes[N_DIMS] = { rows, cols };
	int subsizes[N_DIMS] = { size_row, size_col };

	// descriem blocul ca submatrice (submatrix or subarray)
	MPI_Datatype b_type;
	MPI_Type_create_subarray(N_DIMS, bigsizes, subsizes, starts, MPI_ORDER_C, MPI_INT, &b_type);
	MPI_Type_commit(&b_type);
	block->MPI_B = b_type;
	block->B_rows = size_row;
	block->B_cols = size_col;
}

int main(int argc, char **argv) {
	srand(time(NULL));

	int rank, size, grila_rank;
	int *grila_ranks;
	int *matrix;

	MPI_Group MPI_GROUP_WORLD, grila_group;
	MPI_Comm grila_group_com, grila_com;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int grila_group_size = size - (size % N_DIMS);              // dimensiunea grupului matricei de procese, adica numarul de procese incluse in acest grup
	int grila_cols = grila_group_size / N_DIMS;                 // numarul de coloane din matricea procesului
	grila_ranks = (int*)malloc(grila_group_size * sizeof(int)); // alocam memorie pentru a determina gradele de procese din matricea procesului

	if (rank == ROOT_RANK) {
		// introducem dimensiunea matricei
		printf("Enter number of rows:\n");
		scanf("%d", &rows);
		printf("Enter number of cols:\n");
		scanf("%d", &cols);

		// introducem dimensiunea blocurilor in care va fi divizata matricea initiala
		printf("Enter number of block rows:\n");
		scanf("%d", &block_rows);
		printf("Enter number of block cols:\n");
		scanf("%d", &block_cols);

		// algoritm de initializare a matricii initiale
		int total = rows * cols;
		matrix = (int*)malloc(total * sizeof(int));
		for (int i = 0; i < total; i++) {
			matrix[i] = i;
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++)
				printf("%3d", matrix[i * cols + j]);
			printf("\n");
		}

		// definim randurile de procese care vor fi incluse in grupul ce formeaza matricea procesului
		printf("Grila size = %d\n", grila_group_size);
		for (int i = 0; i < grila_group_size; i++) {
			grila_ranks[i] = i;
		}
	}
    sleep(1);
    MPI_Barrier(MPI_COMM_WORLD);

	// trecem toate procesele de date introduse si rangurile de proces pentru grupul de matrice de proces
	// utilizand functia de broadcast (MPI_Bcast)
	MPI_Bcast(&rows, 1, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);
	MPI_Bcast(&cols, 1, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);
	MPI_Bcast(&block_rows, 1, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);
	MPI_Bcast(&block_cols, 1, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);
	MPI_Bcast(grila_ranks, grila_group_size, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);

	// determinam numarul total de randuri si coloane din matricea partitiei
	total_block_rows = rows / block_rows + (rows % block_rows == 0 ? 0 : 1);
	total_block_cols = cols / block_cols + (cols % block_cols == 0 ? 0 : 1);

	// cream un grup format din procese care vor forma o matrice de procese
    MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);
	MPI_Group_incl(MPI_GROUP_WORLD, grila_group_size, grila_ranks, &grila_group);
	// cream un comunicator pentru grupul creat anterior
	MPI_Comm_create(MPI_COMM_WORLD, grila_group, &grila_group_com);
	// definim rangul procesului grupului
    MPI_Group_rank(grila_group, &grila_rank);

	// daca procesul apartine acestui grup rezulta ca:
    if (grila_rank != MPI_UNDEFINED) {

		int dim_size[N_DIMS] = { N_DIMS, grila_group_size / N_DIMS };
		int periods[N_DIMS] = { 1, 0 };
		int reorder = 1;
		int coords[N_DIMS];

		// cream un nou comunicator
		MPI_Cart_create(grila_group_com, N_DIMS, dim_size, periods, reorder, &grila_com);

		// definim coordonatele procesului
		MPI_Cart_coords(grila_com, rank, N_DIMS, coords);

		if (rank == ROOT_RANK) {
			BBlockInfo b_type;
			int grila_coords[N_DIMS] = { 0, 0 };
			int calculated_rank;
			// rulam prin intreaga matrice de partitie
			// ** nu se creeaza nicio matrice de partitie, dar cunoscand dimensiunea sa => putem defini un bloc specific in orice pozitie
			for (int i = 0; i < total_block_rows; i++) {
				for (int j = 0; j < total_block_cols; j++) {
					createSubBType(i, j, &b_type); // definim blocul prin coordonatele i, j in matricea partitiei

					// calcula coordonatele procesului din matricea procesului, la care ar trebui trecut blocul
					grila_coords[0] = i % N_DIMS;
					grila_coords[1] = j % grila_cols;
					// gasim rangul procesului dupa coordonatele sale
					MPI_Cart_rank(grila_com, grila_coords, &calculated_rank);

					// trimitem blocul la proces din matricea procesului
					MPI_Send(&matrix[0], 1, b_type.MPI_B, calculated_rank, TAG, grila_com);
					MPI_Type_free(&b_type.MPI_B);
				}
			}
		}

		// parcurgem pe coloana (pe orizontala) si pe rand (pe verticala) pentru a determina urmatorul bloc
		// pe care acest proces ar trebui sa il primeasca
		int horizontal_step = grila_cols, vertical_step = N_DIMS;

		BBlockInfo b_type;
		int *buf;
		int total_received = 0;
	        sleep(rank);
        	MPI_Barrier(MPI_COMM_WORLD);

		// parcurgem blocurile necesare din matricea partitiei
		// ** cele care trebuie sa fie primite de aceste procese
		for (int current_row = coords[0]; current_row < total_block_rows; current_row += vertical_step) {
			for (int current_col = coords[1]; current_col < total_block_cols; current_col += horizontal_step) {
                // definim dimensiunea blocului de partitionare la pozitia curenta a matricei de partitionare
				createSubBType(current_row, current_col, &b_type);
                // alocam memorie pentru bufer
				buf = (int*)malloc(sizeof(int) * b_type.B_rows * b_type.B_cols);
				// obtinem un bloc dintr-un proces cu un rang ROOT
				MPI_Recv(buf, b_type.B_rows * b_type.B_cols, MPI_INT, ROOT_RANK, TAG, grila_com, MPI_STATUS_IGNORE);
				total_received++;

				// afiseaza blocul obtinut
				printf("Process %d (%d,%d) received:\n", grila_rank, coords[0], coords[1]);
				for (int _i = 0; _i < b_type.B_rows; _i++) {
					for (int _j = 0; _j < b_type.B_cols; _j++) {
						printf("%3d", buf[_i * b_type.B_cols + _j]);
					}
					printf("\n");
				}

				MPI_Type_free(&b_type.MPI_B);
				free(buf);
			}
		}
        MPI_Barrier(MPI_COMM_WORLD);
		printf("Process %d received total %d blocks\n", grila_rank, total_received);
	}
	else {
		printf("Process %d is idle\n", rank);
	}

	MPI_Finalize();
	return 0;
}

