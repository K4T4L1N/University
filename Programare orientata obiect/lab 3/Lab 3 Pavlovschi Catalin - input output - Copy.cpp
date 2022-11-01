#include<iostream>
#include<string.h>

using namespace std;

class Angajat{
    string Nume;
     string functie;
     int salariu;

     public:
     void Print()
     {
         cout<<Nume<<endl;
         cout<<salariu<<endl;
         cout<<functie<<endl;
     }
     Angajat(string D,string P,int C)
     {
        Nume=D;
         functie=P;
        salariu=C;
     }
   friend   Angajat operator*(int f,Angajat &other);
 Angajat operator++(Angajat &other,int f)
 {
     other.salariu+f;
     return *this;
 }

};
  Angajat operator*(int f,Angajat &other){
         other.salariu -f;
         return other; }

int main()
{
    Angajat ob("Marcel.B","Procuror",20000);

     ob++5;
     6*ob;


    return 0 ;
}
