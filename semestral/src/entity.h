#pragma once

#include <string>
#include "map.h"

class CEntity
{
protected:
    int m_PosX;
    int m_PosY;
    int m_MaxHealth;
    int m_Health;

public:
    CEntity(){};           // def contructor
    CEntity(int x, int y); //constructor with position

    int GetX(void);
    int GetY(void);
    //movement commands
    virtual void Move(const CMap &map, const char direction) = 0;
};

