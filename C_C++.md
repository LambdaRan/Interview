
# C/C++ 基础知识
> 参考：https://github.com/linw7/Skill-Tree



# <span id = "root">目录</span>

| Chapter 1 | Chapter 2 | Chapter 3| Chapter 4 | 
| :---------: | :---------: | :---------: | :---------: | 
| [编程基础](#base)|[面向对象基础](#oop)|[标准模板库](#stl)|[待定](#other)|


# <span id = "base">编程基础</span>

## 1. c/c++中static关键字
> Linux 多线程服务端编程（陈硕）ch12.5.1 p526
### C语言的static关键字有两种用途：
* 第一种 用于函数内部修饰变量，即函数内静态变量。
这种变量的生存期长于该函数，使得函数具有一定的“状态”。使用静态变量的函数一般是不可重入的，也不是线程安全的。
* 第二种 用于文件级别（函数体之外），修饰变量和函数。
用于表示该变量或函数只在本文件可见，其他文件看不到、也访问不到该变量或函数。专业的说法叫“具有internal linkage”。
### C++语言的static关键字的四种用法：
* 第三种 用于修饰class的数据成员，即所谓“静态成员”。
这种成员的生存期大于class的对象（实体、instance）。静态数据成员是每个class有一份，普通数据成员是每个instance有一份，
因此，也分别叫做class variable和instance variable。
* 第四种 用于修饰class的成员函数，即所谓“静态成员函数”。
**这种成员函数只能访问class variable和其他静态程序函数，不能访问instance variable或instance method**。

> C++ Primer 5th P269   

类成员声明前加static，使得其与类关联在一起。    
类的静态成员存在于任何对象之外，对象中不包含任何与静态数据成员有关的数据.   
静态成员函数也不与任何对象绑定在一起，它们不包含this指针。  
静态成员函数 不能声明成const的。 
我们可以使用类的对象、引用或者指针来访问静态成员。  
static关键字只能出现在类内部的声明语句中    
* 类内声明有static
* 类外定义没有static

一般来说，我们不能在类的内部初始化静态成员，相反，必须在类的外部定义和初始化每个静态成员.    
要想确保对象只定义一次，最好的办法是把静态数据成员的定义与其他非内联函数的定义放在同一个文件中  
我们可以为静态成员提供const整数类型的类内初始值，不过要求静态成员必须是字面值常量类型的constexpr.    
如果静态成员仅限于编译器使用，可以只声明而不需要在类外定义；其他情况必须定义；  
如果在类的内部提供了一个初始值，则成员的定义不能再指定一个初始值了；
*  error: duplicate initialization of ‘A::value’    

静态成员能用于某些场景，而普通成员不能：
* 静态成员可以是不完全类型。特别的，静态数据成员的类型可以就是它所属的类类型。
    而非静态数据成员则受到限制，只能声明成他所属类的指针或引用。
* 我们可以使用静态成员作为默认实参

## 2. new与malloc、 delete与free的区别

* malloc和free是标准库函数，支持覆盖； new和delete是运算符(关键字)，并且支持重载。
* malloc仅仅分配内存空间，free仅仅回收空间，不具备调用构造函数和析构函数功能，用malloc分配空间存储类的对象存在风险;
new和delete除了分配回收功能外，还会调用构造函数和析构函数。
* malloc和free返回的是void类型指针（必须进行类型转换），new和delete返回的是具体类型指针。
使用new操作符申请内存分配时无须指定内存块的大小，编译器会根据类型信息自行计算。而malloc则需要显式地指出所需内存的尺寸。
* new操作符从自由存储区（free store）上为对象动态分配内存空间，而malloc函数从堆上动态分配内存。自由存储区是C++基于new操作符的一个抽象概念，凡是通过new操作符进行内存申请，该内存即为自由存储区。而堆是操作系统中的术语，是操作系统所维护的一块特殊内存，用于程序的内存动态分配，C语言使用malloc从堆上分配内存，使用free释放已分配的对应内存。自由存储区不等于堆，如上所述，布局new就可以不位于堆中。
* delete和delete[]区别
    * delete只会调用一次析构函数。
    * delete[]会调用数组中每个元素的析构函数。

## 3. struct和class的区别、struct与union区别
* struct在C语言中是聚合数据类型。
* C++中两者唯一一点区别是：struct和class的默认访问权限不同。struct默认是public访问权限，而class默认private访问权限。
* 都可以继承。
* union(联合)是一种特殊的类，一个union可以有多个数据成员，但是在任意时刻只有一个数据成员可以有值；
* union不能含有引用类型的成员；
* union可以为其成员指定public、protected、private等保护标记；默认情况下成员都是公有的；
* union可以定义构造/析构函数在内的成员函数；
* union既不能继承自其他类，也不能作为基类使用，所以union中不能含有虚函数；

## 4. 指针和引用区别
* **引用只是别名**，不占用具体存储空间，只有声明没有定义；
* **引用在声明时必须初始化**为另一变量，一旦出现必须为typename refname &varname形式；
* 引用一旦初始化之后就不可以再改变（变量可以被引用为多次，但引用不能重新绑定到另外一个变量）；
* 不存在指向空值的引用，必须有具体实体；但是存在指向空值的指针（未定义）；
* 因为引用本身不是一个对象，所以不能定义引用的引用； 
* 指针本身就是一个对象，允许指针赋值和拷贝；
* 指针在生命周期内可以先后指向几个不同的对象；
* 指针无需在定义时赋值；指针声明和定义可以分开，可以先只声明指针变量而不初始化，等用到时再指向具体变量；
* 可以定义指针的指针（二级指针）；
* 对引用类型执行sizeof运算得到被引用对象所占空间的大小；
* 对指针执行sizeof运算得到指针本身所占空间的大小；

## 5. sizeof运算符
> C++ Primer 5th p139   

sizeof运算符返回一条表达式或一个类型名字所占的字节数；sizeof运算符满足右结合律。    
在sizeof的运算对象中解引用一个无效指针仍然是一种安全的行为，因为指针实际上并没有被真正使用；    
sizeof运算符无须提供一个具体对象，就能知道类成员的大小；     
* 对引用类型执行sizeof运算得到被引用对象所占空间的大小；
* 对指针执行sizeof运算得到指针本身所占空间的大小；
* 对解引用指针执行sizeof运算符得到指针指向的对象所占空间的大小，指针不需有效；
* 对数组执行sizeof运算得到整个数组所占空间的大小；
* 对string对象或vector对象执行sizeof运算只返回该类型固定部分的大小，不会计算对象中的元素占用了多少空间；

## 6. strlen和sizeof区别

* sizeof是运算符，并不是函数，结果在编译时得到而非运行中获得；strlen是字符处理的库函数。
* sizeof参数可以是任何数据的类型或者数据（sizeof参数不退化）；strlen的参数只能是字符指针且结尾是'\0'的字符串。
* 因为sizeof值在编译时确定，所以不能用来得到动态分配（运行时分配）存储空间的大小。
* char str[] = "lambda"; strlen(str) = 6; sizeof(str) = 7;

## 7. 对象赋值时深拷贝和浅拷贝

类内含有指针指向的资源（动态申请的空间），当发生拷贝时，如果新对象重新分配资源，将指针所指资源复制到新对象中，这叫深拷贝；如果新对象没有重新分配资源，只是简单的拷贝指针变量，这叫浅拷贝；
* 深拷贝时，当被拷贝对象存在动态分配的存储空间时，需要先动态申请一块存储空间，然后逐字节拷贝内容。
* 浅拷贝仅仅是拷贝指针字面值。
* 当使用浅拷贝时，如果原来的对象调用析构函数释放掉指针所指向的数据，则会产生空悬指针。因为所指向的内存空间已经被释放了。
## 8. 结构体内存对齐问题
> https://blog.csdn.net/dream_1996/article/details/54934334 
### 内存对齐的原因：
1. 平台原因
   不是所有硬件平台都可以访问任意地址上的任意数据； 
   某些硬件平台只能在某些地址处取某些特定类型的数据，否则抛出硬件异常。 
2. 性能原因
  数据结构（尤其是栈）应该尽可能的在自然边界上对齐。    
  原因在于在访问未对齐的内存时，处理器需要进行两次内存访问；而对齐的内存访问仅需要一次。    

### 结构体（struct）内存对齐规则：
1. 第一个成员在与结构体变量偏移量为0的地址处。
2. 其它成员变量要对齐到某个数字（对齐数）的整数倍的地址处。
   * 对齐数=编译器默认的一个对齐数与该成员大小的一个较小值  
   * Vs中默认的对齐数是8  
   * Linux中默认的对齐数是4   
3. 结构体总大小：最大对齐数（每个成员变量的除了第一个成员都有一个对对齐数）的整数倍。（每个成员变量在对齐之后，把成员大小加起来，再扩大到最大对齐数的整数倍）
4. 如果嵌套了结构体的情况，嵌套的结构体对齐到自己的最大对齐数的整数倍处，结构体的整体大小就是所有对齐数（含嵌套结构体的对齐数）的整数倍。
   
* 结构体内成员按照声明顺序存储，第一个成员地址和整个结构体地址相同。
* 未特殊说明时，按结构体中size最大的成员对齐（若有double成员），按8字节对齐。

### 编译器命令 
* // #pragma pack (n)         作用：C编译器将按照n个字节对齐。
* // #pragma pack ()          作用：取消自定义字节对齐方式。
* // #pragma  pack (push,1)   作用：是指把原来对齐方式设置压栈，并设新的对齐方式设置为1个字节对齐
* // #pragma pack(pop)        作用：恢复对齐状态 
* 使用offsetof宏来判断结构体中成员的偏移地址。#define offsetof(type,menber) (size_t)&(((type*)0)->member)

## 9. 宏定义

### 宏定义和函数有何区别
* 宏在编译时完成替换，之后被替换的文本参与编译，相当于直接插入了代码，运行时不存在函数调用，执行起来更快；函数调用在运行时需要跳转到具体调用函数。
* 宏函数属于在结构中插入代码，没有返回值；函数调用具有返回值。
* 宏函数参数没有类型，不进行类型检查；函数参数具有类型，需要检查类型。
* 宏函数不要在最后加分号。

### 宏定义和const区别
* 宏替换发生在编译阶段之前，属于文本插入替换；const作用发生于编译过程中。
* 宏不检查类型；const会检查数据类型。
* 宏定义的数据没有分配内存空间，只是插入替换掉；const定义的变量只是值不能改变，但要分配内存空间。

### 宏定义和typedef区别
* 宏主要用于定义常量及书写复杂的内容；typedef主要用于定义类型别名。
* 宏替换发生在编译阶段之前，属于文本插入替换；typedef是编译的一部分。
* 宏不检查类型；typedef会检查数据类型。
* 宏不是语句，不在在最后加分号；typedef是语句，要加分号标识结束。
* 注意对指针的操作，typedef char* p_char 和 #define p_char char* 区别巨大。

### 宏定义和内联函数(inline)区别
* 在使用时，宏只做简单字符串替换（编译前）。而内联函数可以进行参数类型检查（编译时），且具有返回值。
* 内联函数本身是函数，强调函数特性，具有重载等功能。
* 内联函数可以作为某个类的成员函数，这样可以使用类的保护成员和私有成员。而当一个表达式涉及到类保护成员或私有成员时，宏就不能实现了。

### 宏定义和枚举类型（enum）
* 枚举属于字面值常量类型
* C++两种枚举:
    1. 限定作用域 enum class(或struct) 枚举名字 { 逗号分隔枚举成员};
    2. 不限定作用域 enum 枚举名字(可选) {逗号分隔枚举成员}；
* 在限定作用域的枚举类型中，枚举成员的名字遵循常规的作用域准则，并且在枚举类型的作用域外不可访问;
* 枚举成员是const,因此在初始化枚举成员时提供的初始值必须是常量表达式;
* 枚举可作为switch语句中case标签;非类型模板形参使用;等
* 可以指定enum的大小：
    1. 不限定作用域 enum intValue : unsigned long long {};
    2. 限定作用域 的enum成员默认是int;
* 前置声明: 
    1. enum intValue : unsigned long long;// 不限定作用域的必须指定成员类型
    2. enum class open_modes; // 限定作用域的枚举类型可以使用默认的成员类型int

## 10. const限定符
* const变量的值不能被改变
* const 对象声明时必须初始化    
* 默认状态下,const对象仅在文件内有效 
### const的引用
```
int i = 42;
const int &r1 = i; // 允许const int& 绑定到一个普通int对象上
const int &r2 = 42; // r2是一个常量引用
```
* 常引用可以理解为常量指针，形式为const typename & refname = varname;
* 常引用下,原变量值不会被别名所修改;
* 原变量的值可以通过原名修改;
* 常引用通常用作只读变量别名或是形参传递;
### const与指针
```
int i = 42;
const int *ptr = &i; 
```
* 指向常量的指针也没有规定其所指的对象必须是一个常量;
* 常量指针是一个指针，读成常量的指针，指向一个只读变量。如int const *p或const int *p;
* 指针常量是一个不能给改变指向的指针。如int *const p;
```
const double pi = 3.1415926;
const double *const pip = &pi;
```
* pip是一个指向常量对象的常量指针
```
const int a;
int const a;
const int *a;
int *const a;
```
* int const a和const int a均表示定义常量类型a。
* const int * a,其中a为指向int型变量的指针, const在 * 左侧，表示a指向不可变常量。(看成const (*a)，对引用加const)
* int *const a，依旧是指针类型，表示a为指向整型数据的常指针。(看成const(a)，对指针const)
```
const int* p;
指向常量的指针；不能通过p修改所指向的值；指针p可指向其他变量；
int* const pp;
常量指针；可通过pp修改所指向的值；但指针pp不可再指向其他变量；
```
### 顶层const与底层const
* 顶层const(top-level const)表示指针本身是个常量
* 底层const(low-level const) 表示指针所指的对象是一个常量
* const星号(*) --> 被指物是常量 --> 底层const
* 星号(*)const --> 指针本身是常量 --> 顶层const
* 声明引用的const都是底层const
```
int i = 0;
int *const p1 = &i;  // 不能改变p1的值,顶层const
const int ci = 42;   // 不能改变ci的值,顶层const
const int *p2 = &ci; // 允许改变p2的值,底层const
const int *const p3 = p2; // 靠右的const是顶层const, 靠左的const是底层const
const int &r = ci;   // 声明引用的const都是底层const
```
* 非常量可以转换成常量，反之则不行
```
int *p = p3; // 错误：p3包含底层const的定义，而p没有
p2 = p3;     // 正确：p2和p3都是底层const
p2 = &i;     // 正确：int*能转换成const int*
int &r = ci; // 错误：普通的int&不能绑定到int常量上
const int &r2 = i; // 正确：const int&可以绑定到一个普通的int上
```
### const形参与实参

当用实参初始化形参时会忽略掉顶层const.换句话,形参的顶层const被忽略掉了,当形参有顶层const时，传给它常量对象或者非常量对象都是可以的.
```
void fcn(const int i){/* fcn能够读取i,但是不能向i写值*/};
void fcn(int i){/* */}; // 错误：重复定义fcn(int)
```

## 11. 数组名和指针（这里为指向数组首元素的指针）区别

* 二者均可通过增减偏移量来访问数组中的元素。
* 数组名不是真正意义上的指针，可以理解为常指针，所以数组名没有自增、自减等操作。
* 当数组名当做形参传递给调用函数后，就失去了原有特性，退化成一般指针，多了自增、自减操作，但sizeof运算符不能再得到原数组的大小了。
```
int *p[10]
int (*p)[10]
int *p(int)
int (*p)(int)
```
* int *p[10]表示指针数组，强调数组概念，是一个数组变量，数组大小为10，数组内每个元素都是指向int类型的指针变量。
* int (*p)[10]表示数组指针，强调是指针，只有一个变量，是指针类型，不过指向的是一个int类型的数组，这个数组大小是10。
* int* p(int)是函数声明(指针函数)，函数名是p，参数是int类型的，返回值是int *类型的。
* int (*p)(int)是函数指针，强调是指针，该指针指向的函数具有int类型参数，并且返回值是int类型的。
```
假设数组int a[10];
int (*p)[10] = &a;
```
* a是数组名，是数组首元素地址，+1表示地址值加上一个int类型的大小，如果a的值是0x00000001，加1操作后变为0x00000005。*(a + 1) = a[1]。
* &a是数组的指针，其类型为int (*)[10]（就是前面提到的数组指针），其加1时，系统会认为是数组首地址加上整个数组的偏移（10个int型变量），值为数组a尾元素后一个元素的地址。
* 若(int *)p ，此时输出 *p时，其值为a[0]的值，因为被转为int *类型，解引用时按照int类型大小来读取。

## 12. 野指针是什么？

* 也叫空悬指针，不是指向null的指针，是指向垃圾内存的指针。
* 产生原因及解决办法：
    * 指针变量未及时初始化 => 定义指针变量及时初始化，要么置空。
    * 指针free或delete之后没有及时置空 => 释放操作后立即置空。
    * 指针操作超越了变量的作用域范围；
## 13. volatile mutable

* 参考：http://hedengcheng.com/?p=725
* volatile定义变量的值是易变的，每次用到这个变量的值的时候都要去重新读取这个变量的值，而不是读寄存器内的备份。
* 多线程中被几个任务共享的变量需要定义为volatile类型。

* 如果希望能修改类的某个数据成员，即使是在一个const成员函数内。可在变量的声明中加入mutable关键字;C++Primer5th P245
* lambda表达式中，对于一个值被拷贝的变量，lambda不会改变其值.如果我们希望能改变一个被捕获的变量的值，就必须在参数列表首加上关键字mutable。
```
void fcn() 
{
    size_t v1 = 42;
    auto f = [v1]() mutable { return ++v1; };
    ...
}
```

## 14. 堆和栈的区别
* 申请方式不同:
    * 栈由系统自动分配;
    * 堆由程序员手动分配;
* 申请大小限制不同；
    * 栈顶和栈底是之前预设好的，大小固定，可以通过ulimit -a查看，由ulimit -s修改；
    * 堆向高地址扩展，是不连续的内存区域，大小可以灵活调整；
* 申请效率不同：
    * 栈由系统分配，速度快，不会有碎片；
    * 堆由程序员分配，速度慢，且会有碎片；

## 15. C++内存相关问题

* C++的内存布局
    * 在C++中，内存区分为5个区，分别是堆、栈、自由存储区、全局/静态存储区、常量存储区；
* C++内存分配方式
    * 在堆区分配,在栈区分配,静态存储区分配 (待考证)
* 重载new和delete
    * `new 表达式 执行三个步骤`
        1. 第一步 new表达式调用一个名为 **operator new(或operator new[])** 的标准库函数.该函数分配一个足够大的,原始的,未命名的内存空间以便存储待定类型的对象(或对象数组);
        2. 第二步 编译器运行对应的构造函数以构造这些对象,并为其传入初始值;
        3. 第三步 对象被分配空间并构造完成,返回一个指向该对象的指针;
    * `delete表达式 执行两个步骤`
        1. 第一步 对指针所指对象或者对象数组指针所指的数组中的元素执行对应的析构函数;
        2. 第二步 编译器调用名为 **operator delete(或operator delete[])** 的标准库函数释放内存空间;
    * `应用程序可以在全局作用域中定义operator new函数和operator delete函数,也可以将他们定义为成员函数`

## 16. 源码到可执行文件的过程
### 1. 预处理
* 预编译过程主要处理那些源代码文件中以"#"开始的预编译指令.如#include, #define等
    * 将所有的#define删除,并且展开所有的宏定义
    * 处理所有的条件预编译指令,比如#if, #ifdef, #elif, #else, #endif.
    * 处理#include预编译指令,将被包含的文件插入到该预编译指令的位置
    * 删除所有注释 // 和 /**/
    * 添加行号和文件名标识,以便于编译器编译时产生调试用的行号信息及编译产生错误或警告时显示行号.
    * 保留所有的#pragma编译指令,因为编译器需要使用他们
```
命令: 预编译器cpp  产生文件: xx.i
$gcc -E hello.c -o hello.i
或者
$cpp hello.c > hello.i
```
### 2. 编译
* 编译过程就是把预处理器处理完的文件进行一系列词法分析,语法分析,语义分析及优化后生成相应的汇编代码文件
```
命令:汇编器ccl 产生文件:xx.s
$gcc -S hello.i -o hello.s
```
### 3. 汇编
* 汇编器是将汇编代码转变成机器可以执行的指令,每一个汇编语句几乎都对应一条机器指令.
```
命令: 汇编器as 产生文件:xx.o 目标文件
$gcc -c hello.s -o hello.o
或者
$as hello.s -o hello.o
```
### 4. 链接
* 将目标文件链接为可执行程序
```
命令: 连接器ld
$ ld 
```

## 17. 可执行程序加载过程
1. 通过加载器将可执行程序加载到内存,创建程序内存映像;
    * Linux程序可通过调用execve函数来调用加载器;
    * 加载器将可执行目标文件中的代码和数据从磁盘复制到内存中,然后通过跳转到程序的第一条指令或者入口点来运行该程序;这个将程序复制到内存并运行的过程叫做加载;
2. 在程序头部表的引导下,加载器将可执行文件的片复制到代码段和数据段.
3. 加载器跳转多程序的入口点,也就是_start函数(ctrl.o)的地址,调用系统启动函数__libc_start_main(libc.so),初始化执行环境,调用用户层的main函数,处理main函数的返回值,并且在需要的时候把控制权返回给内核;

### 程序内存分布
```
|----------------------   高地址
|    内核空间     
|----------------------  
|      栈                
|------                 ___ esp(栈指针)       栈向低地址增长
| (未使用栈空间) 
|----------------------   
|共享库内存映射区域
|----------------------
|  未使用的堆              
|
|------                 ___ brk              堆向高地址增长
|      堆     
|----------------------
|读写段(.data,.bss)
|----------------------                        由于.data段有对齐要求,所以
|只读代码段                                      代码段和数据段之间是有间隙的
|(.init,.text,.rodata)
|---------------------- 0x400000 
|              
|----------------------  低地址
```
* 代码段
    * .text 存储机器指令
* 数据段
    * .data 已初始化的全局和静态C变量,
    * .bss  未初始化的全局和静态C变量,以及所有被初始化为0的全局或静态变量;
        (在目标文件中这个节不占用实际的空间,它仅仅是一个占位符);
* 堆 
    * 动态分配的内存
* 栈
    * 局部变量

## 18. 函数栈帧及函数调用过程
* 栈保存了一个函数调用所需要的维护信息,称为堆栈帧或活动记录
* 堆栈帧一般包括如下几方面内容:
    * 函数的返回地址和参数
    * 临时变量:包括函数的非静态局部变量以及编译器自动生成的其他临时变量
    * 保存的上下文:包括在函数调用前后需要保持不变的寄存器;
```
-----|------------
  |  |   参数
  |  |
  |  |------------
  |  | 返回地址
  活 |------------  __ ebp
  动 | Old EBP
  记 |------------
  录 | 保存的寄存器
  |  |------------
  |  | 局部变量
  |  |------------
  |  | 其他数据
-----|------------ __ esp
```
* esp寄存器始终指向栈的顶部,同时也就是指向了当前函数的活动记录的顶部
* ebp寄存器指向了活动记录的一个固定位置,ebp寄存器又被称为帧指针;
### 函数调用
* 把所有或一部分参数压入栈中,如果有其他参数没有入栈,那么使用某些特定寄存器传递
* 把当前指令的下一条指令的地址压入栈中
* 跳转到函数体执行;
### 汇编
* push ebp;  // 把ebp压入栈中(称为old ebp)
* move ebp,esp; // ebp = esp (这时ebp指向栈顶,而此时栈顶就是old ebp)
* [可选] sub esp, XXX; // 在栈上分配XXX字节的临时空间
* [可选] push xxx; // 如有必要,保存名为xxx寄存器(可重复多个)
* call 
* [可选] pop xxx; // 如有必要,恢复保存名为xxx寄存器(可重复多个)
* mov esp,ebp; // 恢复ESP同时回收局部变量空间
* pop ebp; // 从栈中恢复保存的ebp的值
* ret  // 从栈中取得返回地址,并跳转到该位置

## 19. malloc 分配原理
* Linux提供两种堆空间分配方式:即两个系统调用完成：brk 和 mmap
* glibc的malloc函数处理用户请求的处理方式:
    * 小于128KB的请求,他会在现有的堆空间里面,按照分配算法分配一块空间并返回;利用brk函数将数据段(.data)的最高地址指针_edata往高地址推；
    * 大于128KB的请求,他会使用mmap函数分配一块匿名空间,然后在这个匿名空间中为用户分配空间;

## 20. 类型转换
### 1. 显示类型转换
* static_cast 任何具有明确定义的类型转换,只要不包含底层const,都可以使用;
* const_cast  只能改变运算对象的底层const;去掉const性质
    * 常常用于由函数重载的上下文中
* reinterpret_cast 通常为运算对象的位模式提供较底层次上的重新解释;
* dynamic_cast 

# <span id = "oop">面向对象基础</span>
> [回到目录](#root)
## 1. 面向对象三大特性
* 封装性：数据和代码捆绑在一起，避免外界干扰和不确定性访问。
* 继承性：让某种类型对象获得另一个类型对象的属性和方法。
* 多态性：同一事物表现出不同事物的能力，即向不同对象发送同一消息，不同的对象在接收时会产生不同的行为（重载实现编译时多态，虚函数实现运行时多态）。

## 2. public/protected/private的区别
* public     的变量和函数在类的内部外部都可以访问。
* protected  的变量和函数只能在类的内部和其派生类中访问。  类的用户不可访问
* private    的变量和函数只能在类内访问。
### 继承
* 某个类对其继承而来的成员的访问权限受到两个因素影响:
    1. 在基类中该成员的访问说明符
    2. 在派生类的派生列表中的访问说明符
* 派生访问说明符对于派生类的成员(及友元)是否访问其直接基类的成员没有什么影响;
对基类成员的访问权限只与基类中的访问说明符有关;
* public继承 
    * 派生类的成员或友元能访问基类中public,protected成员;
    * 派生类用户只能访问public成员;
    * 基类中的成员访问权限,在派生类中访问权限未改变;
* protected继承
    * 派生类的成员或友元能访问基类中public,protected成员;
    * 派生类用户不能访问基类中的成员;
    * 基类中public成员,在派生类中变成protected成员, protected-->private, private-->private;
* private继承
    * 派生类的成员或友元能访问基类中public,protected成员;
    * 派生类用户不能访问基类中的成员;
    * 基类中的成员,在派生类中都变成private;
* 默认情况下,使用class关键字定义的派生类是私有继承;而使用struct关键字定义的派生类是共有继承;
## 3. 函数重载
* 如果**同一作用域**内的几个函数名字形同但形参列表不同(数量或类型),称之为重载;返回值是不影响函数签名
* 一个拥有顶层const的形参无法和另一个没有顶层const的形参区分开来;
```
Record lookup(const Phone);
Record lookup(Phone); // 重复声明
```
* 如果形参是某种类型的指针或引用,则可通过区分其指向的是常量对象还是非常量对象,可实现函数重载
```
Record lookup(const Phone&);
Record lookup(Phone&);

Record lookup(const Account*);
Record lookup(Account*);
```
* 一旦在当前作用域中找到所需的函数名字,编译器就忽略掉外层作用域中同名实体;
### 实现原理
* 编译器在编译.cpp文件中当前使用的作用域里的同名函数时，根据函数形参的类型和顺序会对函数进行重命名；
```
void func(){}
int  func(int){return 0;}
void func(char){}
void func(int, char){}
void func(int&){}
void func(int&, int&, double&){}
void func(char*, int[], double*){}
// 使用 objdump -t 命令查看 
0000000000000000 g     F .text  0000000000000007 _Z4funcv
0000000000000007 g     F .text  000000000000000e _Z4funci
0000000000000015 g     F .text  000000000000000c _Z4funcc
0000000000000021 g     F .text  000000000000000f _Z4funcic
0000000000000030 g     F .text  000000000000000b _Z4funcRi
000000000000003b g     F .text  0000000000000013 _Z4funcRiS_Rd
000000000000004e g     F .text  0000000000000013 _Z4funcPcPiPd
```

## 4. 类对象内存
### 对象内存布局
> https://coolshell.cn/articles/12176.html
### 对象存储空间
* 非静态成员的数据类型大小之和。
* 编译器加入的额外成员变量（如指向虚函数表的指针）。
* 为了边缘对齐优化加入的panding。

## 5. C++空类有哪些成员函数?
* 首先，空类大小为1字节。
* 默认函数有：
    * 构造函数
    * 析构函数
    * 拷贝构造函数
    * 赋值运算符

## 6. 构造函数能否为虚函数，析构函数呢
* 析构函数：
    * 析构函数可以为虚函数，并且一般情况下基类析构函数要定义为虚函数。(只有多态性质的基类才需要虚析构函数)
    * 只有在基类析构函数定义为虚函数时，调用操作符delete销毁指向对象的基类指针时，才能准确调用派生类的析构函数（从该级向上按序调用虚函数），才能准确销毁数据。
    * 析构函数可以是纯虚函数，含有纯虚函数的类是抽象类，此时不能被实例化。但派生类中可以根据自身需求重新改写基类中的纯虚函数。
* 构造函数：
    * 构造函数不能定义为虚函数，不仅如此，构造函数中还不能调用虚函数。因为那样实际执行的是父类对应的函数，因为自己还没有构造好（构造顺序先基类再派生类）。

## 7. 构造函数调用顺序，析构函数呢
* 基类的构造函数：如果有多个基类，先调用纵向上最上层基类构造函数，如果横向继承了多个类，调用顺序为派生表从左到右顺序。
* 成员类对象的构造函数：如果类的变量中包含其他类（类的组合），需要在调用本类构造函数前先调用成员类对象的构造函数，调用顺序遵照在类中被声明的顺序。
* 派生类的构造函数。
* 析构函数与之相反。

## 9. 防止继承的发生
* 方法一:利用虚拟继承
```
template<typename T>
class MakeSealed
{
friend T;
private:
    MakeSealed() {}
    ~MakeSealed() {}
};

class SealedClass2 : virtual public MakeSealed<SealedClass2>
{
public: 
    SealedClass2() {}
    ~SealedClass2() {}
};
class Tr : public SealedClass2 // 错误:SealedClass2不能被继承
{
public:
    Tr() {}
    ~Tr() {}
};
// 尽管MakeSealed<SealedClass>的构造函数与析构函数都是私有的，但由于类SealedClass是它的友元类型，
// 因此在SealedClass中调用MakeSealed<SealedClass>的构造函数和析构函数就不会出错。
```
* 方法二: C++11 新标准提供防止继承的方法,在类名后跟一个关键字final
```
class Base final {}; // Base不能作为基类
```

## 10. 拷贝构造函数和赋值运算符重载的区别
* 拷贝构造函数是函数，赋值运算符是运算符重载。
* 拷贝构造函数会生成新的类对象，赋值运算符不能。
* 拷贝构造函数是直接构造一个新的类对象，所以在初始化对象前不需要检查源对象和新建对象是否相同；赋值运算符需要上述操作并提供两套不同的复制策略，另外赋值运算符中如果原来的对象有内存分配则需要先把内存释放掉。
* 形参传递是调用拷贝构造函数（调用的被赋值对象的拷贝构造函数），但并不是所有出现"="的地方都是使用赋值运算符，如下：
```
    Student s;
    Student s1 = s;    // 调用拷贝构造函数
    Student s2;
    s2 = s;    // 赋值运算符操作
```
注：类中有指针变量时要重写析构函数、拷贝构造函数和赋值运算符
* 拷贝初始化何时发生
    * 用=定义变量时
    * 将一个变量作为实参传递给一个非引用类型的形参
    * 从一个返回类型为非引用类型的函数返回一个对象
    * 用花括号列表初始化一个数组中的元素或一个聚合类中的成员
## 11. 虚函数和纯虚函数区别
* 虚函数: virtual void func();
* 纯虚函数: virtual void func() = 0;
* 虚函数是为了实现动态关联产生的(多态)，目的是通过基类类型的指针或引用指向不同对象时，自动调用相应的和基类同名的函数（使用同一种调用形式，既能调用派生类又能调用基类的同名函数）。虚函数需要在基类中加上virtual修饰符修饰，因为virtual会被隐式继承，所以子类中相同函数都是虚函数。当一个成员函数被声明为虚函数之后，其派生类中同名函数自动成为虚函数，在派生类中重新定义此函数时要求函数名、返回值类型、参数个数和类型全部与基类函数相同。
* 纯虚函数只是相当于一个接口名，但含有纯虚函数的类(抽象基类)不能够实例化。

## 12. 覆盖、重载和隐藏的区别
* 覆盖是派生类中重新定义的函数，其函数名、参数列表（个数、类型和顺序）、返回值类型和父类完全相同，只有函数体有区别。派生类虽然继承了基类的同名函数，但用派生类对象调用该函数时会根据对象类型调用相应的函数。覆盖只能发生在类的成员函数中。
* 隐藏是指派生类函数屏蔽了与其同名的函数，这里仅要求基类和派生类函数同名即可。其他状态同覆盖。可以说隐藏比覆盖涵盖的范围更宽泛，毕竟参数不加限定。
* 重载是具有相同函数名但参数列表不同（个数、类型或顺序）的两个函数（不关心返回值），当调用函数时根据传递的参数列表来确定具体调用哪个函数。重载可以是同一个类的成员函数也可以是类外函数。

## 13. 哪几种情况必须用到初始化成员列表
* 初始化一个const成员。
* 初始化一个reference成员。
* 调用一个基类的构造函数，而该函数有一组参数。
* 调用一个数据成员对象的构造函数，而该函数有一组参数。

## 14. 重载和函数模板的区别
* 重载需要多个函数，这些函数彼此之间函数名相同，但参数列表中参数数量和类型不同。在区分各个重载函数时我们并不关心函数体。
* 模板函数是一个通用函数，函数的类型和形参不直接指定而用虚拟类型来代表。但只适用于参个数相同而类型不同的函数。

## 15. this指针是什么
* this指针是类的指针，指向对象的首地址。
* this指针只能在成员函数中使用，在全局函数、静态成员函数中都不能用this。
* this指针只有在成员函数中才有定义，且存储位置会因编译器不同有不同存储位置。

## 16. 类模板是什么
* 用于解决多个功能相同、数据类型不同的类需要重复定义的问题。
* 在建立类时候使用template及任意类型标识符T，之后在建立类对象时，会指定实际的类型，这样才会是一个实际的对象。
* 类模板是对一批仅数据成员类型不同的类的抽象，只要为这一批类创建一个类模板，即给出一套程序代码，就可以用来生成具体的类。

## 17. 构造函数和析构函数调用时机
* 全局范围中的对象：构造函数在所有函数调用之前执行，在主函数执行完调用析构函数。
* 局部自动对象：建立对象时调用构造函数，函数结束时调用析构函数。
* 动态分配的对象：建立对象时调用构造函数，调用释放时调用析构函数。
* 静态局部变量对象：建立时调用一次构造函数，主函数结束时调用析构函数。

## 18. 什么是虚指针
* 虚指针或虚函数指针是虚函数的实现细节。
* 虚指针指向虚表结构。
> https://coolshell.cn/articles/12165.html  
> https://coolshell.cn/articles/12176.html

## 19. 虚继承
* 虚继承解决在继承链上含有相同基类时,在派生类中存有多份基类子对象;<菱形继承>
* 虚派生只影响从指定了虚基类的派生类中进一步派生出的类(第三层),它不影响派生类本身(第二层)
* 在虚派生中,虚基类是由最底层的派生类初始化的
* 构造函数的次序
```
class ZooAnimal {};
class Bear : virtual public ZooAnimal {};

class Charactor {};
class BookCharactor : public Charactor {};

class ToyAnimal {}; // 直接虚基类

class TeddyBear : public BookCharator ,
                  public Bear,
                  public virtual ToyAnimal 
{  /*               */};

// 调用构造函数的次序 : 先构造虚基类,然后按照声明顺序逐一构造其他非基类
ZooAnimal();     // Bear的虚基类
ToyAnimal();     // 直接虚基类
Charactor();     // 第一个非基类的间接基类
BookCharactor(); // 第一个直接非基类
Bear();          // 第二个直接非基类
TeddyBear();     // 最底层的派生类

// 析构顺序正好相反
```

# <span id = "stl">标准模板库</span>
> [回到目录](#root)

##  
* STL组成部分（容器、仿函数、配置器、配接器、迭代器、算法）
## vector实现
> 关于vector问的比较多,所以最好自己实现一个vector

[模拟Vector实现](https://github.com/LambdaRan/Interview/blob/master/code/myVector.cpp)

* 线性表，数组实现。
    * 支持随机访问。
    * 插入删除操作需要大量移动数据。
* 需要连续的物理存储空间。
* 每当大小不够时，重新分配内存（*2），并复制原内容。

迭代器失效
* 插入元素
    * 尾后插入：size < capacity时，首迭代器不失效尾迭代器失效（未重新分配空间）,size == capacity时，所有迭代器均失效（需要重新分配空间）。
    * 中间插入：size < capacity时，首迭代器不失效但插入元素之后所有迭代器失效，size == capacity时，所有迭代器均失效。
* 删除元素
    * 尾后删除：只有尾迭代失效。
    * 中间删除：删除位置之后所有迭代失效。

## string实现

可以参考酷壳中陈硕大牛的文章: [C++面试中STRING类的一种正确写法](https://coolshell.cn/articles/10478.html)   

[模拟String实现](https://github.com/LambdaRan/Interview/blob/master/code/myString.cpp)

## list实现
[模拟list实现](https://github.com/LambdaRan/Interview/blob/master/code/myList.cpp)

## map与set

很多面试官都会问道map与set底层实现原理红黑树,所以如果有时间自己实现一遍

## unordered_map与unordered_set


## 智能指针
shared_ptr是管理共享资源的利器，需要注意避免循环引用，通常的做法是ower持有child的shared_ptr,child持有指向owner的weak_ptr.
## 迭代器种类
> 迭代器(iterator)扮演STL中容器和算法粘合剂的作用;使得算法的实现不依赖与容器的具体类型;
#### std::iterator_traits   
实现原理: "内嵌类型"编程技巧 与 编译器的template参数推导   
能够获取到容器中所存储元素的类型,如果想使得自己设计的迭代器与STL算法兼容,自定义迭代器必须包含一下内嵌类型或直接继承自std::iterator   
[参考](https://en.cppreference.com/w/cpp/iterator/iterator)    
```
difference_type - 可用来标识迭代器间距离的有符号整数类型
value_type - 迭代器解除引用后所得到的值的类型。对于输出迭代器，该类型为 void 。
pointer - 指向被迭代类型（ value_type ）的指针
reference - 被迭代类型（ value_type ）的引用类型
iterator_category - 迭代器类别。必须是迭代器类别标签之一。
```
iterator_category(类别标签)的作用是:编译器利用类别标签来进行重载的决议,来选择更加匹配的函数实现底层操作;    
达到不同的迭代器选择不同的函数(实现方式);   
[参考](https://en.cppreference.com/w/cpp/iterator/iterator_tags)    
```
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
```
* 在迭代器中: 迭代器类别应该定义为 该迭代器所属各种类型中,最强化的那个;
* STL算法一个命名规则:以算法所能接受之最低阶迭代器类型,来为其迭代器类别参数命名;
### 5种迭代器
```
input_iterator_tag 对应 输入迭代器 (InputIterator) 。
output_iterator_tag 对应 输出迭代器 (OutputIterator) 。
forward_iterator_tag 对应 向前迭代器 (ForwardIterator) 。
bidirectional_iterator_tag 对应 双向迭代器 (BidirectionalIterator) 。
random_access_iterator_tag 对应 随机访问迭代器 (RandomAccessIterator) 。
```
## 其他

bind functional lambda type_traits

## 介绍C++11特性
* lambda表达式
* 智能指针:shared_ptr,unique_ptr,weak_ptr




p121：
    当一个对象被用作右值的时候，用的是对象的值（内容）
    当对象被用作左值的时候，用的是对象的身份（在内存中的位置）

右值引用：P471
    一个重要性质：只能绑定到一个将要销毁的对象。
    
    一个右值引用也不过是某个对象的另一个名字而已。
    不能将一个右值引用绑定到一个左值上；

    返回左值引用的函数，连同赋值、下标、解引用和前置递增/减运算符，都是返回左值的表达式例子。
    返回非引用类型的函数，连同算术、关系、位以及后置定增/减运算符，都生成右值。
        可以将一个const的左值引用或一个右值引用绑定到这类表达式上。

    变量是左值
    右值要么是字面常量，要么是在表达式求值过程中创建的临时对象。

    变量表达式也有左值/右值属性。变量表达式都是左值。
        因此，我们不能将一个右值引用绑定到一个右值引用类型的变量上。
        例如：
            int &&rr1 = 42;
            int &&rr2 = rr1; // 错误：表达式rr1是左值
    变量是左值，因此我们不能将一个右值引用直接绑定到一个变量上，即使这个变量是右值引用类型也不行。

P608
当一个函数参数是模板类型参数的一个普通（左值）引用时（即形如T&）
    只能传递给它一个左值
```
template<typename T> void f1(T&); // 实参必须是一个左值
// 对f1的调用使用实参所引用的类型作为模板参数类型
f1(i); // i是一个int；模板参数类型T为int
f1(ci); // ci是一个const int；模板参数T是const int
f1(5); // 错误：传递给一个&参数的实参必须是一个左值
```
如果一个函数参数的类型是const T&
    我们可以传递给它 一个对象（const或非const），一个临时对象或一个字面常量值。
    当函数参数本身是const时,T的类型推断的结果不会是一个const类型。
```
template<typename T> void f(const T &p); // 可以接收一个右值
// f2中的参数类型是const &,实参中的const是无关的
// 在每个调用中,f2的函数参数都被推断为 const int&
f2(i); // i是一个int；模板参数类型T为int
f2(ci); // ci是一个const int；模板参数T是int
f2(5); // 一个const &可以绑定到一个右值；T是int
```
当一个函数参数是模板类型参数的一个右值引用时（即形如T&&）
    //可以传递给它一个右值
    任意类型实参
    template<typename T> void f3(T &&p);
    f3(42); // 实参是一个int类型的右值；模板参数T是int

我们不能将一个右值引用绑定到一个左值上，两个例外情况：
    1.影响将一个右值引用参数的推断如何进行。
        编译器推断模板类型参数为实参的左值引用类型。
        例如：
            template<typename T> void f(T &&p);
            int value = 32；
            f(value); // 此时 T为 int&

    1.如果我们间接创建一个引用的引用，则这些引用形成了“折叠”；
        X& &、X& &&和X&& &都折叠成类型X&
        类型X&& &&折叠成X&&
```
template<typename T> void f3(T &&p);
f3(i); // 实参是一个左值；模板参数T是一个int&
f3(ci); // 实参是一个左值；模板参数T是一个const int&
```
如果一个函数参数是一个指向模板类型参数的右值引用（如，T&&），则他可以被绑定到一个左值，且
如果实参是一个左值，则推断出的模板实参类型将是一个左值引用，且函数函数参数将被实例化为一个（普通）左值引用参数（T&）

右值引用通常用于两种情况：
    模板转发其实参
    模板被重载
```
P610
/***************************************************************************/
//
// Exercise 16.42:
// Determine the type of T and of val in each of the following calls:
//     template <typename T> void g(T&& val);
//     int i = 0; const int ci = i;
//     (a) g(i);
//  since i is lvalue, T is deduced as int&, val is int& && collapsing to int&
//     (b) g(ci);
//  since ci is lvaue, T is deduced as const int&, val is const int& && collapsing to const int&
//     (c) g(i * ci);
//  since i * ci is rvalue, T is deduced as int, val is int&& && colapsing to int&&
//
// Exercise 16.43:
// Using the function defined in the previous exercise, what would the template
// parameter of g be if we called g(i = ci)?
//      (i = ci) returns lvalue refering to the object i.
//      Hence T is deduced as int& val is int& && .
//      any change on val changes the object i.
//
// Exercise 16.44:
// Using the same three calls as in the first exercise, determine the types for T
// if g’s function parameter is declared as T (not T&&).
//                                           ^
//      g(i);       --  T is deduced as int
//      g(ci);      --  T is deduced as int, const is ignored.
//      g(i * ci);  --  T is deduced as int, (i * ci) returns rvalue which is copied to
//                      T
// What if g’s function parameter is const T&?
//                                    ^^^^^^^^
//      g(i)        --  T is deduced as int  , val : const int&
//      g(ci)       --  T is deduced as int  , val : const int&
//      g(i * ci)   --  T is deduced as int  , val : const int&(see example on page 687)
//
// Exercise 16.45:
// Given the following template, explain what happens if we call g on a literal value
// such as 42. What if we call g on a variable of type int?
//     template <typename T> void g(T&& val) { vector<T> v; }
//
// Discussion on SO:
//  http://stackoverflow.com/questions/21624016/when-a-lvalue-is-passed-to-t-what-will-happen
//
//     relevant section from textbook:
// When we pass an lvalue (e.g., i) to a function parameter that is an rvalue reference to a
// template type parameter (e.g, T&&), the compiler deduces the template type parameter as the
// argument’s lvalue reference type. So, when we call f3(i), the compiler deduces the type of
// T as int&, not int.
//         --  P.688
//
// In this case, when calling on a literal value, say 42. int&& && will collapse to int&&. At last
// T is deduced as int. Hence std::vector<T> is instantiated as std::vector<int> which is legal.
//
// When calling on a variable int. T will be deduced as int&. int & && will collapse to int&.
// std::vector<int&> is not legal. The reason why int& can't be element of a vector can be found at:
//  http://stackoverflow.com/questions/922360/why-cant-i-make-a-vector-of-references
//
```

P613
如果一个函数参数是指向模板类型参数的右值引用，它对应的实参的const属性和左值/右值属性将得到保持。

当用于一个指向模板参数类型的右值引用函数参数（T&&）时，forward会保持实参类型的所有细节

// 引用折叠
int& forward<int&>(int&);
int& forward<int&>(int&&);

int&& forward<int>(int&);
int&& forward<int>(int&&);

