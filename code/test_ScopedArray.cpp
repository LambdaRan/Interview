
#include <iostream>
#include "ScopedArray.h"

using namespace std;

class TA 
{
public:
    TA() 
    {
        cout << "TA()" << endl;
    }
    ~TA() 
    {
        cout << "~TA()" << endl;
    }
};

int main()
{
    {
        ScopedArray<TA> ta1(new TA[3]);
    }
    ScopedArray<int> ta2(new int[3]);
    cout << ta2[1] << endl;
    return 0;
}