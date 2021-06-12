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

    void setType(char TileType) { m_TileType = TileType; };
};

class CStoneB : public CTile
{
public:
    CStoneB() : CTile(2, 2){};
};

class CIronB : public CTile
{
public:
    CIronB() : CTile(3, 2){};
};

class CBoneB : public CTile
{
public:
    CBoneB() : CTile(0, 1){};
};

class CGoldB : public CTile
{
public:
    CGoldB() : CTile(4, 3){};
};

class CDiamondB : public CTile
{
public:
    CDiamondB() : CTile(5, 4){};
};

class CCoralB : public CTile
{
public:
    CCoralB() : CTile(1, 1){};
};

class CAirB : public CTile
{
public:
    CAirB() : CTile('.', 1){};
};