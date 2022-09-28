
#include "coral.h"
#include <iostream>

CCoral::CCoral( int quantity )
: CItem( quantity , 100 , 5 )
{}

void CCoral::PrintAdded( int quantity )
{
    std::cout << "+ " << quantity;
    std::cout << "CORAL" << std::endl;
}

void CCoral::Print()
{
    std::cout << "\033[34m" << "@" << "\033[0m" << "    CORAL        │";
}