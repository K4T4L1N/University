

#include <iostream>
#include <math.h>
#include <algorithm>
using namespace std;

template <class T>
class Angajat
{
    string nume;
    int cod;
    T b_s;
public:
    Angajat (string a, int code, T val)
    {
        nume= a;
        b_s = val;
        cod = code;
    }
    output()
    {


    if (typeid(T) == typeid(cod))    //in caz ca variabila de tip T e integer, atunci va afisa
    {
        cout <<"denumire angajat: " << nume<<endl;;
        cout <<"numar birou: " <<cod <<endl;;
        cout <<"telefon: " <<b_s <<endl;
    }
    else
     {
        cout <<"denumire angajat: " << nume<<endl;;
        cout <<"salariu: " <<cod <<endl;;
        cout <<"adresa: " <<b_s <<endl;
     }

}
};

int main()
{



    Angajat <int> ang1 ("Maria M", 12, 7900166);
    Angajat <string> ang2 ("Dariu G", 3800, "Igor Vieru 4");
    ang1.output();
    ang2.output();

  return 0;
}
