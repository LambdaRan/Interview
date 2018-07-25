

#include <iostream>
#include <assert.h>
#include <algorithm>

using namespace std;

template<typename T>
class Vector {
public:
    Vector()
        : array_(NULL), size_(0), capacity_(0)
    {}
    ~Vector() 
    {
        delete[] array_;
        array_ = NULL;
        size_ = 0;
        capacity_ = 0;
    }
    Vector(const T& t, size_t n)
        : array_(new T[size]), size_(size), capacity_(size)
    {
        
    }
    // 拷贝构造函数
    Vector(const Vector<T>& other);
    // 拷贝赋值运算符
    Vector<T>& operator=(const Vector<T>& other);
    T& operator[](size_t position);
    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    void clear();
    void push_back(const T& t);
    void insert(size_t pos, const T& t);
    T pop_back();
    T front() const;
    T back() const;
    void erase(size_t pos);
private:
    T* allocator(size_t size);
    void destory(T* array);
private:
    T *array_;
    size_t size_;
    size_t capacity_;
};