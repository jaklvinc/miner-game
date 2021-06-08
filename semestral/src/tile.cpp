#include "tile.h"
#include "enum.h"

CTile::CTile(                   int type , 
                                int MineTime )
{
    m_TileType = type;
    m_MineTime = MineTime;
}