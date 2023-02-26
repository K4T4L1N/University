#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

using namespace std;

// definim fata cubului
enum SIDE {
	X_FIRST, 	// x = 0
	X_LAST, 	// x = dimensiunea cubului
	Y_FIRST, 	// y = 0
	Y_LAST, 	// y = dimensiunea cubului
	Z_FIRST,	// z = 0
	Z_LAST		// z = dimensiunea cubului
};

const SIDE FIXED_SIDE = X_LAST;   // partea fixa ce va fi ​​selectata
const int ROOT_RANK = 0;          // rangul procesului root
const int TAG = 111;              // tag pentru MPI_Send, MPI_Recv
const int N_DIMS = 3;             // numarul de dimensiuni ale cubului
const int RELATIVE_TO_AXIS = 0;   // directia in sensul acelor ceasornicului este calculata pentru toate axele, sau in mod specific pentru fiecare parte
                                  //(0 - pentru fiecare fata, 1 - pentru axa)


// definim coordonatele 3D
struct Coords3D {
	Coords3D() { }
	Coords3D(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	int x;
	int y;
	int z;
};


// definim coordonatele 2D
struct Coords2D {
	Coords2D() { }
	Coords2D(int x, int y) {
		this->x = x;
		this->y = y;
	}

	int x;
	int y;

	bool operator==(Coords2D rhs) {
		return this->x == rhs.x && this->y == rhs.y;
	}
};


// definim coordonatele punctului de pe fata pentru proces
struct SidePoint {
	SidePoint() { }
	SidePoint(int rank, Coords3D source, Coords2D translated) {
		this->rank = rank;
		this->source = source;
		this->translated = translated;
	}

	int rank;            // rangul procesului
	Coords3D source;     // coordonate 3D
	Coords2D translated; // coordonatele originale traduse in 2D, in functie de partea fixata
};


// determinam daca poate fi creat un cub cu un numar dat de procese.
// returneaza adevarat daca este posibil sa cream un cub cu numarul dat de procese, in caz contrar, fals
// [IN] numtasks		    - numarul de procese
// [OUT] side_length		- dimensiunea unei fete de cub pentru un numar dat de procese
bool canCreateCube(int numtasks, int *side_length) {
	// numarul de procese trebuie sa fie de 3 grade fata de numarul care determina dimensiunea cubului
	// ex, daca side_length=3 => numtasks=27, side_length=4 => numtasks=64 etc.
	int curoot = round(pow(numtasks, 1.0/3.0));
	*side_length = curoot;
	return curoot * curoot * curoot == numtasks;
}

// verifica daca punctul se afla pe marginea cubului
// [IN] points		    - punct de verificat
// [IN] side		    - muchie fixa
// [IN] side_length 	- dimensiunea cubului
bool isPointOnSide(Coords3D &point, SIDE side, int side_length) {
	int last_coord = side_length - 1;
	switch (side) {
		case X_FIRST:
			return point.x == 0 && (point.y == 0 || point.y == last_coord || point.z == 0 || point.z == last_coord);
		case X_LAST:
			return point.x == last_coord && (point.y == 0 || point.y == last_coord || point.z == 0 || point.z == last_coord);
		case Y_FIRST:
			return point.y == 0 && (point.x == 0 || point.x == last_coord || point.z == 0 || point.z == last_coord);
		case Y_LAST:
			return point.y == last_coord && (point.x == 0 || point.x == last_coord || point.z == 0 || point.z == last_coord);
		case Z_FIRST:
			return point.z == 0 && (point.x == 0 || point.x == last_coord || point.z == 0 || point.z == last_coord);
		case Z_LAST:
			return point.z == last_coord && (point.x == 0 || point.x == last_coord || point.y == 0 || point.y == last_coord);
		default:
			return false;
	}
}

// converteste coordonatele din 3d in 2d, in functie de fata
// [IN] side		- muchia
// [IN] point3d		- punct de start
// [OUT] point2d	- coordonate traduse
void translateTo2D(SIDE side, Coords3D &point3d, Coords2D *point2d) {
	switch (side) {
		case X_FIRST:
		case X_LAST:
			// daca fata x este fixa, atunci se preiau coordonatele y si z
			point2d->x = point3d.y;
			point2d->y = point3d.z;
			break;
		case Y_FIRST:
		case Y_LAST:
			// daca fata y este fixa, atunci se preiau coordonatele x si z
			point2d->x = point3d.x;
			point2d->y = point3d.z;
			break;
		case Z_FIRST:
		case Z_LAST:
			// daca fata z este fixa, atunci se preiau coordonatele x si y
			point2d->x = point3d.x;
			point2d->y = point3d.y;
			break;
	}
}

// cauta puncte cu coordonatele bidimensionale date si returneaza un pointer la elementul gasit
// [IN] data		- matrice cu datele initiale
// [IN] ranks_count	- nr de elemente din matricea data
// [IN] coord		- Coordonate 2D
SidePoint *findBy2dCoord(SidePoint *data, int ranks_count, Coords2D coord) {
	for (int i = 0; i < ranks_count; i++) {
		if (data[i].translated == coord)
			return &data[i];
	}
	return NULL;
}

// defineste randurile de procese de pe marginea cubului
// Converteste o matrice de ranguri sortate in sensul acelor de ceasornic incepand cu punctul (0,0) din fata 2D
// [IN] side			    - muchia
// [IN] side_length	    	- dimensiunea cubului
// [OUT] ranks_count		- nr de procese de pe margine
int *getRanksOnSide(SIDE side, int side_length, int *ranks_count) {
	int last_coord = side_length - 1;
	*ranks_count = 4 * last_coord;
	int *ranks = (int*)malloc(*ranks_count * sizeof(int));
	SidePoint *data = (SidePoint*)malloc(*ranks_count * sizeof(SidePoint));
	int current_rank = 0, i = 0;
	Coords3D coord;
	Coords2D coord_translated;

	// trece prin toate punctele cubului dat
	for (int x = 0; x < side_length; x++) {
		coord.x = x;
		for (int y = 0; y < side_length; y++) {
			coord.y = y;
			for (int z = 0; z < side_length; z++, current_rank++) {
				coord.z = z;
				if (!isPointOnSide(coord, side, side_length))
					continue;

				// daca punctul cutare(ex. x, y, z) apartine fetei date, traducem coordonatele sale in 2d
				translateTo2D(side, coord, &coord_translated);
				data[i++] = SidePoint(current_rank, coord, coord_translated);
			}
		}
	}

	// ordoneaza randurile in sensul acelor ceasornicului
	i = 0;
	Coords2D p(0,0);
	SidePoint *ordered;

	do {
		ordered = findBy2dCoord(data, *ranks_count, p);
		ordered != NULL && (ranks[i++] = ordered->rank);
		p.x++;
	} while (p.x < last_coord);

	do {
		ordered = findBy2dCoord(data, *ranks_count, p);
		ordered != NULL && (ranks[i++] = ordered->rank);
		p.y++;
	} while (p.y < last_coord);

	do {
		ordered = findBy2dCoord(data, *ranks_count, p);
		ordered != NULL && (ranks[i++] = ordered->rank);
		p.x--;
	} while (p.x > 0);

	do {
		ordered = findBy2dCoord(data, *ranks_count, p);
		ordered != NULL && (ranks[i++] = ordered->rank);
		p.y--;
	} while (p.y > 0);

	return ranks;
}

int main(int argc, char **argv) {
	int rank, size, side_length, side_rank;
	Coords3D coords;

	MPI_Comm comm, side_comm, side_ring_comm;
	MPI_Group MPI_GROUP_WORLD, side_group;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// verificam daca este posibil sa cream un cub din numarul curent de procese
	bool isCube = canCreateCube(size, &side_length);
	if (!isCube) {
		if (rank == ROOT_RANK)
			printf("Can not create cube from %d processes.\n", size);

		MPI_Finalize();
		exit(1);
	}

	int dims[N_DIMS] = { side_length, side_length, side_length };
	int periods[N_DIMS] = { 0, 0, 0 };
	int _coords[N_DIMS];
	// cream o topologie pentru cub
	MPI_Cart_create(MPI_COMM_WORLD, N_DIMS, dims, periods, 0, &comm);
	// obtinem coordonatele procesului in topologia cubului
	MPI_Cart_coords(comm, rank, N_DIMS, _coords);
	coords.x = _coords[0]; coords.y = _coords[1]; coords.z = _coords[2];

	// obtinem randurile de procese situate pe marginile unei fete fixe
	int ranks_on_side_count;
	int *ranks_on_side = getRanksOnSide(FIXED_SIDE, side_length, &ranks_on_side_count);

	// cream un grup pentru MPI_COMM_WORLD pentru a crea un nou grup de procese pe partea unei fete
	MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);
	// cream un grup de procese pe partea unei fete pentru a crea o topologie de cerc
	MPI_Group_incl(MPI_GROUP_WORLD, ranks_on_side_count, ranks_on_side, &side_group);
	// cream un comunicator pentru acest grup
	MPI_Comm_create(MPI_COMM_WORLD, side_group, &side_comm);
	// obtinem rangul procesului in grupul creat
	MPI_Group_rank(side_group, &side_rank);

	if (side_rank != MPI_UNDEFINED) {
		// daca procesul apartine unui grup (adica este pe o margine fixa)

		// cream o topologie carteziana de tip cerc
		int side_comm_size, source, destination;
		int side_card_persiods[1] = { 1 };
		MPI_Comm_size(side_comm, &side_comm_size);
		MPI_Cart_create(side_comm, 1, &side_comm_size, side_card_persiods, 0, &side_ring_comm);

		int displ = -1;
		if (!RELATIVE_TO_AXIS && (FIXED_SIDE == X_FIRST || FIXED_SIDE == Y_FIRST || FIXED_SIDE == Z_FIRST))
			displ = 1;
		// defineste vecinii
		MPI_Cart_shift(side_ring_comm, 1, displ, &source, &destination);

		// trimite date intr-un cerc
		const int BUF_SIZE = N_DIMS + 2;
		int sendbuf[BUF_SIZE] = { rank, side_rank, coords.x, coords.y, coords.z };
		int recvbuf[BUF_SIZE];
		MPI_Send(sendbuf, BUF_SIZE, MPI_INT, destination, TAG, side_ring_comm);
		MPI_Recv(recvbuf, BUF_SIZE, MPI_INT, source, TAG, side_ring_comm, MPI_STATUS_IGNORE);

		printf("Process %3d (%d,%d,%d) -> %3d (%d,%d,%d)\n", sendbuf[0], sendbuf[2], sendbuf[3], sendbuf[4], recvbuf[0], recvbuf[2], recvbuf[3], recvbuf[4]);

		MPI_Comm_free(&side_ring_comm);
	}

	// MPI_Group_free(&side_group);
	MPI_Finalize();
	return 0;
}

