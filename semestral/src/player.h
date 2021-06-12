#pragma once

#include "inventory.h"
#include "map.h"
#include <string>

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

    std::pair<int, int> NewPos(const char dir, const int width, const int height) const;
    void PrintEquipLvl(int lvl) const;
    void PrintShop() const;

public:
    CPlayer(){};
    CPlayer(int x, int y);

    bool Load(std::string filename);
    bool Save(std::string filename);

    int GetLight() const;
    CInventory &GetInv();
    int GetX() const;
    int GetY() const;

    void Move(const CMap &map, const char direction);
    int MoveCost(const CMap &map, const char direction) const;
    void Mine(CMap &map);
    void OxDown(const int amount);
    void OxReset();
    void Die();
    bool UpgradeEquipent(int type);

    void PrintStats() const;
    void PrintInvCapacity() const;
    void Inventory();
    void Shop();
};