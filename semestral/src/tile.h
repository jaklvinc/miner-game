#pragma once
#include <cstdlib>
/* 
    A = air
    S = stone
    C = coral
    I = iron
    G = gold
    B = bone
    D = Diamond
*/

class CTile
{
protected:
    int m_TileType;
    int m_MineTime; // sets the energy used to mine certain Tile
public:
    CTile(){};
    CTile(int type,
          int MineTime);

    //getters for the member properties
    int getType() const { return m_TileType; };
    int getToughtness() const { return m_MineTime; };
};
