
#include "bomb.h"

CBomb::CBomb(int x, int y) : CEntity(x,y) {}

void CBomb::Action( CPlayer & player, CMap & map )
{   
    if ( abs(m_X-player.GetX()) <= 1 && abs(m_Y-player.GetY()) <= 1 )
    {
        m_Used = true;
        player.OxDown(150);
        CTile air('.',1);
        for( int y = -2 ; y <= 2 ; y++ )
        {
            for( int x = -2 ; x <= 2 ; x++ )
            {
                map.SetOnIndex(m_X+x,m_Y+y,air);
            }
        }
    }
}

char CBomb::Appearance()
{
    return 'B';
}

void CBomb::Save( std::ofstream & file )
{
    file << 'B' << ' ' << m_X << ' ' << m_Y << std::endl;
}