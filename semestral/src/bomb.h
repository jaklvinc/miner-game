#pragma once

#include "entity.h"

class CBomb : public CEntity
{
public:
    /**
     * Constructor with x and y coordinates
     * 
     * @param[in] x,y Coords on the map
     */
    CBomb(int x, int y);
    void Action( CPlayer & player , CMap & map ) override;
    char Appearance( ) override;
    void Save( std::ofstream & to ) override;
};