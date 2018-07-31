

#include <iostream>

#include <algorithm>
#include <memory>
#include <utility>
#include <iterator>
#include <initializer_list>
#include <cstddef>


template<typename T>
struct _list_node {
    explicit _list_node(const T& x) : prev_(0), next_(0), data_(x) {}
    //typedef void* void_pointer;
    typedef _list_node<T>* void_pointer;
    void_pointer prev_;
    void_pointer next_;
    T data_;
};

template<typename T>
struct _list_iterator :  public std::iterator<std::bidirectional_iterator_tag, T>
{ 
    // 这里有一个问题:如果不继承std::iterator,自己定义类型标签时,不能识别所属的迭代器类型标签
    typedef _list_iterator<T> iterator;
    typedef _list_iterator<T> self;

    //typedef std::bidirectional_iterator_tag iterator_category;
    typedef T               value_type;
    typedef value_type*     pointer;
    typedef value_type&     reference;
    typedef _list_node<T>*  link_type;
    typedef size_t          size_type;
    //typedef ptrdiff_t       defference_type;

    link_type node_; // 迭代器内部普通指针,指向list的节点

    // constructor
    _list_iterator(link_type x) : node_(x) {}
    _list_iterator() : node_(0) {}
    _list_iterator(const iterator& x) : node_(x.node_) {}

    // 合成的就可以
    // self& operator=(iterator rhs)
    // {
    //     swap(rhs);
    //     return *this;
    // }
    // void swap(_list_iterator<T>& rhs)
    // {
    //     using std::swap;
    //     swap(node_, rhs.node_);
    // }
    bool operator==(const self& x) const { return node_ == x.node_; }
    bool operator!=(const self& x) const { return node_ != x.node_; }
    // dereference, 取的是节点的数据值
    reference operator*() const { return node_->data_; }
    // 以下是迭代器成员存取(member access)运算子的标准做法
    pointer operator->() const { return &(operator*()); }

    // 前置运算符应该返回递增或递减后对象的引用
    // 后置运算符应该返回对象的原值,返回的形式是一个值而非引用

    // 对迭代器累加1,就是前进一个节点 (前置++)
    self& operator++()
    {
        node_ = (link_type)((*node_).next_);
        return *this;
    }
    // 后置++
    self operator++(int) 
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    // 对迭代器递减1,就是后退一个节点 (前置--)
    self& operator--()
    {
        node_ = (link_type)((*node_).prev_);
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
};

template<typename T>
class List {
public:
    // 以下直接定义也可以
    typedef _list_iterator<T>       iterator;
    typedef T                       value_type;
    typedef value_type*             pointer;
    typedef value_type&             reference;
    typedef size_t                  size_type;
    typedef ptrdiff_t               defference_type;
protected:
    typedef _list_node<T> list_node_;
public:
    typedef list_node_* link_type;
protected:
    link_type node_; // 只要一个指针,便可表示整个环状双向链表
    // 空间分配器
    std::allocator<list_node_> allocator_;

public:
    List() { _empty_initialize(); }
    List(const List<T>& rhs)
    {
        _empty_initialize();
        std::copy(rhs.begin(), rhs.end(), std::back_inserter(*this));
    }
    List<T>& operator=(List<T> rhs)
    {
        swap(rhs);
        return *this;
    }

    List(std::initializer_list<T> rhs)
    {
        _empty_initialize();
        std::copy(rhs.begin(), rhs.end(), std::back_inserter(*this));        
    }
    List<T>& operator=(std::initializer_list<T> rhs)
    {
        _empty_initialize();
        std::copy(rhs.begin(), rhs.end(), std::back_inserter(*this)); 
        return *this;
    }
    ~List() { clear(); node_ = 0; }
    iterator begin() { return iterator((*node_).next_); }
    iterator end() { return iterator(node_); }
    iterator begin() const { return (*node_).next_; }
    iterator end() const { return node_; }
    bool empty() const { return node_->next_ == node_; }
    size_type size() const
    {
        return std::distance(begin(), end());
    }
    // 取头节点的内容(元素值)
    reference front() { return *begin(); }
    // 取尾节点的元素值(元素值)
    reference back() { return *(--end()); }
    // 插入位置之前插入新节点
    iterator insert(iterator position, const T& x)
    {
        link_type tmp = _create_node(x);
        // 调整双向指针,将tmp加入链表
        tmp->next_ = position.node_;
        tmp->prev_ = position.node_->prev_;
        (link_type(position.node_->prev_))->next_ = tmp;
        position.node_->prev_ = tmp;
        return tmp;
    }
    void push_back(const T& x) { insert(end(), x); }
    void push_front(const T& x) { insert(begin(), x); } 

    // 移除迭代器所指节点
    iterator erase(iterator position)
    {
        link_type next_node = link_type(position.node_->next_);
        link_type prev_node = link_type(position.node_->prev_);
        prev_node->next_ = next_node;
        next_node->prev_ = prev_node;
        _destroy_node(position.node_);
        return iterator(next_node);
    }
    // 移除头节点
    void pop_front() { erase(begin()); }
    // 移除尾节点
    void pop_back() 
    { 
        iterator tmp = end();
        erase(--tmp); 
    }

    // 清除所有节点(整个链表)
    void clear();
    // 将值为value的所有元素移除
    void remove(const T& value);
    // 移除数值相同的连续元素
    // 注意: 只有"连续相同的元素"才会被移除剩下一个
    void unique();
    // 将x接合于position所指位置之前,x必须不同于*this
    void splice(iterator position, List& x)
    {
        if (!x.empty())
        {
            _transfer(position, x.begin(), x.end());
        }
    }
    // 将i所指元素接合于position所指位置之前,position和i可指向同一个list
    void splice(iterator position, List&, iterator i)
    {
        iterator j = i;
        ++j;
        if (position == i || position == j) return;
        _transfer(position, i, j);
    }
    // 将［first, last）内的所有元素接合于position所指位置之前
    // position和[first, last)可指向同一个list
    // 但position不能位于{first,last)之间-
    void splice(iterator position, List&, iterator first, iterator last)
    {
        if (first != last)
            _transfer(position, first, last);
    }
    // 将x合并到this身上,两个list必须先经过排序
    void merge(List<T>& x);
    // 将*this中的元素逆序
    void reverse();
    // 利用quick sort将列表中的元素排序
    void sort();

private:
    // 配置一个节点并返回
    link_type _get_node() { return allocator_.allocate(1); }
    // 释放一个节点
    void _put_node(link_type p) 
    { 
        allocator_.deallocate(p, 1); 
    }
    // 产生(配置并构造)一个节点,带有元素值
    link_type _create_node(const T& x)
    {
        link_type p = _get_node();
        allocator_.construct(p, x);
        return p;
    }
    // 销毁(析构并释放)一个节点
    void _destroy_node(link_type p)
    {
        allocator_.destroy(p);
        _put_node(p);
    }

    void _empty_initialize()
    {
        node_ = _get_node();
        node_->next_ = node_;
        node_->prev_ = node_;
    }
    // 将[first, last)内的所有元素移到position之前
    void _transfer(iterator position, iterator first, iterator last);

    void swap(List<T>& rhs)
    {
        using std::swap;
        swap(node_, rhs.node_);
    }
};

template<typename T>
void List<T>::clear()
{
    link_type cur = node_->next_;
    while (cur != node_)
    {
        link_type tmp = cur;
        cur = cur->next_;
        _destroy_node(tmp);
    }
    node_->next_ = node_;
    node_->prev_ = node_;
}
template<typename T>
void List<T>::remove(const T& value)
{
    iterator first = begin();
    iterator last = end();
    while (first != last)
    {
        iterator next = first;
        ++next;
        if (*first == value) erase(first);
        first = next;
    }
}
template<typename T>
void List<T>::unique()
{
    iterator first = begin();
    iterator last = end();
    if (first == last) return;
    iterator next = first;
    while (++next != last)
    {
        if (*first == *next)
            erase(next);
        else  
            first = next;
        next = first;
    }
}
template<typename T>
void List<T>::_transfer(iterator position, iterator first, iterator last)
{
    if (position != last)
    {
        (*link_type((*last.node_).prev_)).next_ = position.node_;
        (*link_type((*first.node_).prev_)).next_ = last.node_;
        (*link_type((*position.node_).prev_)).next_ = first.node_;
        link_type tmp = link_type((*position.node_).prev_);
        (*position.node_).prev_ = (*last.node_).prev_;
        (*last.node_).prev_ = (*first.node_).prev_;
        (*first.node_).prev_ = tmp;
    }
}
template<typename T>
void List<T>::merge(List<T>& x)
{
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end();

    while (first1 != last1 && first2 != last2)
    {
        if (*first2 < *first1)
        {
            iterator next = first2;
            _transfer(first1, first2, ++next);
            first2 = next;
        }
        else  
        {
            ++first1;
        }
        if (first2 != last2)
            _transfer(last1, first2, last2);
    }
}

template<typename T>
void List<T>::reverse()
{
    // 如果是空表或只有一个元素,直接返回
    if (node_->next_ == node_ || 
        link_type(node_->next_)->next_ == node_)
    {
        return;
    }
    // 头插法
    iterator first = begin();
    ++first; 
    while (first != end())
    {
        iterator old = first;
        ++first;
        _transfer(begin(), old, first);
    }
}

// 有问题 死循环
template<typename T>
void List<T>::sort()
{
    // 如果是空表或只有一个元素,直接返回
    if (node_->next_ == node_ || 
        link_type(node_->next_)->next_ == node_)
    {
        return;
    }
    List<T> carry;
    List<T> counter[64];
    int fill = 0;
    while (!empty())
    {
        carry.splice(carry.begin(), *this, begin());
        int i = 0;
        while (i < fill && !counter[i].empty())
        {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if (i == fill) ++fill;
    }

    for (int i = 1; i < fill; i++)
    {
        counter[i].merge(counter[i-1]);
    }
    swap(counter[fill-1]);
}

int main()
{
    List<int> ilist;
    std::cout << "size:" << ilist.size() << std::endl;
    ilist.push_back(0);
    ilist.push_back(1);
    ilist.push_back(2);
    ilist.push_back(3);
    ilist.push_back(4);
    std::cout << "size:" << ilist.size() << std::endl;

    std::cout << "front:" << ilist.front() << " *begin():" << *ilist.begin() << '\n';
    std::cout << "back:" << ilist.back() << " *(end()-1):" << *(--ilist.end()) << '\n';

    std::cout << "0 1 2 3 4  --> ";
    std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    List<int>::iterator ite = std::find(ilist.begin(), ilist.end(), 3);
    if (ite != 0)
        ilist.insert(ite, 99);
    
    std::cout << "size: 6 --> " << ilist.size() << '\n';
    std::cout << "insert(99): 3 --> " << *ite << '\n';

    std::cout << "0 1 2 99 3 4  --> ";
    std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    ite = std::find(ilist.begin(), ilist.end(), 1);
    if (ite != ilist.end())
        std::cout << "2 --> " << *(ilist.erase(ite)) << '\n';

    std::cout << "0 2 99 3 4 --> ";
    std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    ilist.push_front(8);
    std::cout<< "8 --> " << "front:" << ilist.front() << " *begin():" << *ilist.begin() << '\n'; 

    ilist.pop_back();
    ilist.pop_front();
    std::cout << "0 2 99 3  --> ";
    std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    ilist.push_back(3);
    ilist.push_front(0);
    ite = std::find(ilist.begin(), ilist.end(), 3);
    if (ite != 0)
        ilist.insert(ite, 99);

    std::cout << "0 0 2 99 99 3 3  --> ";
    std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    ilist.unique();
    std::cout << "unique: 0 2 99 3  --> ";
    std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    ilist.reverse();
    std::cout << "reverse: 3 99 2 0  --> ";
    std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    // ilist.sort();

    List<int> ilist2(ilist);
    ilist.insert(ilist.begin(), 66);
    std::cout << "ilist2 size: 4 --> " << ilist2.size() << std::endl;
    std::copy(ilist2.begin(), ilist2.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    List<int> ilist3({1,2,3,4,5,6});
    std::cout << "ilist3 size: 6 --> " << ilist3.size() << std::endl;
    std::copy(ilist3.begin(), ilist3.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    List<int> ilist4 = {4,6,8,9,10,11};
    std::cout << "ilist4 size: 6 --> " << ilist4.size() << std::endl;
    std::copy(ilist4.begin(), ilist4.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    ilist3.merge(ilist4);
    std::copy(ilist3.begin(), ilist3.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    ilist.splice(ilist.begin(), ilist2); 
    std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}