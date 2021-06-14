#pragma once

#include "player.h"
#include <fstream>

class CEntity
{
protected:
    int m_X;
    int m_Y;
    bool m_Used;

public:
    CEntity();
    CEntity(int x, int y);

    virtual void Move(CPlayer & player, const CMap &map);
    virtual void Action( CPlayer & player , CMap & map );
    virtual char Appearance( ) = 0;
    virtual void Save( std::ofstream & to ) = 0;
    bool GetUsed() const;
    bool InRange( CPlayer & player , int far ) const;

    int GetX()const;
    int GetY()const;
};




