#pragma once

#include "fairy.h"
#include "bomb.h"
#include "teleport.h"
#include <queue>

class CEntityHandler
{
    private:
        std::queue<std::shared_ptr<CEntity>> m_Entities;
    
    public:
        bool LoadEntities(std::string filename , CMap & map);
        bool SaveEntities(std::string filename);
        void ShowEntities( std::vector<std::vector<char>> & toPrint );
        void MoveEntities( CPlayer & player , CMap & map );
        void ActionEntities( CPlayer & player , CMap & map );
};