#include "entity.h"
#include <fstream>
#include <sstream>
#include <iostream>

CEntity::CEntity(int x, int y) : m_PosX(x), m_PosY(y) {}

int CEntity::GetX()
{
    return m_PosX;
}

int CEntity::GetY()
{
    return m_PosY;
}
