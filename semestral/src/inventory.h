#pragma once

#include <map>
#include <string>

/**
 * Class that saves information about inventory of player.
 */
class CInventory
{
private:
    /** Stores number of each item in possesion */
    int m_Inv[6];
    int m_Prices[6];
    int m_Weights[6];

    /** Stores current fullness of inventory */
    int m_currentLoad;
    
    /** Stores max size of inventory*/
    int m_maxLoad;

    /**
     * Prints one item of inventory
     * 
     * @param[in] type the type of item to print
     */
    void PrintLine(const int type) const;

public:
    /**
     * Loads inventory from a file
     * 
     * @param[in] filename name of the file to load from
     * @param[in] backpackLvl level of players backpack to set max load of inventory
     * 
     * @returns if the inventory was loaded properly
     */
    bool InitInv(const std::string filename,const int backpackLvl);

    /**
     * Saves inventory to a file
     * 
     * @param[in] filename name of the file to save to
     * 
     * @returns if the inventory could be saved
     */
    bool SaveInv(const std::string filename) const;
    
    /**
     * Adds a certain quantity of an item to inventory
     * 
     * If inventory is full, only nuber of items that fit, will be added. The rest is thrown away.
     * 
     * @param[in] type the type of item to be added
     * @param[in] quantity the quantity of specified item to be added
     * @param[in] backpackLvl the level of players backpack
     */
    void AddToInv(const int type, const int quantity, const int backpackLvl);

    /**
     * Updates the max size of inventory
     * 
     * @param[in] size to what size to update the invenrtory
     */
    void UpdateSize( int size );

    /**
     * Resets the inventory
     * 
     * Sets all the quantities to 0
     */
    void Reset();

    /**
     * Erases half of all items in inventory.
     */
    void Die();

    /**
     * Getter for weight of a specified item
     * 
     * @param[in] type type of the item to get wight of
     * 
     * @returns weight of the item
     */
    int GetWeight(const int type) const;

    /**
     * Getter for price of a specified item
     * 
     * @param[in] type type of the item to get price of
     * 
     * @returns price of the item
     */
    int GetPrice(const int type) const;

    /**
     * Getter for quantity of a specified item
     * 
     * @param[in] type type of the item to get quantity of
     * 
     * @returns quantity of the item
     */
    int GetQuantity(const int type) const;
    
    /**
     * Getter for max size of the inventory
     * 
     * @returns max size of the inventory
     */
    int GetSize() const;

    /**
     * Getter for current fullness of the inventory
     * 
     * @returns current fullness
     */
    int GetLoad() const;

    /**
     * Prints the inventory to the standard output.
     */
    void Print() const;

    /**
     * Prints the capacity of the inventory.
     */
    void PrintCapacity() const;
};