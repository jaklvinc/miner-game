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
    int m_MineTime;
public:
    /**
     * Constructor with types
     * 
     * @param[in] type Sets the type of this tile.
     * @param[in] mineTime Sets how many turns is needed to mine this.
     */
    CTile(int type,
          int mineTime);

    /**
     * Getter for tile type.
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
