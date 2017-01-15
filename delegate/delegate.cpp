#include <iostream>
#include <typeinfo>
#include <list>

class IDelegate 
{
public:
    virtual ~IDelegate() {}
    virtual bool istype(const std::type_info & _type) = 0;
    virtual void invoke() = 0;
    virtual bool compare(IDelegate * _delegate) const = 0;
};

class CStaticDelegate : public IDelegate
{
public:
    typedef void (*Func)();
    CStaticDelegate(Func _func) : m_func(_func) { }

public:
    virtual bool istype(const std::type_info & _type) { 
        return typeid(CStaticDelegate) == _type; 
    }

    virtual void invoke() {
        m_func();
    }

    virtual bool compare(IDelegate * _delegate) const {
        if (NULL == _delegate || !_delegate->istype(typeid(CStaticDelegate))) 
            return false;

        CStaticDelegate * cast = dynamic_cast<CStaticDelegate *>(_delegate);
        return NULL == cast ? false : cast->m_func == m_func;
    }

private:
    Func m_func;
};

template<class T>
class CMethodDelegate : public IDelegate
{
public:
    typedef void (T::*Method)();
    CMethodDelegate(T * _object, Method _method) : m_object(_object), m_method(_method) {}
    virtual bool istype(const std::type_info & _type) { 
        return typeid(CMethodDelegate) == _type; 
    }

    virtual void invoke() {
        (m_object->*m_method)();
    }

    virtual bool compare(IDelegate * _delegate) const {
        if (NULL == _delegate || !_delegate->istype(typeid(CMethodDelegate))) 
            return false;

        CMethodDelegate * cast = dynamic_cast<CMethodDelegate *>(_delegate);
        return NULL == cast ? false : cast->m_object == m_object && cast->m_method == m_method;
    }
    

private:
    T * m_object;
    Method m_method;
};

inline IDelegate * NewDelegate(void (*_func)())
{
    return new CStaticDelegate(_func);
}

template<class T>
inline IDelegate * NewDelegate(T * _object, void (T::*_method)())
{
    return new CMethodDelegate<T>(_object, _method);
}

class CMultiDelegate
{
public:
    typedef std::list<IDelegate *> ListDelegate;
    typedef ListDelegate::iterator ListDelegateIterator;
    typedef ListDelegate::const_iterator ListDelegateIteratorConst;

public:
    CMultiDelegate() {}
    virtual ~CMultiDelegate() { clear(); }

private:
    CMultiDelegate(const CMultiDelegate & _delegate);
    CMultiDelegate& operator=(const CMultiDelegate & _delegate);

public:
    bool empty() const {
        return m_delegate.empty();
    }
    void clear() {
        for (ListDelegateIterator iter = m_delegate.begin(); 
                iter != m_delegate.end(); iter++) {
            if (*iter) {
                delete (*iter), (*iter) = NULL;
            }
        }
    }
    CMultiDelegate& operator+=(IDelegate * _delegate) {
        for (ListDelegateIterator iter = m_delegate.begin(); 
                iter != m_delegate.end(); iter++) {
            if (*iter && (*iter)->compare(_delegate)) {
                delete _delegate;
                return *this;
            }
        }
        m_delegate.push_back(_delegate);
        return *this;
    }
    CMultiDelegate& operator-=(IDelegate * _delegate) {
        ListDelegateIterator iter = m_delegate.begin();
        while (iter != m_delegate.end()) {
            if (*iter && (*iter)->compare(_delegate)) {
                delete (*iter), *iter = NULL;
                iter = m_delegate.erase(iter);
            } else {
                iter++;
            }
        }
        delete _delegate;
        return *this;
    }
    void operator()() {
        ListDelegateIterator iter = m_delegate.begin();
        while (iter != m_delegate.end()) {
            if (*iter) {
                (*iter)->invoke();
                iter++;
            } else {
                iter = m_delegate.erase(iter);
            }
        }
    }

public:
    ListDelegate m_delegate;
};

void NormalFunc() {
    std::cout << "normal function" << std::endl;
}

class CBase {
public:
    void output() {
        std::cout << "class function" << std::endl;
    }
};


int main(int argc, char **argv)
{
    CBase base;
    CMultiDelegate delegate;
    
    delegate += NewDelegate(NormalFunc);
    delegate += NewDelegate(&base, &CBase::output);
    delegate();

    delegate -= NewDelegate(NormalFunc);
    delegate();

    return 0;
}
