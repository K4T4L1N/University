

#include <iostream>
#include <math.h>
#include <algorithm>
using namespace std;

template <class T>
class Student
{
    string nume;
    string gr;
    T m_a;
public:
    Student (string a, string gre, T val)
    {
        nume= a;
        m_a = val;
        gr = gre;
    }
    output()
    {




    if (typeid(T) == typeid(double))
    {
        cout <<"denumirea material: " << nume<<endl;;
        cout <<"grupa: " <<gr <<endl;;
        cout <<"media: " <<m_a <<endl;
    }
    else
     {
        cout <<"denumirea material: " << nume<<endl;;
        cout <<"grupa: " <<gr <<endl;;
        cout <<"anul nasterii: " <<m_a <<endl;
     }

}
};

int main()
{



    Student <double> st1 ("Marius G", "I-2001", 8.4);
    Student <int> st2 ("Marina B", "M-2021", 2021);
    st1.output();
    st2.output();

  return 0;
}
