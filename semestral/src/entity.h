#pragma once

#include "player.h"
#include <fstream>

/**
 * Class that represents all entities
 */
class CEntity
{
protected:
    int m_X;
    int m_Y;
    bool m_Used;

public:
    /**
     * Default constructor
     */
    CEntity();

    /**
     * Constructor with indeces
     * 
     * @param[in] x,y coodrinates of entity in the game
     */
    CEntity(int x, int y);

    /**
     * Moves the entity around the map
     * 
     * @param[in] player player to reference
     * @param[in] map map for reference
     */
    virtual void Move(CPlayer & player, const CMap &map);

    /**
     * Performs action based on the type of entity
     * 
     * @param[in,out] player player to reference
     * @param[in,out] map map for reference
     */
    virtual void Action( CPlayer & player , CMap & map ) = 0;
    
    /**
     * Shows its appearance based on the type of entity
     * 
     * @returns appearance in for reference in map
     */
    virtual char Appearance( ) = 0;

    /**
     * Saves itself into speficied file
     * 
     * @param[in] to filestram of the file to save to
     */
    virtual void Save( std::ofstream & to ) = 0;

    /**
     * Getter for if the entity is used
     *
     * @returns if the entity is used
     */
    bool GetUsed() const;

    /**
     * Decides if player is around within distance
     * 
     * @param[in] player player to check the distance with
     * @param[in] far distance to check
     * 
     * @returns if player is within specified distance
     */
    bool InRange( CPlayer & player , int far ) const;

    /**
     * Getter for x coodrinate of entity
     * 
     * @returns x coord of 'this'
     */
    int GetX()const;

    /**
     * Getter for y coordinate of an entity
     * 
     * @returns y coord of 'this'
     */
    int GetY()const;
};




