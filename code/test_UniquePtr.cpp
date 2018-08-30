
#include "UniquePtr.h"

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;
namespace {
	template<class T>
	struct Del{
		void operator()(T *p){ delete p; }
	};
}
void testCase1()
{
    UniquePtr<int> up1;
    assert(up1 == nullptr);

    UniquePtr<int> up2(new int(5));
    assert(up2 != nullptr);
    assert(*up2 == 5);

    UniquePtr<string, Del<string>> up3(new string("test UniquePtr"), Del<string>());
    assert(up3 != nullptr);
    assert(*up3 == "test UniquePtr");

    auto up4(std::move(up2));
    assert(*up4 == 5);
    assert(up2 == nullptr);

    UniquePtr<string, Del<string>> up5;
    up5 = std::move(up3);
    assert(*up5 == "test UniquePtr");
    assert(up3 == nullptr);

    auto up6 = make_unique<string>(6, 'z');
    assert(*up6 == "zzzzzz");
}
void testCase2()
{
    auto up = make_unique<string>(10, '0');
    up->append("111");
    assert(*up == "0000000000111");

    up.release();
    assert(up == nullptr);

    up.reset(new string("hello"));
    assert(*up == "hello");
}
void testCase3()
{
    auto up1 = make_unique<string>(2, '0');
    auto up2 = make_unique<string>(2, '1');

    up1.swap(up2);
    assert(*up1 == "11" && *up2 == "00");

    swap(up1, up2);
    assert(*up1 == "00" && *up2 == "11");
}

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
    cout << "Test UniquePtr" << endl;

    testCase1();
    testCase2();
    testCase3();

    // { // 不能使用数组
    //     UniquePtr<TA[]> taarr(new TA[3]); 
    // }
    return 0;
}