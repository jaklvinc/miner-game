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

    /**
     * Blows up player and area around if player moves too close
     * 
     * @param[in,out] player player to be blown up if within distance
     * @param[in,out] map map to have a crater generated
     */
    void Action( CPlayer & player , CMap & map ) override;

    /**
     * Shows appearance
     * 
     * @returns appearance in for reference in map
     */
    char Appearance( ) override;

    /**
     * Saves itself into speficied file
     * 
     * @param[in] to filestram of the file to save to
     */
    void Save( std::ofstream & to ) override;
};