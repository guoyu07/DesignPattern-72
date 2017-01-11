#include <iostream>

class ICashSuper
{
public:
    virtual ~ICashSuper() {}
    virtual double acceptCash(double _cash) = 0;
};

class CCashNormal : public ICashSuper
{
public:
    virtual double acceptCash(double _cash) {
        return _cash;
    }
};

class CCashRebate : public ICashSuper
{
public:
    CCashRebate(double _rebate) : m_rebate(_rebate) {}
    virtual double acceptCash(double _cash) {
        return _cash * m_rebate;
    }

private:
    double m_rebate;
};

class CCashBack : public ICashSuper
{
public:
    CCashBack(double _condition, double _back) : m_condition(_condition), m_back(_back) {}
    virtual double acceptCash(double _cash) {
        double result = _cash;
        if (_cash >= m_condition) {
            result = _cash - m_back;
        }
        return result;
    }

private:
    double m_condition;
    double m_back;
};

class CCashContext
{
public:
    CCashContext(ICashSuper * _super) : m_cashSuper(_super) {}
    void switchContext(ICashSuper * _super) {
        m_cashSuper = _super;
    }
    double calculate(double _cash) {
        return m_cashSuper->acceptCash(_cash);
    }

private:
    ICashSuper * m_cashSuper;
};

class CCashFactory 
{
public:
    CCashFactory(int _type) {
        double result;
        switch (_type) {
            case 1:
                m_cashSuper = new CCashBack(300, 100);
                break;
            case 2:
                m_cashSuper = new CCashRebate(0.8);
                break;
            default:
                m_cashSuper = new CCashNormal();
                break;
        }
    }
    ~CCashFactory() {
        if (m_cashSuper)
            delete m_cashSuper, m_cashSuper = NULL;
    }
    
    double result(double _cash) {
        CCashContext cashContext(m_cashSuper);
        return cashContext.calculate(_cash);
    }

private:
    ICashSuper * m_cashSuper;
};

int main()
{
    CCashFactory * cashFactory = NULL;

    cashFactory = new CCashFactory(1);
    std::cout << cashFactory->result(600) << std::endl;

    delete cashFactory;

    cashFactory = new CCashFactory(2);
    std::cout << cashFactory->result(600) << std::endl;

    delete cashFactory;
}
