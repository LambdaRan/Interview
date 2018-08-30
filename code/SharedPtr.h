#ifndef RDG_SHAREDPTR_H
#define RDG_SHAREDPTR_H

#include <atomic>
#include <functional>
#include <memory>

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


};


#endif // !RDG_SHAREDPTR_H
