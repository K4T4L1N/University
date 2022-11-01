#include <iostream>
using namespace std;

struct Edge {
    int x, y;
    Edge(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

int n; // numărul de noduri
bool ad[NMAX][NMAX]; // matricea de adiacență

bool find(Edge edge) {
    return ad[edge.x][edge.y];
}

void remove(Edge edge) {
    ad[edge.x][edge.y] = ad[edge.y][edge.x] = false;
}

void insert(Edge edge) {
    ad[edge.x][edge.y] = ad[edge.y][edge.x] = true;
}

void neighbors(int node) {
    for (int j = 1; j <= n; j++)
        if (ad[node][j])
            cout << j << ' ';
    cout << '\n';
}

int main() {
    n = 5;
    insert(Edge(1, 2));
    insert(Edge(1, 3));
    insert(Edge(1, 4));
    insert(Edge(4, 5));

    insert(Edge(3, 4));
    remove(Edge(3, 4));

    cout << find(Edge(4, 5)) << '\n';
    cout << find(Edge(2, 5)) << '\n';

    neighbors(1);
    neighbors(2);
    neighbors(3);
    neighbors(4);
    neighbors(5);
    return 0;
