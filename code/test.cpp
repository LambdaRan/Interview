#include <iostream>
#include <vector>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <array>

#include <string.h>

using namespace std;

//#pragma GCC diagnostic ignored "-Wunused-parameter"
template <class T, std::size_t N>
constexpr std::size_t size(const T (&pArray)[N]) noexcept
{
    return (void)pArray, N;
}
//#pragma GCC diagnostic error "-Wunused-parameter"

int main()
{
    int arr[10];
    //int ints[] = {1,2,3,4};
    int ints[3][4];
    std::cout << std::extent<decltype(ints), 0>::value << '\n'; //< array size 
    std::cout << std::extent<decltype(ints), 1>::value << '\n'; //< array size 
    std::cout << std::rank<decltype(arr)>::value << endl;
    std::cout << std::rank<int[1][2][3]>::value << '\n';

      cout << size(arr) << endl;
    return 0;
}
