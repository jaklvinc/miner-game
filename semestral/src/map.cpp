#include "map.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include "enum.h"

CMap::CMap()
{
    m_Height = 0;
    m_Width = 0;
}

const std::shared_ptr<CTile> &CMap::GetOnIndex(int x, int y) const
{
    //checks if asked index is in the map actually
    if (x >= m_Width || x < 0 || y >= m_Height || y < 0)
    {
        throw std::runtime_error("Searched tile not part of the map! \n");
    }
    else
        return m_Map[y][x];
}

void CMap::SetOnIndex(int x, int y, std::shared_ptr<CTile> new_tile)
{
    if (x >= m_Width || x < 0 || y >= m_Height || y < 0)
    {
        throw std::runtime_error("Searched tile not part of the map! \n");
    }
    else
        m_Map[y][x] = new_tile;
    return;
}

bool CMap::Save(std::string filename)
{
    std::string name = filename;

    //creates a new save
    std::ofstream data(name, std::ios_base::app);
    if (data.is_open())
    {
        // first saves the height and width of the map
        data << m_Width << " " << m_Height << std::endl;

        //then the actual tiles
        for (int y = 0; y < m_Height; y++)
        {
            for (int x = 0; x < m_Width; x++)
            {
                int type = m_Map[y][x]->getType();
                if (type < 10)
                    data << type;
                else
                    data << '.';
            }
            data << std::endl;
        }
        return true;
    }
    else
        return false;
}

bool CMap::Load(std::string filename)
{
    std::string name = filename;

    //checks if some save file exists
    std::ifstream data;
    data.open(name);
    if (data.is_open())
    {

        std::string line;

        for (int i = 0; i < 6; i++)
            std::getline(data, line);

        std::stringstream ss(line);
        ss >> m_Width >> m_Height;
        if (m_Width <= 0 || m_Height <= 0)
        {
            return false;
        }
        for (int y = 0; y < m_Height; y++)
        {
            std::getline(data, line);
            std::stringstream ss(line);

            std::vector<std::shared_ptr<CTile>> rowTmp;
            for (int x = 0; x < m_Width; x++)
            {
                char type;
                ss >> type;
                type -= '0';

                switch (type)
                {
                case STONE:
                {
                    auto tile = std::make_shared<CStoneB>();
                    rowTmp.push_back(tile);
                    break;
                }
                case BONE:
                {
                    auto tile = std::make_shared<CBoneB>();
                    rowTmp.push_back(tile);
                    break;
                }
                case CORAL:
                {
                    auto tile = std::make_shared<CCoralB>();
                    rowTmp.push_back(tile);
                    break;
                }
                case IRON:
                {
                    auto tile = std::make_shared<CIronB>();
                    rowTmp.push_back(tile);
                    break;
                }
                case GOLD:
                {
                    auto tile = std::make_shared<CGoldB>();
                    rowTmp.push_back(tile);
                    break;
                }
                case DIAMOND:
                {
                    auto tile = std::make_shared<CDiamondB>();
                    rowTmp.push_back(tile);
                    break;
                }
                default:
                    auto tile = std::make_shared<CAirB>();
                    rowTmp.push_back(tile);
                }
            }
            m_Map.push_back(rowTmp);
        }
        return true;
    }
    else
        return false;
}

void CMap::ShowMap(int playerX, int playerY, int lightLvl)
{
    std::cout << std::string(30, '\n');
    // TODO CLEAR SCREEN
    for (int y = -1; y <= m_Height; y++)
    {
        for (int x = -1; x <= m_Width; x++)
        {
            if (y == -1 || x == -1 || y == m_Height || x == m_Width)
            {
                std::cout << '@';
            }
            else if (playerX == x && playerY == y)
            {
                std::cout << "\033[1m\033[34m"
                          << "╬"
                          << "\033[0m";
            }
            else if (abs(playerX - x) <= lightLvl && abs(playerY - y) <= lightLvl / 2 + 1)
            {
                int type = m_Map[y][x]->getType();
                if (type == '.')
                    std::cout << ' ';

                else if (type == STONE)
                    std::cout << "▓";

                else if (type == IRON)
                    std::cout << "\033[31m"
                              << "▓"
                              << "\033[0m";

                else if (type == GOLD)
                    std::cout << "\033[33m"
                              << "▓"
                              << "\033[0m";

                else if (type == CORAL)
                    std::cout << "\033[32m" << '?' << "\033[0m";

                else if (type == DIAMOND)
                    std::cout << "\033[36m"
                              << "▓"
                              << "\033[0m";

                else if (type == BONE)
                    std::cout << "▒";

                else
                    std::cout << type;
            }
            else if (y == 0)
            {
                std::cout << "\033[32m"
                          << "░"
                          << "\033[0m";
            }
            else
                std::cout << "░";
        }
        std::cout << std::endl;
    }
    return;
}

void CMap::Regenerate()
{
    for (int y = 1; y < m_Height-1; y++)
    {
        for (int x = 0; x < m_Width; x++)
        {
            if ( m_Map[y][x]->getType()=='.' && m_Map[y+1][x]->getType()!=CORAL && m_Map[y+1][x]->getType()!='.' )
            {
                int random = rand() % 100;
                if ( random <= 1 )
                {
                    auto coral = std::make_shared<CCoralB>();
                    m_Map[y][x] = coral;
                }
            }
        }
    }
}