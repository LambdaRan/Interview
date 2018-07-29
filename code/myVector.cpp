

#include <iostream>
#include <assert.h>

#include <memory>
#include <algorithm>
#include <utility>

using namespace std;

template<typename T>
class Vector {
public:
    typedef T           value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef value_type& reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
public:
    Vector()
        : start_(0), finish_(0), end_of_storage_(0)
    {}
    Vector(size_type n, const T& value)
    {
        fill_initialize(n, value);
    }
    explicit Vector(size_type n)
    {
        fill_initialize(n, T());
    }
    // 拷贝构造函数
    Vector(const Vector<T>& rhs)
    {
        start_ = allocator_.allocate(rhs.size());
        uninitialized_copy(rhs.begin(), rhs.end(), start_);
        finish_ = start_ + rhs.size();
        end_of_storage_ = finish_;
    }
    // 拷贝赋值运算符 
    Vector<T>& operator=(Vector<T> rhs)
    {
        swap(rhs);
        return *this;
    }

    ~Vector()
    {
        destroy(start_, finish_);
        deallocate();
        //std::cout << "~Vector()" << std::endl;
    }

    iterator begin(){ return start_; }
    iterator end() { return finish_; }
    iterator begin() const { return start_; }
    iterator end() const { return finish_; }
    reference front() { return *begin(); }
    reference back() { return *(end() - 1); }

    size_type size() const { return size_type(end() - begin()); }
    size_type capacity() const { return size_type(end_of_storage_ - begin()); }
    bool empty() const { return end() == begin(); }
    reference operator[](size_type n) {return *(begin() + n); }

    void push_back(const T& x)
    {
        // 有存储空间
        if (finish_ != end_of_storage_)
        {
            allocator_.construct(finish_, x);
            ++finish_;
        } // 没有足够的空间
        else  
        {
            insert_aux(end(), x);
        }
    }
    void pop_back()
    {
        if (finish_ != start_)
        {
            --finish_;
            allocator_.destroy(finish_);
        }
    }

    iterator erase(iterator position)
    {
        if (position + 1 != end())
            std::copy(position+1, end(), position);// 后续元素前移
        --finish_;
        allocator_.destroy(finish_);
        return position;
    }
    iterator erase(iterator first, iterator last)
    {
        if (first == last)
        {
            return erase(first);
        }
        else  
        {
            size_type erase_size = last - first;
            while(erase_size--)
                allocator_.destroy(--finish_);
        }
        return first;
    }
    iterator insert(iterator position, const T& value)
    {
        size_type diff = position - begin();
        insert(position, 1, value);
        return begin()+diff;
    }
    void insert(iterator position, size_type n, const T& x);
    void resize(size_type new_size, const T& x)
    {
        if (new_size < size())
            erase(begin()+new_size, end());
        else  
            insert(end(), new_size-size(), x);
    }
    void resize(size_type n) { resize(n, T()); }

    void clear() { erase(begin(), end()); }
private:
    void insert_aux(iterator position, const T& value);
    //void insert(iterator position, size_type n, const T& x);
    void destroy(iterator first, iterator last)
    {
        size_type erase_size = last - first;
        while (erase_size--)
            allocator_.destroy(--finish_);
        
    }
    void deallocate()
    {
        if (start_)
        {
            allocator_.deallocate(start_, end_of_storage_ - start_);
        }
    }
    // 配置空间并塞满内容
    iterator allocate_and_fill(size_type n, const T& x)
    {
        iterator result = allocator_.allocate(n);
        uninitialized_fill_n(result, n, x);
        return result;
    }
    void fill_initialize(size_type n, const T& value)
    {
        start_ = allocate_and_fill(n, value);
        finish_ = start_ + n;
        end_of_storage_ = finish_;
    }
    void swap(Vector<T>& rhs)
    {
        using std::swap;
        swap(start_, rhs.start_);
        swap(finish_, rhs.finish_);
        swap(end_of_storage_, rhs.end_of_storage_);
    }
private:
    iterator start_; // 表示目前使用空间的头
    iterator finish_; // 表示目前使用空间的尾
    iterator end_of_storage_; // 表示目前可用空间的尾
    // 空间分配器
    std::allocator<value_type> allocator_;
};

template<typename T>
void Vector<T>::insert_aux(iterator position, const T& x)
{
    if (finish_ != end_of_storage_)
    {
        allocator_.construct(finish_, *(finish_-1));
        ++finish_;
        std::copy_backward(position, finish_-2, finish_-1);
        T x_copy = x;
        *position = x_copy;
    }
    else  // 已无备用空间,重新分配新空间
    {
        const size_type old_size = size();
        const size_type new_size = old_size == 0 ? 1 : 2 * old_size;
        iterator new_start = allocator_.allocate(new_size);
        iterator new_finish = new_start;

        // 将原数据拷贝到新空间中
        new_finish = uninitialized_copy(begin(), position, new_finish);
        allocator_.construct(new_finish, x);
        ++new_finish;
        new_finish = uninitialized_copy(position, end(), new_finish);

        // 析构并释放原vector
        //destroy(begin(), end());
        destroy(start_, finish_);
        deallocate();
        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = new_start + new_size;
    }
}
template<typename T>
void Vector<T>::insert(iterator position, size_type n, const T& x)
{
    if (n != 0) // 当n不等于0时才插入
    {
        if (size_type(end_of_storage_-finish_) >= n)
        { 
            // 备用空间大小大于等于新增元素的大小
            T x_copy = x;
            const size_type elems_after = finish_ - position;
            iterator old_finish = finish_;
            if (elems_after > n) 
            {
                // 插入点之后的现有元素个数 大于 新增元素个数
                //        |     n      | 
                // _______|___a__|______|____b___|______>n______
                //        p     f-n    p+n       f 
                // a区间大小等于b区间的大小
                uninitialized_copy(finish_-n, finish_, finish_);
                finish_ += n;
                // 将a区元素移到b区
                std::copy_backward(position, old_finish-n, old_finish);
                std::fill(position, position+n, x_copy); // 从插入点开始填入新值
            }
            else 
            {
                // 插入点之后的现有元素个数 小于等于 新增元素个数
                //        |        n      | 
                // _______|__________|____|_______>n_________
                //        p          f    
                // 
                uninitialized_fill_n(finish_, n-elems_after, x_copy); 
                finish_ += n - elems_after;
                uninitialized_copy(position, old_finish, finish_); 
                finish_ += elems_after;
                std::fill(position, old_finish, x_copy);
            }
        }
        else // 备用空间大小小于新增元素的大小, 重新分配空间 
        {
            // 新长度: 就长度的两倍 或 旧长度加新增元素大小
            const size_type old_size = size();
            const size_type new_size = old_size + std::max(old_size, n);
            // 申请新长度
            iterator new_start = allocator_.allocate(new_size);
            iterator new_finish = new_start;

            // 将旧空间插入点之前的元素移到新空间中
            new_finish = uninitialized_copy(begin(), position, new_finish);
            // 将新元素填入空间
            new_finish = uninitialized_fill_n(new_finish, n, x);
            // 将旧空间插入点之后的元素移到新空间
            new_finish = uninitialized_copy(position, end(), new_finish);

            // 析构并释放原vector
            destroy(start_, finish_);
            deallocate();
            start_ = new_start;
            finish_ = new_finish;
            end_of_storage_ = new_start + new_size; 
        }     
    }
}

void print_vec(const Vector<int>& vec)
{
    for (auto x: vec) {
         std::cout << ' ' << x;
    }
    std::cout << '\n';
}
int main()
{
    std::cout << "Test my Vector " << std::endl;
    Vector<int> vec;

    vec.push_back(1);
    std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << "\n";
    vec.push_back(2);
    std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << "\n";
    vec.push_back(3);
    std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << "\n";
    vec.push_back(4);
    std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << "\n";
    vec.push_back(5);
    std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << "\n";

    std::cout << "1  --> front: " << vec.front() << " *begin():" << *vec.begin() << "\n";
    std::cout << "5  --> back: " << vec.back() << " *(end()-1):" << *(vec.end()-1) << "\n";
    std::cout << "[2]=3 --> " << vec[2] << "\n";

    vec.pop_back();
    std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << "\n";

    print_vec(vec);
    vec.insert(vec.begin(), 3, 3);
    print_vec(vec);
    std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << "\n";

    vec.insert(vec.end(), 2);
    print_vec(vec);
    std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << "\n";

    vec.resize(4);
    std::cout << "resize(4) --> size: " << vec.size() << " capacity: " << vec.capacity() << "\n";
    
    std::cout << "测试拷贝构造函数:" << "\n";
    Vector<int> vec2(vec);
    std::cout << "vec2.size: " << vec2.size() << " vec2.capacity: " << vec2.capacity() << "\n"; 

    std::cout << "测试拷贝赋值运算符:" << "\n";
    Vector<int> vec3;
    vec3 = vec2;
    std::cout << "vec3.size: " << vec3.size() << " vec3.capacity: " << vec3.capacity() << "\n"; 

    vec.clear();
    std::cout << "vec.clear() --> size: " << vec.size() << " capacity: " << vec.capacity() << "\n";    
    return 0;
}