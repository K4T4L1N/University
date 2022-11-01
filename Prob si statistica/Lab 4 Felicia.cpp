///Să se creeze clasa-șablon Poligon. Clasa trebuie să conțină constructori și o metodă de afișare.
///Să se realizeze reprezentările a) Denumirea figurii, Numărul de laturi; b) Denumirea figurii, suprafața.

#include <iostream>
#include <math.h>
#include <algorithm>
using namespace std;

template <class T>
class Materiale
{
    string nume;
    int cod;
    T g_c;
public:
    Materiale (string a, int code, T val)
    {
        nume= a;
        g_c = val;
        cod = code;
    }
    output()
    {




    if (g_c == (int)g_c)
    {
        cout <<"denumirea material: " << nume<<endl;;
        cout <<"cod: " <<cod <<endl;;
        cout <<"Cantitate: " <<g_c <<endl;
    }
    else
     {
        cout <<"denumirea material: " << nume<<endl;;
        cout <<"cod: " <<cod <<endl;;
        cout <<"Greutate: " <<g_c <<endl;
     }

}
};

int main()
{



    Materiale <double> mat1 ("Lemn", 02, 3388.0);
    Materiale <int> mat2 ("Ciment", 04, 320);
    mat1.output();
    mat2.output();

  return 0;
}
