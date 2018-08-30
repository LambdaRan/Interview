#ifndef RDG_UNIQUEPTR_H
#define RDG_UNIQUEPTR_H

#include <cstddef>
#include <algorithm>

template<typename T>
struct default_delete
{
    void operator()(T* ptr) { if (ptr) delete ptr; }    
};

template<typename T>
struct default_delete<T[]>
{
    void operator()(T* ptr) { if (ptr) delete[] ptr; }
};

template<typename T, typename D = default_delete<T>>
class UniquePtr
{
public: 
    typedef T               element_type;
    typedef D               deleter_type;
    typedef element_type*   pointer;
public: 
    explicit UniquePtr(T* data = nullptr)
        : data_(data)
    {}
    UniquePtr(T* data, deleter_type del)
        : data_(data), deleter_(del)
    {}

    UniquePtr(UniquePtr&& rhs)
        : data_(nullptr)
    {
        //using std::swap;
        std::swap(data_, rhs.data_);
    }

    UniquePtr& operator=(UniquePtr&& rhs)
    {
        if (*this != rhs)
        {
            _clean();
            std::swap(data_, rhs.data_);
        }
        return *this;
    }

    UniquePtr(const UniquePtr& rhs) = delete;
    UniquePtr& operator=(const UniquePtr& rhs) = delete;

    ~UniquePtr() { _clean(); }

    pointer get() const { return data_; }
    pointer get() { return data_; }
    deleter_type& get_deleter() { return deleter_; }
    deleter_type& get_deleter() const { return deleter_; }

    operator bool() const { return get() != nullptr; }
    pointer release() 
    {
        //using std::swap;
        element_type* p = nullptr;
        std::swap(data_, p);
        return p;
    }
    void reset(pointer p = pointer())
    {
        _clean();
        data_ = p;
    }
    void swap(UniquePtr& rhs)
    {
        std::swap(data_, rhs.data_);
    }

    element_type& operator*() { return *data_; }
    element_type& operator*() const { return *data_; }
    pointer operator->() { return &(operator*());}
    pointer operator->() const { return &(operator*()); }
private: 
    void _clean()
    {
        deleter_(data_);
        data_ = nullptr;
    }
private: 
    pointer data_;
    deleter_type deleter_;
};

template<typename T, typename D>
void swap(UniquePtr<T,D>& lhs, UniquePtr<T,D>& rhs)
{
    lhs.swap(rhs);
}
template<typename T1, typename D1, typename T2, typename D2>
bool operator==(const UniquePtr<T1, D1>& lhs, const UniquePtr<T2, D2>& rhs)
{
    return lhs.get() == rhs.get();
}
template <class T, class D>
bool operator== (const UniquePtr<T, D>& lhs, std::nullptr_t p)
{
    return lhs.get() == p;
}
template <class T, class D>
bool operator== (std::nullptr_t p, const UniquePtr<T, D>& rhs)
{
    return rhs.get() == p;
}

template<typename T1, typename D1, typename T2, typename D2>
bool operator!=(const UniquePtr<T1, D1>& lhs, const UniquePtr<T2, D2>& rhs)
{
    return !(lhs == rhs);
}
template <class T, class D>
bool operator != (const UniquePtr<T, D>& lhs, std::nullptr_t p)
{
    return lhs.get() != p;
}
template <class T, class D>
bool operator != (std::nullptr_t p, const UniquePtr<T, D>& rhs)
{
    return rhs.get() != p;
}

template <class T, class... Args>
UniquePtr<T> make_unique(Args&&... args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
};

#endif // !RDG_UNIQUEPTR_H
