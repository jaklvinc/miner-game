#pragma once

#include "inventory.h"
#include "map.h"
#include <string>

/**
 * Class that stores information about the player
 * 
 * This concludes the position, inventory, health etc.
 */
class CPlayer
{
protected:
    int m_PosX;
    int m_PosY;
    int m_MaxHealth;
    int m_Health;

    CInventory m_Inv;
    int m_LvlLight;
    int m_LvlTank;
    int m_LvlDrill;
    int m_LvlBackpack;
    int m_Money;

    /**
     * Finds new position of a player after he moves in a certain direction
     * 
     * @param[in] dir direction to move
     * @param[in] width,height dimensions of the map
     * 
     * @returns pair of x,y coords
     */
    std::pair<int, int> NewPos(const char dir, const int width, const int height) const;

    /**
     * Prints lvl of equpiement to std::cout
     * 
     * @param[in] lvl level od equipement to be printed
     */
    void PrintEquipLvl(const int lvl) const;

    /**
     * Prints full shop interface to standard output
     */
    void PrintShop() const;

public:
    /**
     * Default constructor
     */
    CPlayer();

    /**
     * Implicit constructor with indeces
     * 
     * @param[in] x,y indeces of player in the map
     */
    CPlayer(int x, int y);

    /**
     * Loads the player in from a specified file
     * 
     * @param[in] filename name of the file to load from
     * @param[in] map map to get dimensions from
     *  
     * @returns if the player was loaded properly
     */
    bool Load(std::string filename, CMap & map);

    /**
     * Saves the player to specified file
     * 
     * @param[in] filename name of the file to save the player
     *
     * @returns if hte player was saver properly
     */
    bool Save(std::string filename);

    /**
     * Getter for how far the player can see
     * 
     * @returns the distance the player can see
     */
    int GetLight() const;

    /**
     * Getter for players inventory
     * 
     * @returns inventory of the player
     */
    CInventory & GetInv();

    /**
     * Getter for the X coordinate of the player
     * 
     * @returns x coord of the player
     */
    int GetX() const;

    /**
     * Getter for the Y cooridnate of the player
     * 
     * @returns y coord of the player
     */
    int GetY() const;

    /**
     * Moves the player in a certain direction
     * 
     * @param[in] map the map that the player is on
     * @param[in] direction direction of the movement
     */
    void Move(const CMap &map, const char direction);

    /**
     * Teleports the player to certain coordinates
     * 
     * @param[in] map the map that the player is on
     * @param[in] x,y coodinates to be moved to
     */
    void Teleport( const CMap &map, int x , int y );

    /**
     * Method to count the number of turns it takes to move in a certain direction
     * 
     * @param[in] map the map that the player is on
     * @param[in] diraction of tile to count hardness of
     * 
     * @returns number of moves it takes to move
     */
    int MoveCost(const CMap &map, const char direction) const;

    /**
     * Mines a tile on a map that player is standing on. 
     * 
     * @param[out] map map that player is on
     */
    void Mine(CMap &map);

    /**
     * Lowers the oxygen level
     * 
     * @param[in] amount amount of oxygen to be lowered.
     */
    void OxDown(const int amount);

    /**
     * Resets the oxygen to the maximum value
     */
    void OxReset();

    /**
     * Moves the player on top and resets the inventory and oxygen
     */
    void Die();

    /**
     * Upgrade certain players equipement
     * 
     * @param[in] type type of equipment
     * 
     * @returns if the equipment was uprgaded
     */
    bool UpgradeEquipment(int type);

    /**
     * Ouptuts the position of the player to a 2D array
     * 
     * @param[out] toPrint 2D vector of chars to then be ouptutted to the screen
     */
    void ShowPos(std::vector<std::vector<char>> & toPrint) const;

    /**
     * Prints player stats to standard output
     */
    void PrintStats() const;

    /**
     * Prints inventory capacity to standard output
     */
    void PrintInvCapacity() const;

    /**
     * Prints inventory to standard output. Also lets player to sell his inventory if possible
     */
    void Inventory();

    /**
     * Prints shop to standard output. Also lets player upgrade his equipment
     */
    void Shop();

    /**
     * Checks if the player reached the win condition.
     */
    bool Win() const ;
};