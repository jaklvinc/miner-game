#pragma once
#include <string>
#include <vector>
#include <memory>
#include "tile.h"

/**
 * Class that stores 2D array of tiles.
 * 
 * Used to load , save and store and show in-game map.
 */
class CMap
{
private:
    int m_Height;
    int m_Width;
    std::vector<std::vector<CTile>> m_Map;
    
public:
    /**
     * Default constructor
     */
    CMap();

    /**
     * Getter for tile on a certain index
     * 
     * @param[in] x,y indeces of a searched tile
     * 
     * @returns searched tile
     */
    const CTile & GetOnIndex(int x, int y) const;

    /**
     * Setter of tile for a certain index.
     * 
     * @param[in] x,y indeces to put the new tile into
     * @param[in] newTile tile to set on specified index
     */
    void SetOnIndex(int x, int y, CTile newTile);

    /**
     * Loads a map from a certain file.
     * 
     * @param[in] filename the name of file to load from
     * 
     * @returns if the map could be loaded
     */
    bool Load(const std::string & filename);

    /**
     * Saves a map to a certain file.
     * 
     * @param[in] filename the name of file to save to
     * 
     * @returns if the map could be saved
     */
    bool Save(const std::string & filename);

    /**
     * Outputs the state of map into a 2D vector of chars.
     * 
     * @param[in] toPrint 2D vector of chars to then be ouptutted to the screen.
     */
    void ShowMap(std::vector<std::vector<char>> & toPrint) const;

    /**
     * Method to grow corals
     * 
     * Goes through the map and on eligible places
     * there is a random chance that a coral will respawn.
     */
    void Regenerate();

    /**
     * Getter for the width of this map
     * 
     * @returns width of this map
     */
    int GetWidth() const;

    /**
     * Getter for hte height of this map
     * 
     * @returns height of this map
     */
    int GetHeight() const;


};