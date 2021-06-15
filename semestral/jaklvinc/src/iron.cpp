
#include "iron.h"
#include <iostream>

CIron::CIron( int quantity )
: CItem( quantity , 200 , 10 )
{}

void CIron::PrintAdded( int quantity )
{
    std::cout << "+ " << quantity;
    std::cout << "IRON" << std::endl;
}

void CIron::Print()
{
    std::cout << "\033[34m" << "@" << "\033[0m" << "    IRON         │";
}