#include <iostream>

class ITankState 
{
public:
    virtual ~ITankState() {}
    virtual void move(int x, int y) = 0;
    virtual void attack() = 0;
};

class CSiegeState : public ITankState
{
public:
    virtual void move(int x, int y) {
        std::cout << "cannot move in siege mode" << std::endl;
    }
    virtual void attack() {
        std::cout << "attack stronger" << std::endl;
    }
};

class CMoveState : public ITankState
{
public:
    virtual void move(int x, int y) {
        std::cout << "move to " << x << "," << y << std::endl;
    }
    virtual void attack() {
        std::cout << "attack normal" << std::endl;
    }
};

class CTankObject
{
public:
    CTankObject(ITankState * _state) : m_tankState(_state) {}

public:
    void switchState(ITankState * _state) {
        m_tankState = _state;
    }

    void move(int x, int y) {
        m_tankState->move(x, y);
    }

    void attack() {
        m_tankState->attack();
    }

private:
    ITankState * m_tankState;
};

int main()
{
    CSiegeState * siegeState = new CSiegeState;
    CMoveState  * moveState  = new CMoveState;

    CTankObject tank(moveState);

    tank.move(1, 2);
    tank.attack();

    tank.switchState(siegeState);

    tank.move(1, 2);
    tank.attack();

    delete siegeState;
    delete moveState;
}
