
#include "inventory.h"
#include "enum.h"
#include <fstream>
#include <sstream>
#include <iostream>

CInventory::CInventory()
{
}

bool CInventory::InitInv(std::string filename, int BackpackLvl)
{
    m_maxLoad = BackpackLvl * 150;
    m_currentLoad = 0;

    std::ifstream setup;
    setup.open(filename);

    if (setup.is_open())
    {
        std::string line;

        for (int i = 0; i < 4; i++)
            std::getline(setup, line);
        std::stringstream ss(line);

        int boneCnt, coralCnt, stoneCnt, ironCnt, goldCnt, diamondCnt = 0;
        ss >> boneCnt >> coralCnt >> stoneCnt >> ironCnt >> goldCnt >> diamondCnt;
        if (boneCnt < 0 || coralCnt < 0 || stoneCnt < 0 || ironCnt < 0 || goldCnt < 0 || diamondCnt < 0)
            return false;

        m_Inv[BONE]=boneCnt;
        m_Inv[CORAL]=coralCnt;
        m_Inv[STONE]=stoneCnt;
        m_Inv[IRON]=ironCnt;
        m_Inv[GOLD]=goldCnt;
        m_Inv[DIAMOND]=diamondCnt;

        m_Weights[BONE]=25;
        m_Weights[CORAL]=5;
        m_Weights[STONE]=10;
        m_Weights[IRON]=10;
        m_Weights[GOLD]=15;
        m_Weights[DIAMOND]=5;

        m_Prices[BONE]=50;
        m_Prices[CORAL]=200;
        m_Prices[STONE]=50;
        m_Prices[IRON]=500;
        m_Prices[GOLD]=1000;
        m_Prices[BONE]=2000;

        m_currentLoad+= boneCnt*m_Weights[BONE] + coralCnt*m_Weights[CORAL] + stoneCnt*m_Weights[STONE] +
                        ironCnt*m_Weights[IRON] + goldCnt*m_Weights[GOLD] + diamondCnt*m_Weights[DIAMOND];
        if ( m_currentLoad > m_maxLoad )
            return false;
        
        return true;
    }
    else
        return false;
}

bool CInventory::SaveInv( std::string filename ) const
{
    
    std::ofstream data;
    data.open(filename, std::ios_base::app);
    if (data.is_open())
    {
        data << m_Inv[BONE] << ' ' << m_Inv[CORAL] << ' ' << m_Inv[STONE] << ' ' << m_Inv[IRON] <<  ' ' << m_Inv[GOLD] <<  ' ' << m_Inv[DIAMOND] << std::endl;
        data << std::endl;
        return true;
    }
    return false;
}

void CInventory::AddToInv(int type, int quantity)
{
    for( int i = 0; i< quantity ; i++ )
    {
        if ( m_currentLoad + m_Weights[type] > m_maxLoad )
        {
            return;
        }
        else
        {
            m_Inv[type]++;
            m_maxLoad += m_Weights[type];
        }
    }
    return;
}

int CInventory::GetWeight( int type ) const
{
    return m_Weights[type];
}

void CInventory::PrintLine( int type ) const
{
    std::cout<< "    ";
    std::cout<< m_Weights[type];
    for ( size_t i = 0 ; i < 19-std::to_string( m_Weights[type] ).length() ; i++ )
        std::cout << ' ';
    
    std::cout << "│   ";
    std::cout << m_Inv[type];
    for ( size_t i = 0 ; i < 15-std::to_string( m_Inv[type] ).length() ; i++ )
        std::cout << ' ';
    
    std::cout << "│      ";
    std::cout << m_Inv[type] * m_Weights[type];
    for ( size_t i = 0 ; i < 19-std::to_string( m_Inv[type] * m_Weights[type] ).length() ; i++ )
        std::cout << ' ';
    std::cout << "\033[34m" <<"@"<< "\033[0m" << std::endl;
    return;
}

void CInventory::Print(  ) const
{
    std::cout << std::string(30,'\n');
    std::cout << "\033[34m" << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << "\033[0m" << std::endl;
    std::cout << "\033[34m" <<"@"<< "\033[0m" << "    ITEM TYPE    │    WEIGHT PER ITEM    │   IN INVENTORY   │      TOTAL WEIGHT       " << "\033[34m" <<"@"<< "\033[0m" << std::endl;
    std::cout << "\033[34m" <<"@"<< "\033[0m" << "─────────────────┼───────────────────────┼──────────────────┼─────────────────────────" << "\033[34m" <<"@"<< "\033[0m" << std::endl;
    std::cout << "\033[34m" <<"@"<< "\033[0m" << "    BONE         │";
    PrintLine(BONE);
    std::cout << "\033[34m" <<"@"<< "\033[0m" << "    CORAL        │";
    PrintLine(CORAL);
    std::cout << "\033[34m" <<"@"<< "\033[0m" << "    STONE        │";
    PrintLine(STONE);
    std::cout << "\033[34m" <<"@"<< "\033[0m" << "    IRON         │";
    PrintLine(IRON);
    std::cout << "\033[34m" <<"@"<< "\033[0m" << "    GOLD         │";
    PrintLine(GOLD);
    std::cout << "\033[34m" <<"@"<< "\033[0m" << "    DIAMOND      │";
    PrintLine(DIAMOND);
    std::cout << "\033[34m" <<"@"<< "\033[0m" << "─────────────────┴───────────────────────┴──────────────────┴─────────────────────────" << "\033[34m" <<"@"<< "\033[0m" << std::endl;
    std::cout << "\033[34m" <<"@"<<"\033[31m" << "  TOTAL CAPACITY                                                   " << m_currentLoad << "/" << m_maxLoad<< "\033[34m";
    for ( size_t i = 0 ; i < 19-std::to_string( m_currentLoad ).length()-std::to_string( m_maxLoad ).length()-1 ; i++ )
        std::cout << ' ';
    std::cout << "\033[34m" <<"@"<< "\033[0m" << std::endl;
    std::cout << "\033[34m" <<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << "\033[0m" << std::endl;
    std::cout << "Press any button to leave the inventory."; 
    std::cout << std::endl;
    char doesntMatter;
    std::cin >> doesntMatter;
    return;
}

void CInventory::Die()
{
    
}