
//11.	Să se definească ierarhia de clase: Punct – Latură – Triunghi.
//Clasa Triunghi trebuie să conţină metodele de calculare a suprafeţei şi al perimetrului.
//Ultimele două clase din ierarhie trebuie să aibă constructori. Să se definească de asemenea funcţia Afişare pentru fiecare clasă.
//Să se demonstreze lucrul fiecărei funcţii membre.
//Să se scrie programul în care se declară două triunghiuri, se determină care dintre ele este mai mic şi dacă cel mai mic poate fi inclus
//în cel mai mare.

#include <iostream>
#include <cmath>

using namespace std;

class Point_length
{
protected:
    double length;
    double area;

public:
    getLenght();
    double getLength(){return length; }
    double getArea(){ return area; }
    void Afisare() {cout<<"lungimea laturii"<<length<<endl;}
};

class Shape:public Point_length //Laturile
{
protected:
    double perimeter;

public:
    double getPerimeter(){ return perimeter;}
    void Afisare() {cout<<"perimetrul"<<perimeter<<endl;}
};

class Triangle: public Shape
{
public:
    Triangle( double aSize = 0)
    {
        length = aSize; //dimensiunea laturii
        area = 0.433 * length * length; //aria
        perimeter = 3 * length; //perimetrul
    }
     void Afisare() {cout<<"aria"<<area<<endl; cout<<"perimetrul"<<perimeter<<endl;}
};

float lenght(int x1, int y1, int x2, int y2) //formula distantei intre 2 vectori
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));}
int main()
{


    Triangle t1(lenght(20, 21, 35, 48));
    cout << "Triangle1: " << t1.getLength() << ' ' << t1.getPerimeter() <<  ' ' << t1.getArea() << '\n';
    Triangle t2(lenght(-3, 28, 50, 22));
    cout << "Triangle2: " << t2.getLength() << ' ' << t2.getPerimeter() <<  ' ' << t2.getArea() << '\n';

    if (t1.getArea() < t2.getArea())
         cout <<'\n'<< "Triunghiul 2 mai mare ca Triunghiul 1, si T.1 poate fi inclus in T.2"<< '\n';
    else
        cout << '\n'<<"Triunghiul 1 mai mare ca Triunghiul 2, si T.2 poate fi inclus in T.1"<< '\n';
    return 0;
}
