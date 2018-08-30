
#ifndef RDG_SHAREDPTR_H
#define RDG_SHAREDPTR_H

#include <algorithm>

template<typename T>
struct default_delete
{
    void operator()(T* ptr) { if (ptr) delete ptr; }
};

template<typename T, typename D = default_delete<T>>
class SharedPtr
{
public: 
    SharedPtr(T* ptr = nullptr);
    SharedPtr(T* ptr, D del);
    SharedPtr(const SharedPtr<T, D>& rhs);
    SharedPtr<T, D>& operator=(SharedPtr<T, D> rhs);
    ~SharedPtr();

    T& operator*() { return *ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() { return &(operator*()); }
    T* operator->() const { return &(operator*()); }

    long use_count() const { return *count_; }
    T* get() const { return ptr_;}

private: 
    T* ptr_;
    long* count_;
    D deleter_;
};

template<typename T, typename D>
SharedPtr<T, D>::SharedPtr(T* ptr)
    : ptr_(ptr),
    count_(new long(0))
{
    if (ptr_) ++(*count_);
}

template<typename T, typename D>
SharedPtr<T, D>::SharedPtr(T* ptr, D del)
    : ptr_(ptr),
      count_(new long(0)),
      deleter_(del)
{
    if (ptr_) ++(*count_);
}

template<typename T, typename D>
SharedPtr<T, D>::SharedPtr(const SharedPtr<T, D>& rhs)
    : ptr_(rhs.ptr_),
      count_(rhs.count_),
      deleter_(rhs.deleter_)
{
    ++(*count_);
}

template<typename T, typename D>
SharedPtr<T, D>& SharedPtr<T, D>::operator=(SharedPtr<T, D> rhs)
{
    std::swap(ptr_, rhs.ptr_);
    std::swap(count_, rhs.count_);
    std::swap(deleter_, rhs.deleter_);
    return *this;
}

template<typename T, typename D>
SharedPtr<T, D>::~SharedPtr()
{
    if (--(*count_) == 0)
    {
        deleter_(ptr_);
        delete count_;
        ptr_ = nullptr;
        count_ = nullptr;
    }
}

#endif // !RDG_SHAREDPTR_H
