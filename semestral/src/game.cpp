
#include "game.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>

std::string CGame::getFileName(const std::string &filePath)
{
    size_t i = filePath.rfind('/', filePath.length());
    if (i != std::string::npos)
    {
        return (filePath.substr(i + 1, filePath.length() - i));
    }
    return ("");
}

bool CGame::LoadGame(std::string &file)
{
    if (std::filesystem::is_empty("examples/saves"))
    {
        return false;
    }
    while (true)
    {
        std::cout << "Your saves:" << std::endl;
        for (const auto &files : std::filesystem::directory_iterator("examples/saves"))
        {
            std::cout << getFileName(files.path()) << std::endl;
        }
        std::cout << "──────────────────────────" << std::endl;
        file = "examples/saves/";
        std::string fileName;
        std::cin >> fileName;
        file.append(fileName);
        if (std::filesystem::exists(file))
            return true;
        else
        {
            std::cout << std::endl
                      << "Choose an existing save file please." << std::endl;
            continue;
        }
    }
}

void CGame::PrintLegend() const
{
    ClearScreen();
    std::cout << "ORES:" << std::endl;
    std::cout << "▓" << " STONE -> if mined always yields 1 stone (only thing not affected by drill lvl)" << std::endl << std::endl;
    std::cout << "\033[31m" << "▓" << "\033[0m" << " IRON -> if mined yields iron" << std::endl << std::endl;
    std::cout << "\033[33m" << "▓" << "\033[0m" << " GOLD -> if mined yields gold" << std::endl << std::endl;
    std::cout << "\033[36m" << "▓" << "\033[0m" << " DIAMOND -> if mined yields diamonds" << std::endl << std::endl;
    std::cout << "▒" << " BONE -> if mined yields bones" << std::endl << std::endl;
    std::cout << "\033[32m" << "?" << "\033[0m" << " CORAL -> if mined yields corals (also regrows by itself)" << std::endl << std::endl;
    
    std::cout << "ENTITIES:" << std::endl;
    std::cout << "\033[31m" << "Ø" << "\033[0m" << " BOMB -> if player gets to one square around, it will blow up a crater and takes 150 oxygen from player" << std::endl << std::endl;
    std::cout << "\033[42;1m" << "%" << "\033[0m" << " FAIRY -> if player catches it, it will refill its oxygen and disappear for a couple of turns, then it appears again" << std::endl << std::endl;
    std::cout << "\033[44;1m" << "@" << "\033[0m" << " TELEPORT -> if stepped on, it will teleport the player to the surface and randomly moves itself to a different place on a map" << std::endl << std::endl;
    char random;
    std::cin >> random;
}

void CGame::PrintDirections() const
{
    std::cout << "Press \'w,a,s,d\' to move." << std::endl;
    std::cout << "Press \'e\' to open inventory." << std::endl;
    std::cout << "Press \'q\' to save and quit." << std::endl;
    std::cout << "Press \'r\' to open shop." << std::endl;
    std::cout << "Press \'l\' to show legend." << std::endl;
    std::cout << "BE SURE TO PRESS ENTER AFTER EACH KEYPRESS" << std::endl;
}

void CGame::Welcome() const
{
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << "@                               Welcome to miner!                                @" << std::endl;
    std::cout << "@────────────────────────────────────────────────────────────────────────────────@" << std::endl;
    std::cout << "@                In this game you play as a miner in a flodded mine.             @" << std::endl;
    std::cout << "@               You are trying to make anough money to go back to your           @" << std::endl;
    std::cout << "@                          family and provide for them.                          @" << std::endl;
    std::cout << "@────────────────────────────────────────────────────────────────────────────────@" << std::endl;
    std::cout << "@               You have 4 items to help you mine more efficiently.              @" << std::endl;
    std::cout << "@                                                                                @" << std::endl;
    std::cout << "@       1. Lights help you see further and uncover ores                          @" << std::endl;
    std::cout << "@       2. Oxygen tank helps you stay under water for longer                     @" << std::endl;
    std::cout << "@       3. Drill helps you mine more items from precious ores                    @" << std::endl;
    std::cout << "@       4. Backpack lets you carry more of your mined items                      @" << std::endl;
    std::cout << "@────────────────────────────────────────────────────────────────────────────────@" << std::endl;
    std::cout << "@       Underground you can find these ores going up in rarity and price:        @" << std::endl;
    std::cout << "@              STONE , BONE , CORAL , IRON , GOLD and DIAMOND                    @" << std::endl;
    std::cout << "@────────────────────────────────────────────────────────────────────────────────@" << std::endl;
    std::cout << "@      Also watch out for different entities that float around the sea           @" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;   
}

bool CGame::StartGame()
{
    m_Win = false;
    std::cout << "Choose from the following:" << std::endl;
    std::cout << "1 = START NEW GAME" << std::endl;
    std::cout << "2 = LOAD FROM A SAVE" << std::endl;
    std::cout << "──────────────────────────" << std::endl;
    char input;
    while (std::cin >> input)
    {
        if (input == '1')
        {
            if (!m_Map.Load("examples/new_game/ng.txt"))
            {
                std::cout << "Map couldn't be loaded, your save may be corrupted, or it doesn't exist." << std::endl;
                return false;
            }
            if (!m_Player.Load("examples/new_game/ng.txt",m_Map))
            {
                std::cout << "Player data couldn't be loaded, your save may be corrupted, or it doesn't exist." << std::endl;
                return false;
            }
            if ( !m_Entities.LoadEntities("examples/new_game/ng.txt",m_Map) )
            {
                std::cout << "Entity data couldn't be loaded, your save may be corrupted." << std::endl;
                return false;
            }
            return true;
        }
        else if (input == '2')
        {
            std::string filename;
            bool loaded = LoadGame(filename);
            if (!loaded)
            {
                std::cout << "You don't have any saves yet. You will need to start a new game." << std::endl;
                return false;
            }

            if (!m_Map.Load(filename))
            {
                std::cout << "Map couldn't be loaded, your save may be corrupted." << std::endl;
                return false;
            }
            if (!m_Player.Load(filename,m_Map))
            {
                std::cout << "Player data couldn't be loaded, your save may be corrupted." << std::endl;
                return false;
            }
            if ( !m_Entities.LoadEntities(filename,m_Map) )
            {
                std::cout << "Entity data couldn't be loaded, your save may be corrupted." << std::endl;
                return false;
            }
            return true;
        }
        else
        {
            std::cout << "Please pick number 1 or 2." << std::endl;
        }
        std::cout << "──────────────────────────" << std::endl;
    }
    return false;
}

void CGame::SaveAndQuit()
{
    std::cout << std::string(100, '\n');
    std::cout << "Your saves:" << std::endl;
    for (const auto &files : std::filesystem::directory_iterator("examples/saves"))
    {
        std::cout << getFileName(files.path()) << std::endl;
    }
    std::cout << "──────────────────────────" << std::endl;
    std::cout << "Choose an existing save to override or type in a different name and a new save will be created" << std::endl;
    std::string outFilename;
    std::cin >> outFilename;

    outFilename = "examples/saves/" + outFilename;
    if (!m_Player.Save(outFilename))
    {
        std::cout << "Your player stats couldn't be saved." << std::endl;
        return;
    }

    if (!m_Map.Save(outFilename))
    {
        std::cout << "Your map couldn't be saved." << std::endl;
        return;
    }

    if (!m_Entities.SaveEntities(outFilename))
    {
        std::cout << "Entities couldn't be saved." << std::endl;
        return;
    }

    return;
}

void CGame::ClearScreen() const
{
    std::cout << std::string(100,'\n');
}

void CGame::PrintMap() const
{
    
    std::vector<std::vector<char>> toPrint( m_Map.GetHeight() , std::vector<char>( m_Map.GetWidth() , '.' ) );
    m_Map.ShowMap(toPrint);
    m_Entities.ShowEntities(toPrint);
    m_Player.ShowPos(toPrint);
    

    for ( int y = -1 ; y <= m_Map.GetHeight() ; y++  )
    {
        for ( int x = -1 ; x <= m_Map.GetWidth() ; x++ )
        {
            if (y == -1 || x == -1 || y == m_Map.GetHeight() || x == m_Map.GetWidth())
            {
                std::cout << '@';
            }
            else if (abs(m_Player.GetX() - x) <= m_Player.GetLight() && abs(m_Player.GetY() - y) <= m_Player.GetLight()/2 + 1)
            {
                int type = toPrint[y][x];
                if (type == '.')
                    std::cout<<' ';
                
                else if ( type == 's' )
                    std::cout << "▓";
                
                else if ( type == 'i' )
                    std::cout << "\033[31m"
                              << "▓"
                              << "\033[0m";
                
                else if ( type == 'g')
                    std::cout << "\033[33m"
                              << "▓"
                              << "\033[0m";
                
                else if ( type == 'd' )
                    std::cout << "\033[36m"
                              << "▓"
                              << "\033[0m";

                else if ( type == 'b' )
                    std::cout << "▒";

                else if ( type == 'c' )
                    std::cout << "\033[32m" << '?' << "\033[0m";
                
                else if ( type == 'B' )
                    std::cout << "\033[31m"
                              << "Ø"
                              << "\033[0m";
                
                else if ( type == 'F' )
                    std::cout << "\033[42;1m" 
                              << '%' 
                              << "\033[0m";
                
                else if ( type == 'T' )
                    std::cout << "\033[44;1m" 
                              << '@' 
                              << "\033[0m";

                else if ( type == 'P' )
                    std::cout << "\033[34;1m"
                              << "╬"
                              << "\033[0m";
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
}

void CGame::Play()
{
    if (m_Player.GetY() == 0)
        m_Player.OxReset();
    
    ClearScreen();
    m_Player.PrintStats();
    m_Player.PrintInvCapacity();
    PrintMap();
    PrintDirections();
    char ch = '0';
    while (std::cin >> ch)
    {
        bool unknownCmd = false;

        int moveCost = 0;
        if (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd')
        {
            moveCost = m_Player.MoveCost(m_Map , ch);
        }
        else if (ch == 'e')
        {
            m_Player.Inventory();
        }
        else if ( ch == 'l' )
        {
            PrintLegend();
        }
        else if (ch == 'r')
        {
            if (m_Player.GetY() == 0)
                m_Player.Shop();
        }
        else if (ch == 'i')
        {
        }
        else if (ch == 'q')
        {
            break;
        }
        else
        {
            unknownCmd = true;
        }

        if ( m_Player.Win() )
        {
            m_Win = true;
            return;
        }

        for (int i = 0; i < moveCost - 1; i++)
        {
            m_Entities.MoveEntities(m_Player,m_Map);
            m_Entities.ActionEntities(m_Player,m_Map);
            m_Player.OxDown(5);
            m_Map.Regenerate();
            ClearScreen();
            m_Player.PrintStats();
            m_Player.PrintInvCapacity();
            PrintMap();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        m_Map.Regenerate();
        m_Entities.MoveEntities(m_Player,m_Map);
        m_Player.Move(m_Map, ch);
        m_Entities.ActionEntities(m_Player,m_Map);
        if (m_Player.GetY() == 0)
            m_Player.OxReset();
        else if (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd')
            m_Player.OxDown(5);

        ClearScreen();
        m_Player.Mine(m_Map);
        m_Player.PrintStats();
        m_Player.PrintInvCapacity();
        PrintMap();
        if (ch == 'i')
            PrintDirections();
        else if (ch == 'r' && m_Player.GetY() != 0)
            std::cout << "You can only open shop at the top layer." << std::endl;
        if (unknownCmd)
            std::cout << "Please enter a valid command! Press \'i\' to show valid commands." << std::endl;
    }
}

void CGame::Win()
{
    ClearScreen();
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    for ( int i = 0 ; i < 10 ; i++ )
        std::cout << "@                                                                                               @" << std::endl;
    std::cout << "@                                           you win                                             @" << std::endl;
    for ( int i = 0 ; i < 10 ; i++ )
        std::cout << "@                                                                                               @" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
}

bool CGame::GetWin()
{
    return m_Win;
}
