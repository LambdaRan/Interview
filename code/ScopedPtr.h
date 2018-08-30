
#ifndef RDG_SCOPEDPTR_H
#define RDG_SCOPEDPTR_H

template<typename T>
class ScopedPtr
{
public: 
    ScopedPtr(T* ptr = nullptr);
    ~ScopedPtr();
    T& operator*();
    T& operator*() const;
    T* operator->();
    T* operator->() const;
private: 
    ScopedPtr(const ScopedPtr<T>& rhs);
    ScopedPtr<T>& operator=(const ScopedPtr<T>& rhs);
    T* ptr_;
};

template<typename T>
ScopedPtr<T>::ScopedPtr(T* ptr)
    : ptr_(ptr)
{}

template<typename T>
ScopedPtr<T>::~ScopedPtr()
{
    if (ptr_)
    {
        delete ptr_;
        ptr_ = nullptr;
    }
}

template<typename T>
T& ScopedPtr<T>::operator*()
{
    return *ptr_;
}

template<typename T>
T& ScopedPtr<T>::operator*() const 
{
    return *ptr_;
}

template<typename T>
T* ScopedPtr<T>::operator->()
{
    return &(operator*());
}

template<typename T>
T* ScopedPtr<T>::operator->() const 
{
    return &(operator*());
}
#endif // !RDG_SCOPEDPTR_H
