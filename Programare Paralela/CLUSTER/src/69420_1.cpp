#include <iostream>
#include <cstdlib>
#include <climits>
#include <iomanip>
#include <mpi.h>
#include <cmath>

using namespace std;

int rank, processes_amount;

const int COMPOSE_MANUALLY = 1;
const int ROOT_RANK = 0;

//afisarea + formarea matricii manual (operatia de input a datelor)
void compose_matrix_manually(int*& matrix, int rows, int cols) {
	matrix = new int[cols * rows];
	for (int i = 0; i < rows; i++) {
		matrix[i * cols] = i;
		for (int j = 1; j < cols; j++) {
			cout << "\tmatrix[" << i << "][" << j << "] = ";
			cin >> matrix[i * cols + j];
		}
	}
}

//compunerea matricii
void compose_matrix(int*& matrix, int rows, int cols) {
	matrix = new int[cols * rows];
	for (int i = 0; i < rows; i++) {
		matrix[i * cols] = i;                        //bind row number (ca procesele sa stie cu ce rand lucreaza etc.)
		for (int j = 1; j < cols; j++) {
			matrix[i * cols + j] = (rand() % 16 - 5);
		}
	}
}

//afisarea matricii
void show_matrix(int*& matrix, int rows, int cols) {
	cout << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 1; j < cols; j++) {
			cout << setw(5) << matrix[i * cols + j];
		}
		cout << endl;
	}
}

//algoritmul de creare a inversei matricii date
void make_inverse(int*& matrix, int rows, int cols, int*& inverse_matrix) {
	inverse_matrix = new int[(rows + 1) * (cols - 1)];
	for (int j = 1; j < cols; j++) {
		inverse_matrix[(j - 1) * (1 + rows)] = j - 1;
		for (int i = 0; i < rows; i++) {
			inverse_matrix[(j - 1) * (1 + rows) + i + 1] = matrix[i * cols + j];
		}
	}
}

//concatinarea indicelor maxime
void concatenate_max_indexes_vectors(void* v_in, void* vectors, int* len, MPI_Datatype* dptr) {
	int* in = (int*)v_in;
	int* vc = (int*)vectors;

	for (int i = 0; i < *len; i++) {
		if (in[i] != -1)
			vc[i] = in[i];
	}
}

//algoritmul gasirea valorii maxime intr-un rand
void find_max_in_line(int* v_in, int* v_mins, int len, int rows, int cols) {
	int* in = v_in;
	int* row_max_indexes = new int[cols + 1];
	int size = 1;
	int max = in[1];
	row_max_indexes[0] = in[0]; //indexul randului

	for (int i = 2; i < len; i++) {
		if (max == in[i]) size++;
		else if (max < in[i]) {
			max = in[i];
			size = 1;
		}
	}
	
	row_max_indexes[1] = size;
	for (int i = 1, j = 2; i < len; i++) {
		if (max == in[i]) {
			row_max_indexes[j] = i;
			j++;
		}
	}

	for (int i = 0; i < in[0]; i++) {
		for (int j = 0; j < cols - 1; j++) {
			v_mins[i * (cols - 1) + j] = -1;
		}
	}
	for (int i = in[0]; i == in[0]; i++) {
		for (int j = 0; j < cols - 1; j++) {
			if (j < row_max_indexes[1]) {
				v_mins[i * (cols - 1) + j] = row_max_indexes[j + 2];
			}
			else {
				v_mins[i * (cols - 1) + j] = -1;
			}
		}
	}

	for (int i = in[0] + 1; i < rows; i++) {
		for (int j = 0; j < cols - 1; j++) {
			v_mins[i * (cols - 1) + j] = -1;
		}
	}

	delete[]row_max_indexes; //curatarea buferului
}


//gasirea de maximum cu ajutorul operatiilor MPI
void find_max(int*& matrix, int rows, int cols, int*& max_indexes) {
	MPI_Op MPI_CONCAT;
	MPI_Op_create(concatenate_max_indexes_vectors, true, &MPI_CONCAT);

	int rows_counter = 0, stop = INT_MAX;
	int* line = new int[cols];
	int indexes_amount = rows * (cols - 1);
	int* out = new int[indexes_amount];
	max_indexes = new int[indexes_amount];
	for (int i = 0; i < indexes_amount; i++) {
		max_indexes[i] = -1;
	}
	//variabile de impartire a valorilor matricilor pentru procesele MPI
	int* amount_to_take_from_matrix = new int[processes_amount];
	int* from_which_index_to_take = new int[processes_amount];

	while (rows_counter < rows) {
		for (int i = 0; i < processes_amount; i++) {
			if (rows_counter < rows) {
				amount_to_take_from_matrix[i] = cols;
				from_which_index_to_take[i] = cols * rows_counter;
				rows_counter++;
			}
			else {
				stop = i;
				for (int j = i; j < processes_amount; j++) {
					from_which_index_to_take[j] = 0;
					amount_to_take_from_matrix[j] = cols;
				}
				break;
			}
		}
	}
	//impartim proceselor operatiuni
	MPI_Scatterv(matrix, amount_to_take_from_matrix, from_which_index_to_take, MPI_INT, line, cols, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank < stop) {
		find_max_in_line(line, out, cols, rows, cols);
	}
	else {
		for (int i = 0; i < indexes_amount; i++)
			out[i] = -1;
	}
	MPI_Reduce(out, max_indexes, rows * (cols - 1), MPI_INT, MPI_CONCAT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	delete[]amount_to_take_from_matrix;
	delete[]from_which_index_to_take;
	delete[]out;
	delete[]line;
}


int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &processes_amount);

	srand(time(NULL));
	int rows, cols;

	if (rank == ROOT_RANK) {
		cout << "\nEnter rows amount: ";
		cin >> rows;
		cout << "Enter columns amount:  ";
		cin >> cols;
		cols++; //increment to store row indexes
	}

	MPI_Bcast(&rows, 1, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);
	MPI_Bcast(&cols, 1, MPI_INT, ROOT_RANK, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	int* matrix_A;
	int* matrix_B;
	int* inverse_A;
	
	if (rank == ROOT_RANK) {
		if (COMPOSE_MANUALLY) {
			cout << "\nMATRIX A\n";
			compose_matrix_manually(matrix_A, rows, cols);

			cout << "\nMATRIX B\n";
			compose_matrix_manually(matrix_B, rows, cols);
		}
		else {
			compose_matrix(matrix_A, rows, cols);
			compose_matrix(matrix_B, rows, cols);
		}

		cout << "\nMATRIX A";
		show_matrix(matrix_A, rows, cols);
		cout << "\nMATRIX B";
		show_matrix(matrix_B, rows, cols);

		make_inverse(matrix_A, rows, cols, inverse_A);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	int cols_inversed = rows + 1, rows_inversed = cols - 1;
	int* max_indexes_A, * max_indexes_B;

	find_max(matrix_B, rows, cols, max_indexes_B);
	find_max(inverse_A, rows_inversed, cols_inversed, max_indexes_A);

	if (rank == ROOT_RANK) {
		cout << "valori de echilibru (intersectii) : ";
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols - 1 and max_indexes_B[i * (cols - 1) + j] != -1; j++) {
				int max_index_in_row = max_indexes_B[i * (cols - 1) + j] - 1;
				for (int i_inversed = 0; i_inversed < cols_inversed - 1 and max_indexes_A[max_index_in_row * (cols_inversed - 1) + i_inversed] != -1; i_inversed++) {
					if (i == max_indexes_A[max_index_in_row * (cols_inversed - 1) + i_inversed] - 1) {
						printf("( %i, %i ) ", i, max_index_in_row);
					}
				}
			}
		}
		cout << endl;
		delete[]max_indexes_A;
		delete[]max_indexes_B;
		delete[]matrix_B;
		delete[]matrix_A;
		delete[]inverse_A;
	}

	MPI_Finalize();
	return 0;
}
