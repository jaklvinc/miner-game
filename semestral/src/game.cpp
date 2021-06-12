
#include "game.h"
#include <iostream>
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
    if (std::filesystem::is_empty("../examples/saves"))
    {
        return false;
    }
    while (true)
    {
        std::cout << "Your saves:" << std::endl;
        for (const auto &files : std::filesystem::directory_iterator("../examples/saves"))
        {
            std::cout << getFileName(files.path()) << std::endl;
        }
        std::cout << "──────────────────────────" << std::endl;
        file = "../examples/saves/";
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

void CGame::PrintDirections()
{
    std::cout << "Press \'w,a,s,d\' to move." << std::endl;
    std::cout << "Press \'e\' to open inventory." << std::endl;
    std::cout << "Press \'q\' to save and quit." << std::endl;
    std::cout << "Press \'r\' to open shop." << std::endl;
}

void CGame::Welcome()
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
    std::cout << "@                                                                                @" << std::endl;
    std::cout << "@    Also watch out for the animals, they don't like to see unfamiliar faces...  @" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;   
}

bool CGame::StartGame()
{
    std::cout << "Choose from the following:" << std::endl;
    std::cout << "1 = START NEW GAME" << std::endl;
    std::cout << "2 = LOAD FROM A SAVE" << std::endl;
    std::cout << "──────────────────────────" << std::endl;
    char input;
    while (std::cin >> input)
    {
        if (input == '1')
        {
            if (!m_Map.Load("../examples/new_game/ng"))
            {
                std::cout << "Map couldn't be loaded, your save may be corrupted, or it doesn't exist." << std::endl;
                return false;
            }
            if (!m_Player.Load("../examples/new_game/ng"))
            {
                std::cout << "Player data couldn't be loaded, your save may be corrupted, or it doesn't exist." << std::endl;
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
            if (!m_Player.Load(filename))
            {
                std::cout << "Player data couldn't be loaded, your save may be corrupted." << std::endl;
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
    for (const auto &files : std::filesystem::directory_iterator("../examples/saves"))
    {
        std::cout << getFileName(files.path()) << std::endl;
    }
    std::cout << "──────────────────────────" << std::endl;
    std::cout << "Choose an existing save to override or type in a different name and a new save will be created" << std::endl;
    std::string outFilename;
    std::cin >> outFilename;

    outFilename = "../examples/saves/" + outFilename;
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

    return;
}

void CGame::Play()
{
    bool breakOut = false;
    if (m_Player.GetY() == 0)
        m_Player.OxReset();
    m_Map.ShowMap(m_Player.GetX(), m_Player.GetY(), m_Player.GetLight());
    m_Player.PrintStats();
    m_Player.PrintInvCapacity();
    PrintDirections();
    while (!breakOut)
    {
        bool unknownCmd = false;
        char ch = '0';
        std::cin >> ch;

        int moveCost = 0;
        if (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd')
        {
            moveCost = m_Player.MoveCost(m_Map , ch);
        }
        else if (ch == 'e')
        {
            m_Player.Inventory();
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

        for (int i = 0; i < moveCost - 1; i++)
        {
            m_Player.OxDown(5);
            m_Map.Regenerate();
            m_Map.ShowMap(m_Player.GetX(), m_Player.GetY(), m_Player.GetLight());
            m_Player.PrintStats();
            m_Player.PrintInvCapacity();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        m_Map.Regenerate();
        m_Player.Move(m_Map, ch);
        if (m_Player.GetY() == 0)
            m_Player.OxReset();
        else if (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd')
            m_Player.OxDown(5);

        m_Map.ShowMap(m_Player.GetX(), m_Player.GetY(), m_Player.GetLight());
        m_Player.PrintStats();
        m_Player.Mine(m_Map);
        m_Player.PrintInvCapacity();
        if (ch == 'i')
            PrintDirections();
        else if (ch == 'r' && m_Player.GetY() != 0)
            std::cout << "You can only open shop at the top layer." << std::endl;
        if (unknownCmd)
            std::cout << "Please enter a valid command! Press \'i\' to show valid commands." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}