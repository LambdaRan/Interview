#ifndef RDG_SHAREDPTR_H
#define RDG_SHAREDPTR_H

#include <atomic>
#include <functional>
#include <memory>
//#include <cstddef> // nullptr_t

template<typename _T>
class cow_ptr;

namespace Detail
{
template<typename T>
struct _default_delete
{
    void operator()(T* ptr) { if (ptr) delete ptr; }
};

template<typename T>
struct ref_t 
{
    using deleter_type = std::function<void(T*)>;
    
    T* data_;
    std::atomic<size_t> ncount_;
    deleter_type deleter_;

    explicit ref_t(T* p = nullptr, deleter_type pfunc = deleter_type(_default_delete<T>()))
        : data_(p),
          ncount_(0),
          deleter_(pfunc)
    {
        if (data_) ncount_ = 1;
    }

    // 拷贝构造函数
    ref_t(const ref_t&) = delete;
    ref_t& operator=(const ref_t&) = delete;

    ~ref_t()
    {
        if (--ncount_ == 0)
        {
            deleter_(data_);
            data_ = nullptr;
        }
    }

    size_t count() const { return ncount_.load(); }
    T* get_data() const { return data_; }

    ref_t& operator++()
    {
        ++ncount_;
        return *this;
    }
    ref_t operator++(int)
    {
        auto t = *this;
        ++ncount_;
        return t;
    }
    ref_t& operator--()
    {
        --ncount_;
        return *this;
    }
    ref_t operator--(int)
    {
        auto t = *this;
        --ncount_;
        return t;
    }
}; // truct ref_t

template<typename T>
bool operator==(const ref_t<T>& lhs, const ref_t<T>& rhs)
{
    return lhs.get_data() == rhs.get_data();
}
template<typename T>
bool operator!=(const ref_t<T>& lhs, const ref_t<T>& rhs)
{
    return !(lhs == rhs);
}

} // namespace Detail

template<typename T>
class SharedPtr
{
public: 
    typedef T element_type;
private: 
    template<typename Type>
    using ref_t = Detail::ref_t<Type>;
public: 
    explicit SharedPtr(T* p = nullptr)
        : ref_(new ref_t<T>(p))
    {}
    template<typename D>
    SharedPtr(T* p, D del)
        : ref_(new ref_t<T>(p, del))
    {}
    // 拷贝构造函数
    SharedPtr(const SharedPtr& rhs)
    {
        _copy_ref(rhs.ref_);
    }
    SharedPtr& operator=(const SharedPtr& rhs)
    {
        if (this != &rhs)
        {
            _decrease_ref();
            _copy_ref(rhs.ref_);
        }
        return *this;
    }
    ~SharedPtr() { _decrease_ref(); }

    element_type& operator*() { return *(get()); }
    element_type& operator*() const { return *(get()); }
    element_type* operator->() { return &(operator*()); }
    element_type* operator->() const { return &(operator*()); }

    element_type* get() { return ref_->get_data(); }
    element_type* get() const { return ref_->get_data(); }
    size_t use_count() const { return ref_->count(); }

    operator bool() const { return get() != nullptr; }
private: 
    void _decrease_ref()
    {
        if (ref_->get_data())
        {
            --(*ref_);
            if (use_count() == 0)
                delete ref_;
        }
    }
    void _copy_ref(ref_t<T> *ref)
    {
        ref_ = ref;
        ++(*ref_);
    }
private:
    ref_t<T>* ref_;
public: 
    template<typename _T>
    friend class cow_ptr;
};
                                                                                                                                                                     
template<typename T1, typename T2>
bool operator== (const SharedPtr<T1>& lhs, const SharedPtr<T2>& rhs)
{
    return lhs.get() == rhs.get();
}
template<typename T>
bool operator== (const SharedPtr<T>& lhs, std::nullptr_t p)
{
    return lhs.get() == p;
}
template<typename T>
bool operator== (std::nullptr_t p, const SharedPtr<T>& sp){
    return sp == p;
}
template<typename T1, typename T2>
bool operator!= (const SharedPtr<T1>& lhs, const SharedPtr<T2>& rhs)
{
    return !(lhs == rhs);
}
template<typename T>
bool operator!= (const SharedPtr<T>& lhs, std::nullptr_t p)
{
    return !(lhs == p);
}
template<typename T>
bool operator!= (std::nullptr_t p, const SharedPtr<T>& rhs){
    return !(rhs == p);
}

template<typename T, typename...Args>
SharedPtr<T> makeShared(Args... args)
{
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

#endif // !RDG_SHAREDPTR_H
