
#include "SharedPtrII.h"
#include <iostream>
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

int main()
{
    SharedPtr<TA> sp1(new TA());
    assert(sp1.use_count() == 1);

    // 拷贝构造函数
    SharedPtr<TA> sp2(sp1);
    assert(sp2.use_count() == 2);

    {// 拷贝赋值运算符
        SharedPtr<TA> sp3 = sp2;
        assert(sp3.use_count() == 3);
    }
    assert(sp2.use_count() == 2);

    SharedPtr<int> spi(new int(66));
    cout << *spi << endl;

    int* iptr = spi.get();
    cout << *iptr << endl;

    SharedPtr<string, Free> sp4((string *)malloc(sizeof(string)), Free());//字符串操作

    return 0;
}
