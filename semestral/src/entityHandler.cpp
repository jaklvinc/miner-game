
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
                if ( x < 0 || y < 0 || x >= map.getWidth() || y >= map.getHeight() )
                    return false;
                auto bomb = std::make_shared<CBomb>(x,y);
                m_Entities.push(bomb);
            }
            else if ( type == 'F')
            {
                int x , y , cooldown, move;
                ss >> x >> y >> cooldown >> move;
                if ( x < 0 || y < 0 || x >= map.getWidth() || y >= map.getHeight() || cooldown < 0 || cooldown > 7 || move > 1 || move < 0 )
                    return false;
                auto fairy = std::make_shared<CFairy>(x,y,cooldown,move);
                m_Entities.push(fairy);
            }
            else if ( type == 'T' )
            {
                int x , y;
                ss >> x >> y;
                if ( x < 0 || y < 0 || x >= map.getWidth() || y >= map.getHeight() )
                    return false;
                auto teleport = std::make_shared<CTeleport>(x,y);
                m_Entities.push(teleport);
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
            m_Entities.pop();
            entity->Save(data);
        }
        return true;
    }
    else
        return false;
}

void CEntityHandler::ShowEntities( std::vector<std::vector<char>> & toPrint )
{
    for ( size_t i = 0 ; i < m_Entities.size() ; i++ )
    {
        auto entity=m_Entities.front();
        m_Entities.pop();
        int x = entity->GetX();
        int y = entity->GetY();
        char appearance = entity->Appearance();
        if( appearance != '.' )
            toPrint[y][x]=appearance;
        m_Entities.push(entity);
    }
}

void CEntityHandler::MoveEntities( CPlayer & player , CMap & map )
{
    for ( size_t i = 0 ; i < m_Entities.size() ; i++ )
    {
        auto entity=m_Entities.front();
        m_Entities.pop();
        entity->Move(player,map);
        m_Entities.push(entity);
    }
}

void CEntityHandler::ActionEntities( CPlayer & player , CMap & map )
{
    for ( size_t i = 0 ; i < m_Entities.size() ; i++ )
    {
        auto entity=m_Entities.front();
        m_Entities.pop();
        entity->Action(player,map);
        if (! entity->GetUsed())
            m_Entities.push(entity);
    }
}