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

public:
    CInventory();
    void AddToInv(int type, int quantity);
    bool InitInv(std::string filename, int backpackLvl);
    bool SaveInv(std::string filename) const;
    void Die();

    int GetWeight( int type ) const;

    void PrintLine( int type ) const;
    void Print( ) const;
    
};