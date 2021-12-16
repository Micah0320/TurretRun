// File: SLNode.h
#ifndef SLNODE_H
#define SLNODE_H

#include <iostream>

template < typename T >
class SLNode
{
public:
SLNode(const T & key, SLNode< T > * next = NULL)
    : key_(key), next_(next)
    {
        debugPrinting_ = 1;
    }

    T key() const
    {
        return key_;
    }
    T & key()
    {
        return key_;
    }

    T get_key() const
    {
        return key_;
    }

    void set_key(T n)
    {
        key_ = n;
    }

    SLNode< T >* get_next() const
    {
        return next_;
    }

    SLNode< T > *& next()
    {
        return next_;
    }
    SLNode< T > * next() const
    {
        return next_;
    }


    void set_next(SLNode< T > * x)
    {
         next_ = x;
    }

   

    void debugPrinting(bool x) 
    {
        debugPrinting_ = x;  
    }
    bool debugPrinting() const
    {
        return debugPrinting_;
    }

   
private:
T key_;
SLNode< T > * next_;
bool debugPrinting_;
};

template < typename T >
std::ostream & operator<<(std::ostream & cout, const SLNode< T >& x)
{
    if (x.debugPrinting())
    {
        cout << "<SLNode " << &x << ' ' << "key: " << x.key()
             << ' ' << "next: " << x.next() << '>';
    }
    else
        cout << x.key();
    return cout;
}


#endif
