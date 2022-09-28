#include "tile.h"
#include "enum.h"

CTile::CTile(int type, int mineTime)
{
    m_TileType = type;
    m_MineTime = mineTime;
}

int CTile::GetType() const 
{ 
    return m_TileType; 
}

int CTile::GetToughtness() const 
{ 
    return m_MineTime; 
}