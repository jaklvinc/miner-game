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

std::string getFileName(const std::string & filePath) {
    size_t i = filePath.rfind('/', filePath.length());
    if (i != std::string::npos) {
        return(filePath.substr(i+1, filePath.length() - i));
    }

    return("");
}


std::string Welcome( void )
{
    std::cout<< "       Welcome to miner!         " << std::endl;
    
    std::cout<< "–––––––––––––––––––––––––––––––––" << std::endl;
    std::cout<< "Choose your save:" <<  std::endl;
    for ( const auto & file : std::filesystem::directory_iterator("../assets/saves"))
    {
        std::cout << getFileName(file.path()) << std::endl;
    }
    std::cout<< "–––––––––––––––––––––––––––––––––" << std::endl << std::flush ;
    
    while ( true )
    {
        std::string fileLocation = "../assets/saves/";
        std::string fileName;
        std::cin >> fileName;
        fileLocation.append(fileName);
        if ( std::filesystem::exists(fileLocation) )
            return fileLocation;
        else
        {
            std::cout << "Choose an existing save file:" << std::endl ;
            continue;
        }
    }
}

void Play( CMap & map, CPlayer & player )
{
    bool breakOut = false;
    map.ShowMap( player.GetX() , player.GetY() , player.GetLight() );
    player.PrintStats();
    while(!breakOut)
    {
        char ch;
        std::cin >> ch;
        
        int moveCost = 0;
        if ( ch == 'w' || ch == 's' || ch == 'a' || ch == 'd')
        {
            moveCost=player.MoveCost(map,ch);
        }
        else 
        {
            break;
        }

        for ( int i = 0 ; i < moveCost-1 ; i++ )
        {
            player.OxDown(5);
            map.ShowMap( player.GetX() , player.GetY() , player.GetLight() );
            player.PrintStats();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        player.Move(map,ch);
        if ( player.GetY() == 0 )
            player.OxReset();
        else
            player.OxDown(5);
        
        player.Mine(map);
        map.ShowMap( player.GetX() , player.GetY() , player.GetLight() );
        player.PrintStats();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}



int main(int argc, char* args[])
{
    CMap map{};
    CPlayer player;
    
    std::string inFilename;
    inFilename = Welcome();
    
    if (! map.Load( inFilename ))
    {
        std::cout << "Your save was corrupted or cannot be opened." << std::endl;
        return 0;
    }
    
    if (! player.Load( inFilename ))
    {
        std::cout << "Your save was corrupted or cannot be opened." << std::endl;
        return 0;
    }

    Play( map, player);
    

    if (! player.Save( inFilename ))
    {
        std::cout << "Your game couldn't be saved." << std::endl;
        return 0;
    }

    if (! map.Save( inFilename ))
    {
        std::cout << "Your game couldn't be saved." << std::endl;
        return 0;
    }
    

    return 0;
}