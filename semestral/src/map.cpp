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

const CTile &CMap::GetOnIndex(int x, int y) const
{
    if (x >= m_Width || x < 0 || y >= m_Height || y < 0)
    {
        throw std::runtime_error("Searched tile not part of the map! \n");
    }
    else
        return m_Map[y][x];
}

void CMap::SetOnIndex(int x, int y, CTile newTile)
{
    if (x >= m_Width || x < 0 || y >= m_Height || y < 0)
    {
        throw std::runtime_error("Searched tile not part of the map! \n");
    }
    else
        m_Map[y][x] = newTile;
}

bool CMap::Save(const std::string & filename)
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
                int type = m_Map[y][x].GetType();
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

bool CMap::Load(const std::string & filename)
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

            std::vector<CTile> rowTmp;
            for (int x = 0; x < m_Width; x++)
            {
                char type;
                ss >> type;
                type -= '0';

                switch (type)
                {
                case STONE:
                {
                    CTile tile(STONE, 2);
                    rowTmp.push_back(tile);
                    break;
                }
                case BONE:
                {
                    CTile tile(BONE, 1);
                    rowTmp.push_back(tile);
                    break;
                }
                case CORAL:
                {
                    CTile tile(CORAL, 1);
                    rowTmp.push_back(tile);
                    break;
                }
                case IRON:
                {
                    CTile tile(IRON, 2);
                    rowTmp.push_back(tile);
                    break;
                }
                case GOLD:
                {
                    CTile tile(GOLD, 3);
                    rowTmp.push_back(tile);
                    break;
                }
                case DIAMOND:
                {
                    CTile tile(DIAMOND, 4);
                    rowTmp.push_back(tile);
                    break;
                }
                case '.'-'0':
                {
                    CTile tile('.',1);
                    rowTmp.push_back(tile);
                    break;
                }
                default:
                    return false;
                }
            }
            m_Map.push_back(rowTmp);
        }
        return true;
    }
    else
        return false;
}

void CMap::ShowMap(std::vector<std::vector<char>> & toPrint) const
{
    for (int y = 0; y < m_Height; y++)
    {
        std::vector<char> rowTmp;

        for (int x = 0; x < m_Width; x++)
        {
            int type = m_Map[y][x].GetType();
            if (type == '.')
                toPrint[y][x]='.';

            else if (type == STONE)
                toPrint[y][x]='s';

            else if (type == IRON)
                toPrint[y][x]='i';

            else if (type == GOLD)
                toPrint[y][x]='g';

            else if (type == CORAL)
                toPrint[y][x]='c';

            else if (type == DIAMOND)
                toPrint[y][x]='d';

            else if (type == BONE)
                toPrint[y][x]='b';

            else 
                toPrint[y][x]='.';
        }
        toPrint.push_back(rowTmp);
    }
    return;
}

void CMap::Regenerate()
{
    for (int y = 1; y < m_Height-1; y++)
    {
        for (int x = 0; x < m_Width; x++)
        {
            if ( m_Map[y][x].GetType()=='.' && m_Map[y+1][x].GetType()!=CORAL && m_Map[y+1][x].GetType()!='.' )
            {
                int random = rand() % 500;
                if ( random <= 1 )
                {
                    CTile coral(CORAL,1);
                    m_Map[y][x] = coral;
                }
            }
        }
    }
}

int CMap::GetWidth() const
{
    return m_Width;
}

int CMap::GetHeight() const
{
    return m_Height;
}