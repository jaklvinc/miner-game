#include "trap.h"
CTrap::CTrap()
{
    m_Used = false;
}

CTrap::CTrap(int x, int y) : m_X(x) , m_Y(y)
{
    m_Used = false;
}

void CTrap::Move(CPlayer & player, const CMap &map)
{

}

void CTrap::Attack( CPlayer & player, CMap & map )
{

}

bool CTrap::GetUsed() const
{
    return m_Used;
}

bool CTrap::InRange( CPlayer & player , int far) const
{
    return ( abs(player.GetX()-m_X) <= far && abs(player.GetY()-m_Y) <= far );
}

int CTrap::GetX()const
{
    return m_X;
}

int CTrap::GetY()const
{
    return m_Y;
}


CBomb::CBomb() : CTrap() {}

CBomb::CBomb(int x, int y) : CTrap(x,y) {}

void CBomb::Attack( CPlayer & player, CMap & map )
{   
    if ( abs(m_X-player.GetX()) <= 1 && abs(m_Y-player.GetY()) <= 1 )
    {
        m_Used = true;
        player.OxDown(150);
        auto air = std::make_shared<CAirB>();
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


CFairy::CFairy() : CTrap() {}

CFairy::CFairy(int x, int y, int cooldown , int move ) : CTrap(x,y) , m_Cooldown(cooldown) , m_Move(move) 
{
    if (m_Cooldown == 0)
        m_Active=1;
}

void CFairy::Attack( CPlayer & player , CMap & map )
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
        else if ( dir == 1 && m_X<map.getWidth() )
            m_X++;
        else if ( dir == 2 && m_Y>0 )
            m_Y--;
        else if ( dir == 3 && m_Y<map.getHeight())
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