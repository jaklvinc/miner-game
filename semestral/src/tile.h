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
    char m_TileType;
    int m_MineTime; // sets the energy used to mine certain Tile

public:
    CTile(){};
    CTile(  char type,
            int MineTime) ;

    //getters for the member properties
    const char &getType() const { return m_TileType; };
    int getToughtness() const { return m_MineTime; };


    void setType(char TileType) { m_TileType = TileType; };
};


class CStoneB : public CTile { public: CStoneB() : CTile('#', 2){}; };

class CIronB : public CTile { public: CIronB() : CTile('i', 2){}; };

class CBoneB : public CTile { public: CBoneB() : CTile('b', 1){}; };

class CGoldB : public CTile { public: CGoldB() : CTile('g', 3){}; };

class CDiamondB : public CTile { public: CDiamondB() : CTile('d', 4){}; };

class CCoralB : public CTile { public: CCoralB() : CTile('?', 1){}; };

class CAirB : public CTile { public: CAirB() : CTile('.', 1){}; };