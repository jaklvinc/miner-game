
#include "bone.h"
#include <iostream>

CBone::CBone( int quantity )
: CItem( quantity , 20 , 25 )
{}

void CBone::PrintAdded( int quantity )
{
    std::cout << "+ " << quantity;
    std::cout << "BONE" << std::endl;
}

void CBone::Print()
{
    std::cout << "\033[34m" << "@" << "\033[0m" << "    BONE         │";
}