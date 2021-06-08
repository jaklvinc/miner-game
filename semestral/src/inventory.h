#pragma once

#include <map>
#include <string>

class CInventory
{
    private:
        std::map<std::string,int> m_Inv;
        int max_size;
    
    public:
        void AddToInv( char Type, int Quantity );
        bool InitInv ( std::string filename );
};