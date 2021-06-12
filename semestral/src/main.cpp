#include <iostream>
#include <map>
#include <stdlib.h>
#include <filesystem>
#include <chrono>
#include <thread>

//#include <ncurses.h>
#include "map.h"
#include "entity.h"
#include "player.h"

std::string getFileName(const std::string &filePath)
{
    size_t i = filePath.rfind('/', filePath.length());
    if (i != std::string::npos)
    {
        return (filePath.substr(i + 1, filePath.length() - i));
    }

    return ("");
}

bool LoadGame(std::string &file)
{
    if (std::filesystem::is_empty("../assets/saves"))
    {
        return false;
    }
    while (true)
    {
        std::cout << "Your saves:" << std::endl;
        for (const auto &files : std::filesystem::directory_iterator("../assets/saves"))
        {
            std::cout << getFileName(files.path()) << std::endl;
        }
        std::cout << "──────────────────────────" << std::endl;
        file = "../assets/saves/";
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

bool StartGame(CMap &map, CPlayer &player)
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
            if (!map.Load("../assets/new_game/ng"))
            {
                std::cout << "Map couldn't be loaded, your save may be corrupted, or it doesn't exist." << std::endl;
                return false;
            }
            if (!player.Load("../assets/new_game/ng"))
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

            if (!map.Load(filename))
            {
                std::cout << "Map couldn't be loaded, your save may be corrupted." << std::endl;
                return false;
            }
            if (!player.Load(filename))
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

void SaveAndQuit(CMap &map, CPlayer &player)
{
    std::cout << std::string(100, '\n');
    std::cout << "Your saves:" << std::endl;
    for (const auto &files : std::filesystem::directory_iterator("../assets/saves"))
    {
        std::cout << getFileName(files.path()) << std::endl;
    }
    std::cout << "──────────────────────────" << std::endl;
    std::cout << "Choose an existing save to override or type in a different name and a new save will be created" << std::endl;
    std::string outFilename;
    std::cin >> outFilename;

    outFilename = "../assets/saves/" + outFilename;
    if (!player.Save(outFilename))
    {
        std::cout << "Your player stats couldn't be saved." << std::endl;
        return;
    }

    if (!map.Save(outFilename))
    {
        std::cout << "Your map couldn't be saved." << std::endl;
        return;
    }

    return;
}

void Welcome(void)
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

void PrintDirections()
{
    std::cout << "Press \'w,a,s,d\' to move." << std::endl;
    std::cout << "Press \'e\' to open inventory." << std::endl;
    std::cout << "Press \'q\' to save and quit." << std::endl;
    std::cout << "Press \'r\' to open shop." << std::endl;
}

void Play(CMap &map, CPlayer &player)
{
    bool breakOut = false;
    if (player.GetY() == 0)
        player.OxReset();
    map.ShowMap(player.GetX(), player.GetY(), player.GetLight());
    player.PrintStats();
    player.PrintInvCapacity();
    PrintDirections();
    while (!breakOut)
    {
        bool unknownCmd = false;
        char ch = '0';
        std::cin >> ch;

        int moveCost = 0;
        if (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd')
        {
            moveCost = player.MoveCost(map, ch);
        }
        else if (ch == 'e')
        {
            player.Inventory();
        }
        else if (ch == 'r')
        {
            if (player.GetY() == 0)
                player.Shop();
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
            player.OxDown(5);
            map.ShowMap(player.GetX(), player.GetY(), player.GetLight());
            player.PrintStats();
            player.PrintInvCapacity();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        player.Move(map, ch);
        if (player.GetY() == 0)
            player.OxReset();
        else if (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd')
            player.OxDown(5);

        map.ShowMap(player.GetX(), player.GetY(), player.GetLight());
        player.PrintStats();
        player.Mine(map);
        player.PrintInvCapacity();
        if (ch == 'i')
            PrintDirections();
        else if (ch == 'r' && player.GetY() != 0)
            std::cout << "You can only open shop at the top layer." << std::endl;
        if (unknownCmd)
            std::cout << "Please enter a valid command! Press \'i\' to show valid commands." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main(int argc, char *args[])
{
    CMap map{};
    CPlayer player;

    std::string inFilename;
    Welcome();
    if (!StartGame(map, player))
        return 0;

    Play(map, player);

    SaveAndQuit(map, player);

    return 0;
}