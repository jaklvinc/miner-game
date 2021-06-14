
#include "fairy.h"

CFairy::CFairy(int x, int y, int cooldown , int move ) : CEntity(x,y) , m_Cooldown(cooldown) , m_Move(move) 
{
    if (m_Cooldown == 0)
        m_Active=1;
}

void CFairy::Action( CPlayer & player , CMap & map )
{
    if ( InRange( player , 0 ) && m_Active == 1)
    {
        player.OxReset();
        m_Active=0;
        m_Cooldown=7;
    }
}

void CFairy::Move(CPlayer & player, const CMap &map)
{
    if( m_Move == 1 )
    {
        if (m_Cooldown>0)
        {
            m_Cooldown--;
            if (m_Cooldown == 0)
                m_Active=1;
        }
        int dir = rand()%4;
        if ( dir == 0 && m_X>0)
            m_X--;
        else if ( dir == 1 && m_X<map.GetWidth() )
            m_X++;
        else if ( dir == 2 && m_Y>0 )
            m_Y--;
        else if ( dir == 3 && m_Y<map.GetHeight())
            m_Y++;
        
        m_Move = 0;
    }
    else
        m_Move++;
}

char CFairy::Appearance( )
{
    if ( !m_Active )
        return '.';
    else
        return 'F';
}

void CFairy::Save( std::ofstream & file )
{
    file << 'F' << ' ' << m_X << ' ' << m_Y << ' ' << m_Cooldown << ' ' << m_Move << std::endl;
}