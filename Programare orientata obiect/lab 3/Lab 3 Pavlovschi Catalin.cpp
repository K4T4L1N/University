//Creaţi clasa matricelor care conţine un pointer la date de tip întreg, numărul de linii, numărul de coloane şi variabila erorii. Definiţi constructorul
//implicit, constructorul cu un parametru şi cu doi parametri, constructorul de copiere şi destructorul. Supraîncărcaţi operaţiile =, +, -, +=, -=, *, *=
//cu obiecte ale acestei clase şi cu numere întregi. Supraîncărcaţi operaţiile de intrare/ieşire de flux (<< şi >>).
//Să se definească operaţiile ca operatori ai clasei iar una din ele ca funcţie prietenă.


#include <iostream>
using namespace std;

class matrix
{
private:
   int **p, m, n;
public:
   matrix(int row, int col)
   {
      m = row;
      n = col;
      p = new int*[m];
      for (int i = 0; i < m; i++)
	 p[i] = new int[n];
   }
matrix(int val)
   {
      m = val;
      n = val;
      p = new int*[m];
      for (int i = 0; i < m; i++)
	 p[i] = new int[n];
   }

   ~matrix()
   {
      for (int i = 0; i < m; i++)
	 delete p[i];
      delete p;
   }
   void accept()   //Input
   {
      cout<<"\n Enter matrix elements:"<<endl;
      for(int i = 0; i < m; i++)
      {
	 for(int j = 0; j < n; j++)
	 {
	    cin >> p[i][j];
	 }
      }
   }
   void display()       //Output
   {
      cout <<"The matrix is:";
      for(int i = 0; i < m; i++)
      {
	 cout <<endl;
	 for(int j = 0; j < n; j++)
	 {
	    cout << p[i][j] <<" ";
	 }
      }
   }
   matrix operator+ (matrix m2)
   {
      matrix T(m, n);
      for(int i = 0; i < m; i++)
      {
	 for(int j = 0; j < n; j++)
	 {
	    T.p[i][j] = p[i][j] + m2.p[i][j];
	 }
      }
      return T;
   }


   matrix operator- (matrix m3)
   {
      matrix P(m, n);
      for(int i = 0; i < m; i++)
      {
	 for(int j = 0; j < n; j++)
	 {
	    P.p[i][j] = p[i][j] - m3.p[i][j];
	 }
      }
      return P;
   }

    matrix operator-= (matrix m4)
   {
      matrix O(m, n);
      for(int i = 0; i < m; i++)
      {
	 for(int j = 0; j < n; j++)
	 {
	    O.p[i][j] = p[i][j] - p[i][j];
	 }
      }
      return O;
   }

   matrix operator+= (matrix m5)
   {
      matrix I(m, n);
      for(int i = 0; i < m; i++)
      {
	 for(int j = 0; j < n; j++)
	 {
	    I.p[i][j] = p[i][j] + p[i][j];
	 }
      }
      return I;
   }

   matrix operator= (matrix eq)
   {
      m = eq.m;
      n = eq.n;
      p = eq.p;

      return *this;
   }
/*
   istream &operator>>(istream &in, matrix &m)
{
   for (int i = 0; i < m.dx; ++i)    {
       for (int j = 0; j < m.dy; ++j)
           if ( ! (in >> m.p[i][j]) ) return in;
 }
 return in;
}

  friend ostream &operator<< (ostream &output, const matrix m2)
   {
      matrix p(m,n);
      for(int i = 0; i < 4; i++)
      {
	 output <<endl;
	 for(int j = 0; j < 3; j++)
	 {
	  output << p[i][j] <<" ";
	 }
      }
   }
*/

   friend matrix operator* (matrix, matrix);
   friend matrix operator*= (matrix, matrix);
};

matrix operator* (matrix a , matrix b)
{
   matrix B(1,1);
   if(a.n == b.m)
   {
      matrix T(a.m, b.n);
      for(int i = 0; i < a.m; i++)
      {
	 for(int k = 0; k < b.n; k++)
	 {
	    T.p[i][k] = 0;
	    for(int j = 0; j < a.n; j++)
	    {
	       T.p[i][k]+= a.p[i][j] * b.p[j][k];
	    }
	 }
      }
      B = T;
   }
   return B;
}

matrix operator*= (matrix a , matrix b)
{
   matrix B(1,1);
   if(a.n == b.m)
   {
      matrix T(a.m, b.n);
      for(int i = 0; i < a.m; i++)
      {
	 for(int k = 0; k < b.n; k++)
	 {
	    T.p[i][k] = 0;
	    for(int j = 0; j < a.n; j++)
	    {
	       T.p[i][k]+= a.p[i][j] * a.p[i][j];
	    }
	 }
      }
      B = T;
   }
   return B;
}


int main()
{
  matrix rf(3,3), ff(3,3); /*fr(4,3);*/

/*
   fr.accept();
   cout << "\nfr\n";
   fr.display();


   cout << "\nfr << \n";
   cout << fr;
*/

   rf.accept();
   cout << "\nrf\n";
   rf.display();
cout <<endl;
   ff.accept();
   cout << "\nff\n";
   ff.display();
cout <<endl;
   cout << "\nff = ff + rf\n";
   ff = rf + ff;
   ff.display();
cout <<endl;
   cout << "\nff = (rf * ff)\n";
   ff = (rf * ff);
   ff.display();
cout <<endl;
   cout << "\nrf += ff\n";
   rf += rf;
   rf.display();
cout <<endl;
   cout << "\nrf - ff\n";
   rf = rf - ff;
   rf.display();
cout <<endl;
cout << "\nrf - ff\n";
   rf -= ff;
   rf.display();
cout <<endl;
   cout << "\nrf *= ff\n";
   ff *= ff;
   ff.display();


}
