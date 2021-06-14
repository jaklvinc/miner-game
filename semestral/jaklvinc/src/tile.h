#pragma once

/**
 * Class that stores tiles, used in a map.
 * 
 * Each tile has a type and mine time.
 */
class CTile
{
protected:
    int m_TileType;
    
    /** Stores how many turns it takes to mine this tile */
    int m_MineTime;

    /** Stores what is outputted on the screen */
    char m_ShowType;
public:
    /**
     * Constructor with types
     * 
     * @param[in] type Sets the type of this tile.
     * @param[in] mineTime Sets how many turns is needed to mine this.
     */
    CTile(int type, int mineTime);

    /**
     * Getter for tile type
     * 
     * @returns The type of this tile
     */
    int GetType() const;

    /**
     * Getter for tile mine time.
     * 
     * @returns The mining time of this tile
     */
    int GetToughtness() const;
};
