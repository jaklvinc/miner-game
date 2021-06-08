#include "player.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "enum.h"

CPlayer::CPlayer ( int x , int y ) : CEntity(x,y)
{
    //m_Inv.InitInv();
}

std::pair<int,int> CPlayer::NewPos( const char dir , const int width , const int height ) const
{
    int moveX = 0;
    int moveY = 0;

    switch(dir)
    {
        case 'w':
            moveY = -1;
            break;
        case 's':
            moveY = 1;
            break;
        case 'd':
            moveX = 1;
            break;
        case 'a':
            moveX = -1;
            break;
    }
    int new_X = m_PosX;
    int new_Y = m_PosY;
    if ( new_X+moveX >= 0 && new_X+moveX < width && new_Y+moveY >= 0 && new_Y+moveY < height )
    {
        new_X += moveX;
        new_Y += moveY;
    }
    return std::make_pair(new_X,new_Y);
}

bool CPlayer::Load ( std::string filename )
{

    std::ifstream data;
    data.open(filename);
    if (data.is_open())
    {
        std::string line;
        std::getline(data,line);
        std::stringstream ss(line);

        ss >> m_PosX >> m_PosY;

        if ( m_PosY < 0 || m_PosY < 0 )
        {
            return false;
        }

        std::getline(data,line);
        ss=std::stringstream(line);
        ss >> m_LvlLight >> m_LvlTank >> m_LvlDrill >> m_LvlBackpack >> m_Money;
        if (m_LvlLight > 4 || m_LvlLight < 1 || m_LvlTank > 4 || m_LvlTank < 1 ||
            m_LvlDrill > 4 || m_LvlDrill < 1 || m_LvlBackpack > 4 || m_LvlBackpack < 1 || m_Money < 0 )
        {
            return false;
        }

        std::getline(data,line);
        m_Health = stoi(line);
        if ( m_Health < 1 )
        {
            return false;
        }
        m_MaxHealth = 150 * m_LvlTank;

        if ( !m_Inv.InitInv(filename,m_LvlBackpack) )
            return false;

        return true;
    }
    else
        return false;
}

bool CPlayer::Save ( std::string filename)
{
    std::ofstream data;
    data.open(filename);
    if (data.is_open())
    {
        data << m_PosX << ' ' << m_PosY << ' ' << std::endl;
        data << m_LvlLight << ' ' << m_LvlTank << ' ' << m_LvlDrill << ' ' << m_LvlBackpack << ' ' << m_Money << std::endl;
        data << m_Health << std::endl;
        
        data.close();
        if ( !m_Inv.SaveInv( filename ) )
        {
            return false;
        }
        return true;
    }
    return false;
}

int CPlayer::GetLight ()
{
    return m_LvlLight;
}

CInventory & CPlayer::GetInv ()
{
    return m_Inv;
}

void CPlayer::Move ( const CMap & map , char dir )
{
    std::pair<int,int> new_pos;
    new_pos = NewPos(dir,map.getWidth(),map.getHeight());
    m_PosX=new_pos.first;
    m_PosY=new_pos.second;
    return;
}

int CPlayer::MoveCost ( const CMap & map, char dir) const
{
    std::pair<int,int> new_pos;
    new_pos = NewPos(dir,map.getWidth(),map.getHeight());
    if ( new_pos.first == m_PosX && new_pos.second == m_PosY )
        return 0;
    else
        return map.GetOnIndex(new_pos.first,new_pos.second)->getToughtness();
}

void CPlayer::Mine( CMap & map )
{
    auto new_tile=std::make_shared<CAirB>();
    map.SetOnIndex( m_PosX , m_PosY , new_tile);
    return;
}

void CPlayer::OxDown( int amount )
{
    m_Health-=amount;
    if ( m_Health <= 0 )
    {
        Die();
    }
    return;
}

void CPlayer::OxReset( )
{
    m_Health = m_MaxHealth;
}

void CPlayer::Die()
{
    m_PosY = 0;
    OxReset();
    m_Inv.Die();
}

void CPlayer::PrintStats() const
{
    std::cout << "\033[31m" << "OXYGEN: " << m_Health << "\033[34m" << 
        " | LIGHT LVL: " << m_LvlLight << 
        " |  OXYGEN TANK LVL: " << m_LvlTank << 
        " |  DRILL LVL: " << m_LvlDrill << 
        " |  BACKPACK LVL: " << m_LvlBackpack << "\033[0m" << std::endl;
}

