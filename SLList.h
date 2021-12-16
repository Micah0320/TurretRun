// File: SLList.h
#ifndef SLLIST_H
#define SLLIST_H

#include "SLNode.h"
#include <iostream>
class IndexError // An IndexError object is thrown if
{}; // an invalid index value is used in
// a method/operator in the SLList
// class.

class ValueError // A ValueError object is thrown if
{}; // an invalid value is used in a
// method/operator in the SLList class.
// If the value is an index value, then
// the IndexError class is used instead.

template < typename T >

class SLList
{
public:
    SLList(SLNode< T > * p = NULL)
        :phead_(p)
    {
        debugPrinting_ = 0;
        size_ = 0;
        SLNode< T > * it = p;
        while (it != NULL)
        {
            ++size_;
            it = it->next();
        }
    }

    SLList(const SLList< T > &list)
    {
        debugPrinting_ = 0;
        SLNode< T > *p = list.phead();
        phead_ = new SLNode< T >(p->key());
        //std::cout << phead_->key();
        SLNode< T > *q = phead_;
        while (p->next()!= NULL)
        {
            p = p->next();
            insert_tail(p->key());
            
        }
        size_ = list.size();
        //std::cout << std::endl;
        
            
    }

    ~SLList()
    {
        SLNode< T > *p = phead_;
        SLNode< T > *q;

        while (p != NULL)
        {
            q = p->next();
            delete p;
            p = q;
        }
    }

    SLNode< T > * phead() const
    {
        return phead_;
    }

    SLNode< T > * &phead()
    {
        return phead_;
    }
    
    void insert_head(T value)
    {
        phead_ = new SLNode< T >(value, phead_);
        ++size_;
    }

    void insert_tail(T value)
    {
        if (phead_!= NULL)
        {
            SLNode< T > *p = phead_;
            while (p->next() != NULL)
            {
                p = p->next();
            }
            p->next() = new SLNode< T >(value);
            ++size_;
        }
        else
        {
            insert_head(value);
        }
    }

    void delete_head()
    {
        if (phead_ != NULL)
        {
            SLNode< T > * p = phead_;
            phead_ = phead_->next();
            delete p;
            --size_;
        }
    }

    int remove_head()
    {
        if (phead_ != NULL)
        {
            int head = phead_->key();
            SLNode< T > * p = phead_;
            phead_ = phead_->next();
            delete p;
            --size_;
            return head;
        }
       
    }

    void delete_tail()
    {
        if (phead_!= NULL)
        {
            SLNode< T > * p = phead_;
            SLNode< T > * q = p->next();
            while (q->next() != NULL)
            {
                p = p->next();
                q = q->next();
            }
            delete q;
            p->next() = NULL;
            --size_;
        }
    }

    int remove_tail()
    {
        if (phead_!= NULL)
        {
            int tail;
            SLNode< T > * p = phead_;
            SLNode< T > * q = p->next();
            while (q->next() != NULL)
            {
                p = p->next();
                q = q->next();
            }
            tail = q->key();
            delete q;
            p->next() = NULL;
            --size_;
            return tail;
        }
    }
    
    void clear()
    {
        SLNode< T > *p = phead_;
        SLNode< T > *q;
        size_ = 0;

        while (p != NULL)
        {
            q = p->next();
            delete p;
            p = q;
        }
        phead_ = NULL;
    }

    SLNode< T > * find(int value)
    {
        SLNode< T > *p = phead_;
        while (p != NULL)
        {
            if (p->key() == value)
            {
                return p;
            }
            p = p->next();
        }
    }

    int remove(SLNode< T > *index)
    {
        if (phead_!= NULL)
        {
            if (size_ == 0)
                throw ValueError();
            int value;
            //std::cout << index << ' ' << phead_ << std::endl;
            if (index == phead_)
            {
                value = remove_head();
                //std::cout << value << std::endl;
                return value;
            }
            
            SLNode< T > * p = phead_;
            SLNode< T > * q = p->next();
            while ( q != NULL &&
                   q != index)
            {
                //std::cout << p->key() << ' ' << q->key() <<std::endl;;
                p = p->next();
                q = q->next();
            }
            if (q == NULL)
                throw ValueError();
            p->next() = q->next();
            value = q->key();
            delete q;
            --size_;
            return value;
        }
        
           
    }

    void remove(int x)
    {
        remove(find(x));
    }

    SLList< T > & operator=(const SLList &list)
    {
        clear();
        
        SLNode< T > *p = list.phead();
        size_ = list.size();
        phead_ = new SLNode< T >(p->key());
        //std::cout << phead_->key();
        SLNode< T > *q = phead_;
        while (p->next()!= NULL)
        {
            p = p->next();
            insert_tail(p->key());
            
        }
        
    }

    int & operator[](int x)
    {
        if (x < size_)
        {
            if (x < 0)
            {
                throw IndexError();
            }
            SLNode< T > *p = phead();
            for(int i = 0; i < x; ++i)
            {
                p = p->next();
            }
            return p->key();
        }
        else
        {
            throw IndexError();
        }
    }
    


     void debugPrinting(bool x) 
    {
        debugPrinting_ = x;  
    }
    bool debugPrinting() const
    {
        return debugPrinting_;
    }

    int size() const
    {
        return size_;
    }

    bool is_empty() const
    {
        if (size() > 0)
            return 0;
        else
            return 1;
    }
    
private:
    SLNode< T > * phead_;
    int size_;
    bool debugPrinting_;
};

template < typename T >
std::ostream & operator<<(std::ostream & cout, const SLList< T >& x)
{
    if (x.debugPrinting())
    {
        cout << "<SLList " << &x << ' ' << "phead: " << x.phead()
             << '>';
    }
    else
    {
        SLNode< T > *p = x.phead();
        cout << '[';
        while (p!= NULL)
        {
            std::cout << p->key();
            p = p->next();
            cout << ' ';
        }
        cout <<']';
    }
    return cout;
}

#endif
