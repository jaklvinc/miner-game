#pragma once

#include <map>

class CInventory
{
    private:
        std::map<char,int> m_Inv;
    
    public:
        void AddToInv( char Type, int Quantity );
};