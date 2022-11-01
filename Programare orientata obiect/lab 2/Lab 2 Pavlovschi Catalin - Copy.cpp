///17. Să se definească ierarhia de clase: Punct – Placă rotundă – Vas cilindric. Clasa Vas cilindric trebuie să
///conţină metoda de calculare a volumului vasului. Ultimele două clase din ierarhie trebuie să aibă
///constructori. Să se definească de asemenea funcţia Afişare pentru fiecare clasă. Să se demonstreze lucrul
///fiecărei funcţii membre.
///Să se scrie programul în care se declară două vase, se determină care din ele este mai mare şi care din cele
///două vase poate fi introdus în celălalt.

#include <iostream>
#include<math.h>
# define PI           3.14159265358979323846
using namespace std;



class Punct {
protected:
float x, y;
public:
void SetCoord(float a, float b) {
x = a;
y = b;
}
Punct(float a = 0, float b = 0) {
SetCoord (a, b);
}
float X() const { return x; }
float Y() const { return y; }

void Afisare() {cout<<"Punctul este:("<<x<<"."<<y<<")"<<endl;};
};
class placa: public Punct {
float r;
public:
void SetR(float a) { r = 0; }
placa(float a = 0, float b = 0, float c = 1):
Punct(a,b), r(c) { }
placa(placa& c): Punct(c.x, c.y), r(c.r) {}

void Afisare() {cout<<"aria placii  este:"<<PI*pow(x,2)<<endl;}
};


class cilindru: public placa {
float r, v;
public:
void SetR(float a) { r = 0; }
cilindru(float a = 0, float b = 0, float c = 1):
placa(a,b), r(c) { }
cilindru(cilindru& c): placa(c.x, c.y), r(c.r) {}
double Get_Volume() {v=PI*pow(x,2)*y;}
void Afisare() {cout<<"Volumul cilindrului  este:"<<PI*pow(x,2)*y<<endl;}

};





int main()
{

   Punct a(2,3);
   a.Afisare();

   placa c(4);
   c.Afisare();

   cilindru d(5,6);
   d.Afisare();

   cilindru f(6,8);
   f.Afisare();

   if (d.Get_Volume() < f.Get_Volume())
         cout <<'\n'<< "Triunghiul 2 mai mare ca Triunghiul 1, si T.1 poate fi inclus in T.2"<< '\n';
    else
        cout << '\n'<<"Triunghiul 1 mai mare ca Triunghiul 2, si T.2 poate fi inclus in T.1"<< '\n';
    return 0;
}
