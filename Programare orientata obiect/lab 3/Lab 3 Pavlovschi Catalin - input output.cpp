//Creaţi clasa matricelor care conţine un pointer la date de tip întreg, numărul de linii, numărul de coloane şi variabila erorii. Definiţi constructorul
//implicit, constructorul cu un parametru şi cu doi parametri, constructorul de copiere şi destructorul. Supraîncărcaţi operaţiile =, +, -, +=, -=, *, *=
//cu obiecte ale acestei clase şi cu numere întregi. Supraîncărcaţi operaţiile de intrare/ieşire de flux (<< şi >>).
//Să se definească operaţiile ca operatori ai clasei iar una din ele ca funcţie prietenă.

#include<iostream>

using namespace std;

class Array
{
     private: int *p;
     public:
          int length;
          Array(int size = 0): length(size)
          {
                p=new int(length);
          }
          int& operator [](const int k)
          {
               return p[k];
          }
};
class Matrix
{
      private: Array *p;
      public:
            int r,c;
            Matrix(int i=0, int j=0):r(i), c(j)
            {
                 p= new Array[r];
            }
            Array& operator [](const int& i)
            {
                 return p[i];
            }
            friend istream& operator >> (istream& in, Matrix& m);
          friend ostream& operator << (ostream& out, Matrix& m);
};
istream& operator >> (istream& in, Matrix& m)
{
     for(int i=0 ; i < m.r ; i++)
     {
         for(int j=0 ; j < m.c ; j++)
               in >> m[i][j];
     }
}
ostream& operator << (ostream& out, Matrix& m)
{
     for(int i=0 ; i < m.r ; i++)
     {
         for(int j=0 ; j < m.c ; j++)
               out << m[i][j] << " ";
         out << endl;
     }
}


int main()
{
    Matrix M1(3,3);
    cin >> M1;
  cout << "\n" << M1;
}
