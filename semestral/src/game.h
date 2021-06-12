#pragma once

#include "player.h"
#include "map.h"
#include "trap.h"
#include <vector>

class CGame
{
    private:
        CMap m_Map;
        CPlayer m_Player;
        std::vector<std::shared_ptr<CTrap>> m_Traps;

        std::string getFileName(const std::string &filePath);
        bool LoadGame(std::string &file);
        void PrintDirections();

    public:
        void Welcome();
        bool StartGame();
        void SaveAndQuit();
        void Play();
};