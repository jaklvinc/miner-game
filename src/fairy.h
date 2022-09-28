#pragma once

#include "entity.h"

class CFairy: public CEntity
{
private:
    /** How many double turns until the fairy can move*/
    int m_Cooldown;

    /** Stores if the fairy can be seen an heal now */
    bool m_Active;

    /** Stores if the fairy is moving this turn */
    int m_Move;
public:
    /**
     * Constructor with all needed parameters
     * 
     * @param[in] x,y Coords on the map
     * @param[in] cooldown the time left to activate
     * @param[in] move to check if the fairy can move in this turn
     */
    CFairy(int x, int y, int cooldown, int move);
    
    /**
     * Refills players oxygen, when caught
     * 
     * @param[in,out] player player to have oxygen refilled
     * @param[in,out] map map for reference
     */
    void Action( CPlayer & player , CMap & map ) override;

    /**
     * Moves the fairy randomly around map every 2 turns
     * 
     * @param[in] player player for reference
     * @param[in] map map for reference
     */
    void Move(CPlayer & player, const CMap &map) override;

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