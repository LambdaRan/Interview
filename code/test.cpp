#include <iostream>


int func1(const int& v1)
{
    int re = v1;
    return re;
}

int func2(const int& v2)
{
    int re = func1(v2);
    return re;
}

int main()
{
    std::cout << func2(3) << std::endl;;

    return 0;
}