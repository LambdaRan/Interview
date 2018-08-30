
#ifndef RDG_SCOPEDARRAY_H
#define RDG_SCOPEDARRAY_H

#include <assert.h>
#include <stddef.h>

template<typename T>
class ScopedArray
{
public: 
    ScopedArray(T* ptr_ = nullptr);
    ~ScopedArray();
    T& operator[](size_t index);
private: 
    ScopedArray(const ScopedArray<T>& rhs);
    ScopedArray<T>& operator=(const ScopedArray<T>& rhs);
    T* ptr_;
};
template<typename T>
ScopedArray<T>::ScopedArray(T* ptr)
    : ptr_(ptr)
{}

template<typename T>
ScopedArray<T>::~ScopedArray() 
{
    if (ptr_)
    {
        delete[] ptr_;
        ptr_ = nullptr;
    }
}
template<typename T>
T& ScopedArray<T>::operator[](size_t index)
{
    assert(index >= 0);
    return ptr_[index];
}
#endif // !RDG_SCOPEDARRAY_H
