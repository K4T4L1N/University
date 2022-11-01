#include<iostream>
#include<string>
#include<math.h>
using namespace std;
const double pi = 3.14159;
class Punct{
    public:
    string nume_obiect;

};
class Circumferinta : public Punct{
    private:
    double raza_circumferintei;
    public:
    double Suprafata();
    void Afisare();
    Circumferinta();
    Circumferinta(double ,string);
    ~Circumferinta();
};

double Circumferinta :: Suprafata(){
      return pi *pow(raza_circumferintei,2);}
void Circumferinta :: Afisare(){
    cout<<"Denumirea obiectului : "<<nume_obiect<<endl;
    cout<<"Raza circumferintei : "<<raza_circumferintei<<endl;
    cout<<"Suprafata circumferintei : "<<Suprafata()<<endl;
}
Circumferinta :: Circumferinta(){
    raza_circumferintei= 5;
    nume_obiect = "Unknow";
}
Circumferinta :: Circumferinta(double value,string name)
{
    raza_circumferintei = value;
    nume_obiect = name;

}
Circumferinta ::~Circumferinta()
{
    cout<<"Obiectul "<<nume_obiect<<" a fost distrus cu succes"<<endl;
}
class Sfera : public Punct{
    private:
    double raza_sferei;
    public:
    double Volum();
    void Afisare();
    Sfera();
    Sfera(double,string);
    ~Sfera();
};
Sfera ::Sfera()
{
    raza_sferei=10;
    nume_obiect ="Undefined";
}
Sfera ::Sfera(double value,string name)
{
raza_sferei = value;
nume_obiect = name;
}
Sfera::~Sfera(){
    cout<<"Obiectul "<<nume_obiect<<" a fost distrus cu succes"<<endl;
}
double Sfera :: Volum(){
        return  4*pi*pow(raza_sferei,3)/3;
    }
void Sfera :: Afisare()
{
    cout<<"Denumirea obiectului : "<<nume_obiect<<endl;
    cout<<"Raza sferei : "<<raza_sferei<<endl;
    cout<<"Volumul sferei : "<< Volum()<<endl;

}

main()
{
  /* Circumferinta c;
   c.Afisare();
   Circumferinta d(13,"disc");
   d.Afisare();*/
   Sfera s(15.0,"unknow");
   //s.Afisare();
   Sfera e(15.0,"minge");
   s.Afisare();
    e.Afisare();

   if (s.Volum() > e.Volum() ) cout<< "Sfera   se include in sfera "<<endl;
     if (s.Volum()<e.Volum()) cout<<"Sfera  se include in sfera "<<endl;
     if (s.Volum() ==e.Volum()) cout<<"Sferele au acelasi volum si nu se pot include una in alta"<<endl;

}
