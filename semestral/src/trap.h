#pragma once

#include "player.h"
#include <fstream>

class CTrap
{
protected:
    int m_X;
    int m_Y;
    bool m_Used;

public:
    CTrap();
    CTrap(int x, int y);

    virtual void Move(CPlayer & player, const CMap &map);
    virtual void Attack( CPlayer & player , CMap & map );
    virtual char Appearance( ) = 0;
    virtual void Save( std::ofstream & to ) = 0;
    bool GetUsed() const;
    bool InRange( CPlayer & player , int far ) const;

    int GetX()const;
    int GetY()const;
};

class CBomb : public CTrap
{
public:
    CBomb();
    CBomb(int x, int y);
    void Attack( CPlayer & player , CMap & map ) override; // attacks
    char Appearance( ) override;
    void Save( std::ofstream & to ) override;
};

class CFairy: public CTrap
{
private:
    int m_Cooldown;
    bool m_Active;
    int m_Move;
public:
    CFairy();
    CFairy(int x, int y, int cooldown, int move);
    void Attack( CPlayer & player , CMap & map ) override;
    void Move(CPlayer & player, const CMap &map) override;
    char Appearance( ) override;
    void Save( std::ofstream & to ) override;
};
