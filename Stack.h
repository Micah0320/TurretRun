#ifndef STACK_H
#define STACK_H
#include "SLList.h"
#include "SLNode.h"
#include <iostream>
class UnderflowError // An UnderflowError object is thrown if
{};

template < typename T >
class Stack
{
public:
    void push(T val)
    {
        list_.insert_head(val);
    }

    void pop()
    {
        if (list_.size() == 0)
            throw UnderflowError();
        list_.delete_head();
    }

    T top() const
    {
        return list_.phead()->key();
    }

    T & top()
    {
        return list_.phead()->key();
    }

    int size()
    {
        return list_.size();
    }

    bool is_empty()
   {
        return list_.is_empty();
    }

    void clear()
    {
        list_.clear();
    }

    SLNode< T > *phead() const
    {
        return list_.phead();
    }
private:
    SLList < T > list_;
};

template < typename T >
std::ostream & operator<<(std::ostream & cout,
                          const Stack< T > & stack)
{
    
    SLNode< T > *p = stack.phead();
    cout << '[';
    while (p!= NULL)
    {
        std::cout << p->key();
        p = p->next();
        cout << ' ';
    }
    cout <<']';
    return cout;
}
#endif
