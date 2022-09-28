
#include "entityHandler.h"
#include <sstream>

bool CEntityHandler::LoadEntities( std::string filename , CMap & map )
{
    std::ifstream data;
    data.open(filename);

    if (data.is_open())
    {
        std::string line;
        for (int i = 0; i < 6; i++)
        std::getline(data, line);
        int width, height;
        std::stringstream ss(line);

        ss >> width >> height;

        for ( int i = 0; i < height ; i++)
        std::getline(data,line);

        while ( std::getline(data,line) )
        {
            char type;
            std::stringstream ss(line);
            ss >> type;

            if ( type == 'B' )
            {
                int x , y;
                ss >> x >> y;
                if ( x < 0 || y < 0 || x >= map.GetWidth() || y >= map.GetHeight() )
                    return false;
                auto bomb = std::make_shared<CBomb>(x,y);
                m_Entities.push_back(bomb);
            }
            else if ( type == 'F')
            {
                int x , y , cooldown, move;
                ss >> x >> y >> cooldown >> move;
                if ( x < 0 || y < 0 || x >= map.GetWidth() || y >= map.GetHeight() || cooldown < 0 || cooldown > 7 || move > 1 || move < 0 )
                    return false;
                auto fairy = std::make_shared<CFairy>(x,y,cooldown,move);
                m_Entities.push_back(fairy);
            }
            else if ( type == 'T' )
            {
                int x , y;
                ss >> x >> y;
                if ( x < 0 || y < 0 || x >= map.GetWidth() || y >= map.GetHeight() )
                    return false;
                auto teleport = std::make_shared<CTeleport>(x,y);
                m_Entities.push_back(teleport);
            }
            else
                return false;
        }
        return true;
    }
    else
        return false;
}

bool CEntityHandler::SaveEntities( std::string filename )
{
    std::ofstream data(filename, std::ios_base::app);
    if (data.is_open())
    {
        while ( ! m_Entities.empty() )
        {
            std::shared_ptr<CEntity> entity = m_Entities.front();
            m_Entities.pop_front();
            entity->Save(data);
        }
        return true;
    }
    else
        return false;
}

void CEntityHandler::ShowEntities( std::vector<std::vector<char>> & toPrint ) const
{
    for ( auto entity : m_Entities)
    {
        int x = entity->GetX();
        int y = entity->GetY();
        char appearance = entity->Appearance();
        if( appearance != '.' )
            toPrint[y][x]=appearance;
    }
}

void CEntityHandler::MoveEntities( CPlayer & player , CMap & map )
{
    for ( auto entity : m_Entities )
    {
        entity->Move(player,map);
    }
}

void CEntityHandler::ActionEntities( CPlayer & player , CMap & map )
{
    for ( size_t i = 0 ; i < m_Entities.size() ; i++ )
    {
        auto entity=m_Entities.front();
        m_Entities.pop_front();
        entity->Action(player,map);
        if (! entity->GetUsed())
            m_Entities.push_back(entity);
    }
}