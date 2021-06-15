
#include "diamond.h"
#include <iostream>

CDiamond::CDiamond( int quantity )
: CItem( quantity , 5000 , 5 )
{}

void CDiamond::PrintAdded( int quantity )
{
    std::cout << "+ " << quantity;
    std::cout << "DIAMOND" << std::endl;
}

void CDiamond::Print()
{
    std::cout << "\033[34m" << "@" << "\033[0m" << "    DIAMOND      │";
}