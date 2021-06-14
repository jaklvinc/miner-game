#pragma once

#include "fairy.h"
#include "bomb.h"
#include "teleport.h"
#include <list>

/**
 * Class that handles all entities in a game
 */
class CEntityHandler
{
    private:
        std::list<std::shared_ptr<CEntity>> m_Entities;
    
    public:
        /**
         * Loads all entities from a specified file
         * 
         * @param[in] filename Name of the file to load from
         * @param[in] map map for reference to check size with
         * 
         * @returns if the entities could be loaded
         */
        bool LoadEntities(std::string filename , CMap & map);

        /**
        * Saves all entities to a certain file.
        * 
        * @param[in] filename the name of file to save to
        * 
        * @returns if the entities were saved
        */
        bool SaveEntities(std::string filename);

        /**
         * Outputs the apperanace of all entities into a 2D vector of chars.
         * 
         * @param[in] toPrint 2D vector of chars to then be ouptutted to the screen.
         */
        void ShowEntities( std::vector<std::vector<char>> & toPrint ) const;

        /**
         * Calls Move() to all entities saved in this handler
         * 
         * @param[in] player player for reference
         * @param[in] map map for reference
         */
        void MoveEntities( CPlayer & player , CMap & map );

        /**
         * Calls Action() on all entities saved in this handler
         * 
         * @param[in,out] player player for reference for the entities
         * @param[in,out]  map map for refernce for the enitities
         */
        void ActionEntities( CPlayer & player , CMap & map );
};