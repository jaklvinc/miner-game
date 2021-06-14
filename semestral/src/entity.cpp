#include "entity.h"
CEntity::CEntity()
{
    m_Used = false;
}

CEntity::CEntity(int x, int y) : m_X(x) , m_Y(y)
{
    m_Used = false;
}

void CEntity::Move(CPlayer & player, const CMap &map)
{

}

bool CEntity::GetUsed() const
{
    return m_Used;
}

bool CEntity::InRange( CPlayer & player , int far) const
{
    return ( abs(player.GetX()-m_X) <= far && abs(player.GetY()-m_Y) <= far );
}

int CEntity::GetX()const
{
    return m_X;
}

int CEntity::GetY()const
{
    return m_Y;
}