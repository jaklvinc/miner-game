
#include "inventory.h"
#include <fstream>
#include <sstream>

bool CInventory::InitInv ( std::string filename )
{
    
    std::ifstream setup;
    setup.open(filename);

    if ( setup.is_open() )
    {
        
    }
    else 
        return false;
}

void CInventory::AddToInv( char Type, int Quantity )
{

}