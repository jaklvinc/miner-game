#pragma once

#include "player.h"
#include "entityHandler.h"
#include <queue>

class CGame
{
    private:
        CMap m_Map;
        CPlayer m_Player;
        CEntityHandler m_Entities;

        std::string getFileName(const std::string &filePath);
        bool LoadGame(std::string &file);
        void PrintDirections();
        void ClearScreen();
        void PrintMap();
        

    public:
        void Welcome();
        bool StartGame();
        void SaveAndQuit();
        void Play();
};