#include "map.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

CMap::CMap ( )
{
    m_Height = 0;
    m_Width  = 0;
}

std::shared_ptr<CTile> & CMap::GetOnIndex(int x , int y)
{
    //checks if asked index is in the map actually
    if ( x > int ( m_Map.size() ) || y > int ( m_Map[1].size() ) )
    {
        throw std::runtime_error("Searched tile not part of the map! \n");
    }
    else
        return m_Map[y][x];
        
}

bool CMap::Save(  )
{
    std::string name = "saves/map_save.dat";

    //removes old save
    remove(name.c_str());

    //creates a new save
    std::ofstream data(name);
    if (data.is_open())
    {
        // first saves the height and width of the map
        data << m_Width << " " << m_Height << std::endl;

        //then the actual tiles 
        for ( int y = 0 ; y < m_Height ; y++ )
        {
            for ( int x = 0 ; x < m_Width ; x++)
            {
                data << m_Map[y][x]->getType() << ' ' ;
            }
            data << std::endl;
        }
        return true;
        
    }
    else
        return false;
}

bool CMap::Load(  )
{
    std::string name = "./saves/map_save.dat";

    //checks if some save file exists
    if ( std::filesystem::exists(name) )
    {
        std::ifstream data;
        data.open(name);
        if (data.is_open())
        {
            
            std::string line;
            std::getline(data,line);

            std::stringstream ss(line);
            ss >> m_Width >> m_Height;
            if ( m_Width == 0 || m_Height == 0 )
            {
                return false;
            }
            for ( int y = 0 ; y < m_Height ; y++ )
            {
                std::getline(data,line);
                std::stringstream ss(line);
                
                std::vector< std::shared_ptr<CTile> > rowTmp;
                for ( int x = 0 ; x < m_Width ; x++ )
                {
                    char type = 0;
                    ss >> type;

                    switch(type)
                    {
                        case 'S':
                            {
                            auto tile = std::make_shared<CStoneB>();
                            rowTmp.push_back(tile);
                            break;
                            }
                        case 'B':
                            {
                            auto tile = std::make_shared<CBoneB>();
                            rowTmp.push_back(tile);
                            break;
                            }
                        case 'C':
                            {
                            auto tile = std::make_shared<CCoralB>();
                            rowTmp.push_back(tile);
                            break;
                            }
                        case 'I':
                            {
                            auto tile = std::make_shared<CIronB>();
                            rowTmp.push_back(tile);
                            break;
                            }
                        case 'G':
                            {
                            auto tile = std::make_shared<CGoldB>();
                            rowTmp.push_back(tile);
                            break;
                            }
                        case 'D':
                            {
                            auto tile = std::make_shared<CDiamondB>();
                            rowTmp.push_back(tile);
                            break;
                            }
                        case 's':
                            {
                            auto tile = std::make_shared<CDarkStoneB>();
                            rowTmp.push_back(tile);
                            break;
                            }
                        case 'A':
                            {
                            auto tile = std::make_shared<CAirB>();
                            rowTmp.push_back(tile);
                            break;
                            }
                        default:
                            auto tile = std::make_shared<CTile>();
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
    return false;
}