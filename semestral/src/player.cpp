#include "player.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "enum.h"

CPlayer::CPlayer(int x, int y) : m_PosX(x) , m_PosY(y)
{
}

int CPlayer::GetX() const
{
    return m_PosX;
}

int CPlayer::GetY() const
{
    return m_PosY;
}

std::pair<int, int> CPlayer::NewPos(const char dir, const int width, const int height) const
{
    int moveX = 0;
    int moveY = 0;

    switch (dir)
    {
    case 'w':
        moveY = -1;
        break;
    case 's':
        moveY = 1;
        break;
    case 'd':
        moveX = 1;
        break;
    case 'a':
        moveX = -1;
        break;
    }
    int new_X = m_PosX;
    int new_Y = m_PosY;
    if (new_X + moveX >= 0 && new_X + moveX < width && new_Y + moveY >= 0 && new_Y + moveY < height)
    {
        new_X += moveX;
        new_Y += moveY;
    }
    return std::make_pair(new_X, new_Y);
}

bool CPlayer::Load(std::string filename)
{

    std::ifstream data;
    data.open(filename);
    if (data.is_open())
    {
        std::string line;
        std::getline(data, line);
        std::stringstream ss(line);

        ss >> m_PosX >> m_PosY;

        if (m_PosY < 0 || m_PosY < 0)
        {
            return false;
        }

        std::getline(data, line);
        ss = std::stringstream(line);
        ss >> m_LvlLight >> m_LvlTank >> m_LvlDrill >> m_LvlBackpack >> m_Money;
        if (m_LvlLight > 4 || m_LvlLight < 1 || m_LvlTank > 4 || m_LvlTank < 1 ||
            m_LvlDrill > 4 || m_LvlDrill < 1 || m_LvlBackpack > 4 || m_LvlBackpack < 1 || m_Money < 0)
        {
            return false;
        }

        std::getline(data, line);
        m_Health = stoi(line);
        if (m_Health < 1)
        {
            return false;
        }
        m_MaxHealth = 150 * m_LvlTank;

        if (!m_Inv.InitInv(filename, m_LvlBackpack))
            return false;

        return true;
    }
    else
        return false;
}

bool CPlayer::Save(std::string filename)
{
    std::ofstream data;
    data.open(filename);
    if (data.is_open())
    {
        data << m_PosX << ' ' << m_PosY << ' ' << std::endl;
        data << m_LvlLight << ' ' << m_LvlTank << ' ' << m_LvlDrill << ' ' << m_LvlBackpack << ' ' << m_Money << std::endl;
        data << m_Health << std::endl;

        data.close();
        if (!m_Inv.SaveInv(filename))
        {
            return false;
        }
        return true;
    }
    return false;
}

int CPlayer::GetLight() const
{
    return m_LvlLight;
}

CInventory &CPlayer::GetInv()
{
    return m_Inv;
}

void CPlayer::Move(const CMap &map, char dir)
{
    std::pair<int, int> new_pos;
    new_pos = NewPos(dir, map.getWidth(), map.getHeight());
    m_PosX = new_pos.first;
    m_PosY = new_pos.second;
    return;
}

int CPlayer::MoveCost(const CMap &map, char dir) const
{
    std::pair<int, int> new_pos;
    new_pos = NewPos(dir, map.getWidth(), map.getHeight());
    if (new_pos.first == m_PosX && new_pos.second == m_PosY)
        return 0;
    else
        return map.GetOnIndex(new_pos.first, new_pos.second)->getToughtness();
}

void CPlayer::Mine(CMap & map)
{
    auto new_tile = std::make_shared<CAirB>();
    std::shared_ptr<CTile> tile = map.GetOnIndex(m_PosX, m_PosY);
    int tileType = tile->getType();

    if (tileType != '.')
    {
        if (tileType == STONE)
            m_Inv.AddToInv(STONE, 1);
        else
            m_Inv.AddToInv(tileType, (rand() % m_LvlDrill) + 1);
    }

    map.SetOnIndex(m_PosX, m_PosY, new_tile);
    return;
}

void CPlayer::OxDown(int amount)
{
    m_Health -= amount;
    if (m_Health <= 0)
    {
        Die();
    }
    return;
}

void CPlayer::OxReset()
{
    m_Health = m_MaxHealth;
}

void CPlayer::Die()
{
    m_PosY = 0;
    OxReset();
    m_Inv.Die();
}

bool CPlayer::UpgradeEquipent(int type)
{
    if (type == 1)
    {
        if (m_Money >= m_LvlLight * 5000)
        {
            m_Money -= m_LvlLight * 5000;
            m_LvlLight++;
        }
        else
            return false;
    }
    else if (type == 2)
    {
        if (m_Money >= m_LvlTank * 5000)
        {
            m_Money -= m_LvlTank * 5000;
            m_LvlTank++;
        }
        else
            return false;
    }
    else if (type == 3)
    {
        if (m_Money >= m_LvlDrill * 5000)
        {
            m_Money -= m_LvlDrill * 5000;
            m_LvlDrill++;
        }
        else
            return false;
    }
    else if (type == 4)
    {
        if (m_Money >= m_LvlBackpack * 5000)
        {
            m_Money -= m_LvlBackpack * 5000;
            m_LvlBackpack++;
        }
        else
            return false;
    }
    return true;
}

void CPlayer::PrintStats() const
{
    std::cout << "\033[31m"
              << "OXYGEN: " << m_Health << "\033[34m"
              << " |  LIGHT LVL: " << m_LvlLight << " |  OXYGEN TANK LVL: " << m_LvlTank << " |  DRILL LVL: " << m_LvlDrill << " |  BACKPACK LVL: " << m_LvlBackpack << std::endl;
    std::cout << "\033[32m"
              << "MONEY: " << m_Money << "\033[0m" << std::endl;
}

void CPlayer::PrintInvCapacity() const
{
    m_Inv.PrintCapacity();
}

void CPlayer::Inventory()
{
    char sell;
    m_Inv.Print();
    std::cout << "\033[32m"
              << "MONEY: " << m_Money << "\033[0m" << std::endl;
    std::cout << "Press \'s\' to sell all mined items, or any other button to leave the inventory." << std::endl;
    while (std::cin >> sell)
    {
        if (sell != 's')
            break;

        if (m_PosY == 0)
        {
            int money = 0;
            for (int i = 0; i < SIZE; i++)
            {
                money += m_Inv.GetPrice(i) * m_Inv.GetQuantity(i);
            }
            m_Inv.Reset();
            m_Money += money;
            m_Inv.Print();
            std::cout << "\033[32m"
                      << "MONEY: " << m_Money << "\033[0m" << std::endl;
            std::cout << "You've sold your mined items for: " << money << std::endl;
        }
        else
        {
            m_Inv.Print();
            std::cout << "\033[32m"
                      << "MONEY: " << m_Money << "\033[0m" << std::endl;
            std::cout << "You need to be on the surface to sell items from your inventory." << std::endl;
        }
    }
    return;
}

void CPlayer::PrintEquipLvl(int lvl) const
{
    for (int i = 0; i < lvl; i++)
    {
        std::cout << "█  ";
    }
    for (int i = 0; i < 4 - lvl; i++)
    {
        std::cout << "░  ";
    }
}

void CPlayer::PrintShop() const
{
    std::cout << std::string(100, '\n');
    std::cout << "\033[34m"
              << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
              << "\033[0m" << std::endl;
    std::cout << "\033[34m"
              << "@"
              << "\033[0m"
              << "                                "
              << "\033[34m"
              << "@"
              << "\033[0m" << std::endl;
    std::cout << "\033[34m"
              << "@"
              << "\033[0m"
              << "  1.  LIGHT :       ";
    PrintEquipLvl(m_LvlLight);
    std::cout << "\033[34m"
              << "@"
              << "\033[0m" << std::endl;
    std::cout << "\033[34m"
              << "@"
              << "\033[0m"
              << "                                "
              << "\033[34m"
              << "@"
              << "\033[0m" << std::endl;
    std::cout << "\033[34m"
              << "@"
              << "\033[0m"
              << "  2.  OXYGEN TANK : ";
    PrintEquipLvl(m_LvlTank);
    std::cout << "\033[34m"
              << "@"
              << "\033[0m" << std::endl;
    std::cout << "\033[34m"
              << "@"
              << "\033[0m"
              << "                                "
              << "\033[34m"
              << "@"
              << "\033[0m" << std::endl;
    std::cout << "\033[34m"
              << "@"
              << "\033[0m"
              << "  3.  DRILL :       ";
    PrintEquipLvl(m_LvlDrill);
    std::cout << "\033[34m"
              << "@"
              << "\033[0m" << std::endl;
    std::cout << "\033[34m"
              << "@"
              << "\033[0m"
              << "                                "
              << "\033[34m"
              << "@"
              << "\033[0m" << std::endl;
    std::cout << "\033[34m"
              << "@"
              << "\033[0m"
              << "  4.  BACKPACK :    ";
    PrintEquipLvl(m_LvlBackpack);
    std::cout << "\033[34m"
              << "@"
              << "\033[0m" << std::endl;
    std::cout << "\033[34m"
              << "@"
              << "\033[0m"
              << "                                "
              << "\033[34m"
              << "@"
              << "\033[0m" << std::endl;
    std::cout << "\033[34m"
              << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
              << "\033[0m" << std::endl;
    if (m_LvlLight < 4)
        std::cout << "To upgrade LIGHT to lvl " << m_LvlLight + 1 << " for " << m_LvlLight * 5000 << " press 1" << std::endl;
    else
        std::cout << "LIGHT is already max lvl." << std::endl;

    if (m_LvlTank < 4)
        std::cout << "To upgrade OXYGEN TANK to lvl " << m_LvlTank + 1 << " for " << m_LvlTank * 5000 << " press 2" << std::endl;
    else
        std::cout << "OXYGEN TANK is already max lvl." << std::endl;

    if (m_LvlDrill < 4)
        std::cout << "To upgrade DRILL to lvl " << m_LvlDrill + 1 << " for " << m_LvlDrill * 5000 << " press 3" << std::endl;
    else
        std::cout << "DRILL is already max lvl." << std::endl;

    if (m_LvlBackpack < 4)
        std::cout << "To upgrade BACKPACK to lvl " << m_LvlBackpack + 1 << " for " << m_LvlBackpack * 5000 << " press 4" << std::endl;
    else
        std::cout << "BACKPACK is already max lvl." << std::endl;
}

void CPlayer::Shop()
{
    this->PrintShop();
    std::cout << "\033[32m"
              << "MONEY: " << m_Money << "\033[0m" << std::endl;
    char type;
    while (std::cin >> type)
    {
        if (type != '1' && type != '2' && type != '3' && type != '4')
        {
            break;
        }

        if (UpgradeEquipent(type - '0'))
        {
            this->PrintShop();
            std::cout << "\033[32m"
                      << "MONEY: " << m_Money << "\033[0m" << std::endl;
        }
        else
        {
            this->PrintShop();
            std::cout << "\033[32m"
                      << "MONEY: " << m_Money << "\033[0m" << std::endl;
            std::cout << "Unsufficient balance" << std::endl;
        }
    }
    return;
}
