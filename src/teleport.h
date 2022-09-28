#pragma once

#include "entity.h"

class CTeleport : public CEntity
{
public:
    /**
     * Constructor with x and y coordinates
     * 
     * @param[in] x,y Coords on the map
     */
    CTeleport(int x, int y);

    /**
     * Teleports the player to the top layer if stepped onto. If activated also moves to another random place on map
     * 
     * @param[in,out] player player to be moved to top layer
     * @param[in,out] map map for teleport to be moved to another place
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