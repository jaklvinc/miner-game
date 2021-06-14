#pragma once

#include "entity.h"

class CTeleport : public CEntity
{
public:
    CTeleport(int x, int y);
    void Action( CPlayer & player , CMap & map ) override;
    char Appearance( ) override;
    void Save( std::ofstream & to ) override;
};