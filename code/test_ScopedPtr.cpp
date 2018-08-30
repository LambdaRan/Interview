
#include <iostream>
#include "ScopedPtr.h"

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
        ScopedPtr<TA> ta1(new TA());
    }
    ScopedPtr<TA> ta2(new TA());
    
    return 0;
}