
#include "gold.h"
#include <iostream>

CGold::CGold( int quantity )
: CItem( quantity , 500 , 15 )
{}

void CGold::PrintAdded( int quantity )
{
    std::cout << "+ " << quantity;
    std::cout << "GOLD" << std::endl;
}

void CGold::Print()
{
    std::cout << "\033[34m" << "@" << "\033[0m" << "    GOLD         │";
}