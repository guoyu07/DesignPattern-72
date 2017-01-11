#include <iostream>
#include <string>

class IPhone 
{
public:
    virtual ~IPhone() {}
    virtual void show() = 0;
};

class CMeizuPhone : public IPhone
{
public:
    CMeizuPhone(std::string _name) : m_name(_name) {}
    virtual void show() {
        std::cout << "MEIZU " << m_name << std::endl;
    }

private:
    std::string m_name;
};

class CNokiaPhone : public IPhone
{
public: 
    CNokiaPhone(std::string _name) : m_name(_name) {}
    virtual void show() {
        std::cout << "Nokia " << m_name << std::endl;
    }

private:
    std::string m_name;
};

class CDecoratorPhone : public IPhone
{
public:
    CDecoratorPhone(IPhone * _phone) : m_phone(_phone) {}
    virtual void show() {
        if (m_phone)
            m_phone->show();
    }

private:
    IPhone * m_phone;
};

class CWidgetPhone : public CDecoratorPhone
{
public:
    CWidgetPhone(IPhone * _phone) : CDecoratorPhone(_phone) {}
    virtual void show() {
        CDecoratorPhone::show();
        decorate();
    }

private:
    void decorate() {
        std::cout << "widget phone" << std::endl;
    }
};

class CFilmPhone : public CDecoratorPhone
{
public:
    CFilmPhone(IPhone * _phone) : CDecoratorPhone(_phone) {}
    virtual void show() {
        CDecoratorPhone::show();
        decorate();
    }

private:
    void decorate() {
        std::cout << "film phone" << std::endl;
    }
};

int main() 
{
    CMeizuPhone  * meizu  = new CMeizuPhone("Pro6 Plus");
    CWidgetPhone * widget = new CWidgetPhone(meizu);
    CFilmPhone   * film   = new CFilmPhone(widget);

    film->show();

    delete film;
    delete widget;
    delete meizu;

    return 0;
}
