#pragma once

#include "player.h"
#include "map.h"
#include "trap.h"
#include <queue>

class CGame
{
    private:
        CMap m_Map;
        CPlayer m_Player;
        std::queue<std::shared_ptr<CTrap>> m_Traps;

        std::string getFileName(const std::string &filePath);
        bool LoadGame(std::string &file);
        void PrintDirections();
        void ClearScreen();
        void PrintMap();
        bool LoadTraps(std::string filename);
        bool SaveTraps(std::string filename);
        void ShowTraps( std::vector<std::vector<char>> & toPrint );
        void MoveTraps();
        void AttackTraps();

    public:
        void Welcome();
        bool StartGame();
        void SaveAndQuit();
        void Play();
};