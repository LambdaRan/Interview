
// #ifndef RDG_COWPTR_H
// #define RDG_COWPTR_H

// #include "SharedPtr.h"

// template<typename T>
// class cow_ptr;

// namespace Detail
// {
// template<typename T>    
// class proxy
// {
// public: 
//     explicit proxy(cow_ptr<T>* const cwp)
//         : cwp_(cwp)
//     {}
//     proxy(const proxy<T>&) = default;
//     proxy& operator=(const proxy<T>&) = default;

//     T& operator*();
//     T& operator*() const;
//     T* operator->();
//     T* operator->() const;

//     cow_ptr<T>& operator=(const T& val);
//     operator T() const;
// private: 
//     cow_ptr<T>* cwp_;
// }; // class proxy
// } // Detail

// template<typename T>
// class cow_ptr
// {
// public: 
//     typedef T element_type;
// private: 
//     template<typename _T>
//     using proxy = Detail::proxy<_T>;
// public: 
//     explicit cow_ptr(T *p = nullptr);
//     template<typename D>
//     cow_ptr(T *p, D d);

//     cow_ptr(const cow_ptr& cp);
//     cow_ptr& operator= (const cow_ptr& cp);
    
//     proxy<element_type> operator*();
//     proxy<element_type> operator*() const;
//     proxy<element_type> operator->();
//     proxy<element_type> operator->() const;

//     element_type* get();
//     element_type* get() const;

//     operator bool() const;

// private: 
//     SharedPtr<element_type> ptr_;

// public:
//     template<typename T1, typename T2>
//     friend bool operator== (const cow_ptr<T1>& cp1, const cow_ptr<T2>& cp2);
//     template<typename T1>
//     friend bool operator== (const cow_ptr<T1>& cp, std::nullptr_t p);
//     template<typename T1>
//     friend bool operator== (std::nullptr_t p, const cow_ptr<T1>& cp);

//     template<typename T1, typename T2>
//     friend bool operator!= (const cow_ptr<T1>& cp1, const cow_ptr<T2>& cp2);
//     template<typename T1>
//     friend bool operator!= (const cow_ptr<T>& cp, std::nullptr_t p);
//     template<typename T1>
//     friend bool operator!= (std::nullptr_t p, const cow_ptr<T>& cp);

//     template<typename _T>
//     friend class Detail::proxy;
// }; // class cow_ptr
// template<typename T, typename... Args>
// cow_ptr<T> make_cow(Args...args)
// {
//     return cow_ptr<T>(new T(std::forward<Args>(args)...));
// }


// // impl
// namespace Detail
// {
//     template<typename T>
//     T& proxy<T>::operator*()
//     {
//         auto t = *(cwp_->ptr_);
//         cwp_->ptr_ = makeShared<T>(t);
//         return *(cwp_->ptr_);
//     }
//     template<typename T>
//     T& proxy<T>::operator*() const { return *(cwp_->ptr_); }

//     template<typename T>
//     T* proxy<T>::operator->()
//     {
//         auto t = *(cwp_->ptr_);
//         cwp_->ptr_ = makeShared<T>(t);
//         return cwp_->ptr_.operator->();
//     }
//     template<typename T>
//     T* proxy<T>::operator->() const { return cwp_->ptr_.operator->(); }

//     template<typename T>
//     cow_ptr<T>& proxy<T>::operator= (const T& val)
//     {
//         cwp_->ptr_ = makeShared<T>(val);
//         return *cwp_;
//     }
//     template<typename T>
//     proxy<T>::operator T() const { return *(cwp_->ptr_); }
// } // namespace Detail


// ///// impl cow_ptr

// template<typename T>
// cow_ptr<T>::cow_ptr(T *p) : ptr_(p) {}

// template<typename T>
// template<typename D>
// cow_ptr<T>::cow_ptr(T *p, D d) : ptr_(p, d){}

// template<typename T>
// cow_ptr<T>::cow_ptr(const cow_ptr& cp) { ptr_ = cp.ptr_; }

// template<typename T>
// cow_ptr<T>& cow_ptr<T>::operator = (const cow_ptr& cp)
// {
//     if (this != &cp){
//         ptr_._decrease_ref();
//         ptr_ = cp.ptr_;
//     }
//     return *this;
// }

// template<typename T>
// typename cow_ptr<T>::element_type* cow_ptr<T>::get() { return ptr_.get(); }
// template<typename T>
// typename cow_ptr<T>::element_type* cow_ptr<T>::get() const { return ptr_.get(); }

// template<typename T>
// cow_ptr<T>::operator bool() const { return ptr_ != nullptr; }

// template<typename T>
// cow_ptr<T>::proxy<T> cow_ptr<T>::operator*() { return proxy<T>(this); }
// template<typename T>
// cow_ptr<T>::proxy<T> cow_ptr<T>::operator*() const
// {
//     return proxy<T>(const_cast<cow_ptr *const>(this)); 
// }

// template<typename T>
// cow_ptr<T>::proxy<T> cow_ptr<T>::operator->() { return proxy<T>(this); }
// template<typename T>
// cow_ptr<T>::proxy<T> cow_ptr<T>::operator->() const
// { 
//     return proxy<T>(const_cast<cow_ptr *const>(this)); 
// }

// template<typename T1, typename T2>
// bool operator== (const cow_ptr<T1>& cp1, const cow_ptr<T2>& cp2)
// {
//     return cp1.ptr_ == cp2.ptr_;
// }
// template<typename T>
// bool operator == (const cow_ptr<T>& cp, std::nullptr_t p)
// {
//     return cp.ptr_ == p;
// }
// template<typename T>
// bool operator == (std::nullptr_t p, const cow_ptr<T>& cp)
// {
//     return cp == p;
// }
// template<typename T1, typename T2>
// bool operator!= (const cow_ptr<T1>& cp1, const cow_ptr<T2>& cp2)
// {
//     return !(cp1 == cp2);
// }
// template<typename T>
// bool operator!= (const cow_ptr<T>& cp, std::nullptr_t p)
// {
//     return !(cp == p);
// }
// template<typename T>
// bool operator != (std::nullptr_t p, const cow_ptr<T>& cp)
// {
//     return !(cp == p);
// }

// #endif // !RDG_COWPTR_H
