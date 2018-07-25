#include <iostream>

class A {
public:
    A() {
        std::cout << "A" << std::endl;
    }
    virtual ~A(){}
};
class B {
public:
    B() {
        std::cout << "B" << std::endl;
    }
    ~B() {}
};
class C : A {
public:
    C() {
        std::cout << "C" << std::endl;
    }
    ~C(){}
    B b;
};
int main()
{
    C c;
   return 0;
}