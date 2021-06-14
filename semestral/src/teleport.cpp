
#include "teleport.h"

CTeleport::CTeleport(int x, int y) : CEntity(x,y) {}

void CTeleport::Action( CPlayer & player, CMap & map )
{   
    if ( abs(m_X-player.GetX()) == 0 && abs(m_Y-player.GetY()) == 0 )
    {
        player.Teleport(map,player.GetX(),0);
        
        m_X = rand() % map.GetWidth();
        m_Y = rand() % ( map.GetHeight() / 3 ) + ( ( map.GetHeight() / 3 ) * 2 );
    }
}

char CTeleport::Appearance()
{
    return 'T';
}

void CTeleport::Save( std::ofstream & file )
{
    file << 'T' << ' ' << m_X << ' ' << m_Y << std::endl;
}