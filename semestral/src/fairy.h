#pragma once

#include "entity.h"

class CFairy: public CEntity
{
private:
    int m_Cooldown;
    bool m_Active;
    int m_Move;
public:
    CFairy();
    CFairy(int x, int y, int cooldown, int move);
    void Action( CPlayer & player , CMap & map ) override;
    void Move(CPlayer & player, const CMap &map) override;
    char Appearance( ) override;
    void Save( std::ofstream & to ) override;
};