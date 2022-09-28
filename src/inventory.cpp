
#include "inventory.h"
#include "enum.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

bool CInventory::InitInv(const std::string filename,const int BackpackLvl)
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

        m_Inv[STONE]=std::make_shared<CStone>(stoneCnt);
        m_Inv[IRON]=std::make_shared<CIron>(ironCnt);
        m_Inv[GOLD]=std::make_shared<CGold>(goldCnt);
        m_Inv[DIAMOND]=std::make_shared<CDiamond>(diamondCnt);
        m_Inv[CORAL]=std::make_shared<CCoral>(coralCnt);
        m_Inv[BONE]=std::make_shared<CBone>(boneCnt);

        for ( int i = 0 ; i < SIZE ; i++ )
            m_currentLoad += m_Inv[i]->GetActualWeight();
        if (m_currentLoad > m_maxLoad)
            return false;

        return true;
    }
    else
        return false;
}

bool CInventory::SaveInv(const std::string filename) const
{

    std::ofstream data;
    data.open(filename, std::ios_base::app);
    if (data.is_open())
    {
        for ( int i = 0 ; i < SIZE-1 ; i++ )
        {
            m_Inv[i]->Save(data); 
            data<< ' ';
        }
        
        m_Inv[SIZE-1]->Save(data);
        data << std::endl;
        return true;
    }
    return false;
}

void CInventory::UpdateSize( int size )
{
    m_maxLoad = size;
}

void CInventory::Reset()
{
    m_currentLoad = 0;
    for (int i = 0; i < SIZE; i++)
    {
        m_Inv[i]->Reset();
    }
}

void CInventory::Die()
{
    int newCurLoad = 0;
    for (int i = 0; i < SIZE; i++)
    {
        newCurLoad += m_Inv[i]->Die();
    }
    m_currentLoad = newCurLoad;
    return;
}

void CInventory::AddToInv(const int type, const int quantity , const int backpackLvl)
{
    m_maxLoad = backpackLvl * 150;
    for (int i = 0; i < quantity; i++)
    {
        if (m_currentLoad + m_Inv[type]->GetWeight() > m_maxLoad)
        {   
            std::cout << "INVENTORY FULL" << std::endl;
            return;
        }
        else
        {
            m_Inv[type]->AddQuantity(1);
            m_currentLoad += m_Inv[type]->GetWeight();
        }
    }
    m_Inv[type]->PrintAdded(quantity);
    return;
}

int CInventory::GetWeight(const int type) const
{
    return m_Inv[type]->GetWeight();
}

int CInventory::GetPrice(const int type) const
{
    return m_Inv[type]->GetPrice();
}

int CInventory::GetQuantity(const int type) const
{
    return m_Inv[type]->GetQuantity();
}

void CInventory::PrintLine(const int type) const
{
    std::cout << "    ";
    std::cout << m_Inv[type]->GetWeight();
    for (size_t i = 0; i < 19 - std::to_string(m_Inv[type]->GetWeight()).length(); i++)
        std::cout << ' ';

    std::cout << "│   ";
    std::cout << m_Inv[type]->GetQuantity();
    for (size_t i = 0; i < 15 - std::to_string(m_Inv[type]->GetQuantity()).length(); i++)
        std::cout << ' ';

    std::cout << "│      ";
    std::cout << m_Inv[type]->GetActualWeight();
    for (size_t i = 0; i < 19 - std::to_string(m_Inv[type]->GetActualWeight()).length(); i++)
        std::cout << ' ';
    std::cout << "\033[34m"
              << "@"
              << "\033[0m" << std::endl;
    return;
}

int CInventory::GetSize() const
{
    return m_maxLoad;
}

int CInventory::GetLoad() const
{
    return m_currentLoad;
}

void CInventory::Print() const
{
    std::cout << std::string(100, '\n');
    std::cout << "\033[34m" << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
              << "\033[0m" << std::endl;
    std::cout << "\033[34m" << "@" << "\033[0m" << "    ITEM TYPE    │    WEIGHT PER ITEM    │   IN INVENTORY   │      TOTAL WEIGHT       "
              << "\033[34m" << "@" << "\033[0m" << std::endl;
    std::cout << "\033[34m" << "@" << "\033[0m" << "─────────────────┼───────────────────────┼──────────────────┼─────────────────────────"
              << "\033[34m" << "@" << "\033[0m" << std::endl;
    m_Inv[BONE]->Print();
    PrintLine(BONE);
    m_Inv[CORAL]->Print();
    PrintLine(CORAL);
    m_Inv[STONE]->Print();
    PrintLine(STONE);
    m_Inv[IRON]->Print();
    PrintLine(IRON);
    m_Inv[GOLD]->Print();
    PrintLine(GOLD);
    m_Inv[DIAMOND]->Print();
    PrintLine(DIAMOND);
    std::cout << "\033[34m" << "@" << "\033[0m" << "─────────────────┴───────────────────────┴──────────────────┴─────────────────────────"
              << "\033[34m" << "@" << "\033[0m" << std::endl;
    std::cout << "\033[34m" << "@" << "\033[31m" << "  TOTAL CAPACITY                                                   " << m_currentLoad << "/" << m_maxLoad << "\033[34m";
    for (size_t i = 0; i < 19 - std::to_string(m_currentLoad).length() - std::to_string(m_maxLoad).length() - 1; i++)
        std::cout << ' ';
    std::cout << "\033[34m" << "@" << "\033[0m" << std::endl;
    std::cout << "\033[34m" << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << "\033[0m" << std::endl;
    return;
}

void CInventory::PrintCapacity() const
{
    std::cout << "\033[33m"
              << "INVENTORY CAPACITY: " << m_currentLoad << "/" << m_maxLoad << "\033[0m" << std::endl;
}

