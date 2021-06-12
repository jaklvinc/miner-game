#pragma once

#include <map>
#include <string>
#include "item.h"

class CInventory
{
private:
    int m_Inv[6];
    int m_Prices[6];
    int m_Weights[6];
    int m_currentLoad;
    int m_maxLoad;

    void PrintLine(int type) const;

public:
    CInventory();
    void AddToInv(int type, int quantity);
    bool InitInv(std::string filename, int backpackLvl);
    bool SaveInv(std::string filename) const;
    void Reset();
    void Die();

    int GetWeight(int type) const;
    int GetPrice(int type) const;
    int GetQuantity(int type) const;
    int GetSize() const;
    int GetLoad() const;

    void Print() const;
    void PrintCapacity() const;
};