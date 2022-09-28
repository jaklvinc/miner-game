
#include "stone.h"
#include <iostream>

CStone::CStone( int quantity )
: CItem( quantity , 20 , 10 )
{}

void CStone::PrintAdded( int quantity )
{
    std::cout << "+ " << quantity;
    std::cout << "STONE" << std::endl;
}

void CStone::Print()
{
    std::cout << "\033[34m" << "@" << "\033[0m" << "    STONE        │";
}