#include "SharedPtr.h"
#include <iostream>
#include <string>
#include <assert.h>


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

//释放malloc
struct Free
{
	void operator()(void *ptr)
	{
		free(ptr);
		cout << "free()" << endl;
	}
};

void testCase1()
{
    SharedPtr<int> sp1(new int(10));
    assert(*(sp1.get()) == 10);

    SharedPtr<int> sp2(new int(1), default_delete<int>());
    assert(sp2.use_count() == 1);

    auto sp3(sp2); // 拷贝构造函数
    assert(sp3.use_count() == 2);
    {
        auto sp4 = sp2;
        assert(sp4.use_count() == 3 && sp3.use_count() == sp4.use_count());

        assert(sp2.get() == sp3.get() && sp2.get() == sp4.get());
        assert(sp2 == sp3 && !(sp2 != sp4));
    }
    assert(sp3.use_count() == 2);

    SharedPtr<string> sp5(new string("hello"));
    assert(*sp5 == "hello");
    sp5->append(" world");
    assert(*sp5 == "hello world");

    auto sp6 = makeShared<string>(10, '0');
    assert(*sp6 == "0000000000");

    SharedPtr<int> spp;
    assert(spp == nullptr);
    assert(!(spp != nullptr));
}
int main()
{
    cout << "Test SharedPtr" << endl;
    testCase1();

    return 0;    
}
