
// #include "CowPtr.h"
// #include <iostream>
// #include <string>

// #include <assert.h>

// using namespace std;

// void testCase1(){
//     cow_ptr<string> cp1(new string("hello"));
//     assert(*(*cp1) == "hello");
//     assert(cp1);
    
//     auto cp2 = cp1;
//     assert(*(*cp2) == "hello");

//     cow_ptr<string> cp3;
//     cp3 = cp1;
//     assert(*(*cp3) == "hello");

//     assert(cp1.get() == cp2.get() && cp2.get() == cp3.get());
//     assert(cp1 == cp2 && !(cp2 != cp3));

//     string zxh(" zouxiaohang");
//     cp1->append(zxh);
//     assert(*(*cp1) == "hello zouxiaohang");
//     assert(*(*cp2) == "hello" && *(*cp3) == "hello");

//     cow_ptr<string> cp4;
//     assert(cp4 == nullptr);
// }
// void testCase2(){
//     // cow_ptr<string> cp1 = make_cow<string>("zouxiaohang");
//     // auto cp2 = cp1, cp3 = cp1;
//     // assert(cp1 == cp2 && cp2 == cp3);
//     // assert(*cp1 == *cp2 && *cp2 == *cp3 && *cp3 == "zouxiaohang");

//     // //cp2->capacity(); change the cow_ptr -> ÓÉÓÚŽúÀíÀàŽúÀíµÄÀàÐÍÎŽÖª(T)Ôì³ÉµÄ
//     // string s = *cp2;//read
//     // assert(s == "zouxiaohang");
//     // assert(cp1 == cp2 && cp2 == cp3);
//     // assert(*cp1 == *cp2 && *cp2 == *cp3 && *cp3 == "zouxiaohang");

//     // *cp2 = ("C++");//write
//     // assert(*cp1 == *cp3 && *cp3 == "zouxiaohang");
//     // assert(*cp2 == "C++");
// }

// int main() 
// {
//     cout << "Test CowPtr" << endl;
//     testCase1();
//     testCase2();
//     return 0;
// }