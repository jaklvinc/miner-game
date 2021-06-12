#pragma once

#include "inventory.h"
#include "entity.h"
#include <string>

class CPlayer : public CEntity
{
protected:
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
    CPlayer() : CEntity(){};
    CPlayer(int x, int y);

    bool Load(std::string filename);
    bool Save(std::string filename);

    int GetLight();
    CInventory &GetInv();

    void Move(const CMap &map, const char direction) override;
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