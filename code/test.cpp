#include <iostream>
#include <vector>
#include <initializer_list>
#include <memory>

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
        std::unique_ptr<TA[]> uaar(new TA[3]);
    }

    std::shared_ptr<int> sp1;
    cout << sp1.use_count() << endl;
    return 0;
}