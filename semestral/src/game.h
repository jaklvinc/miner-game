#pragma once

#include "player.h"
#include "entityHandler.h"
#include <queue>
/**
 * Wrapper class the whole game.
 */
class CGame
{
    private:
        CMap m_Map;
        CPlayer m_Player;
        CEntityHandler m_Entities;
        bool m_Win;

        /**
         * Gets only name of a file from a path
         * 
         * @param[in] filePath path to the file we want the name of
         * 
         * @returns only the name of the file
         */
        std::string getFileName(const std::string &filePath);
        
        /**
         * Lets the player choose what file to load game from
         * 
         * @param[out] file relative path to file to load the game from
         * 
         * @returns if there are any saves in the save folder 
         */
        bool LoadGame(std::string &file);

        /** 
         * Shows legend
         */
        void PrintLegend() const;
        
        /**
         * Shows all controls to the player if needed
         */
        void PrintDirections() const;
        
        /**
         * Clears the screen by outputting 100 newlines
         */
        void ClearScreen() const;
        
        /**
         * Outputs current state of the map to the standard output
         */
        void PrintMap() const;
        

    public:
        /**
         * Prints a welcome message to standard output
         */
        void Welcome() const;

        /**
         * Loads everything from a New Game or a file specified by user on standard input
         */
        bool StartGame();

        /**
         * Saves everything to a file specified by user
         */
        void SaveAndQuit();

        /**
         * Main function for playing the game
         */
        void Play();

        /**
         *  Shows win prompt.
         */
        void Win();

        /**
         *  Checks if win condition was met
         * 
         * @returns if win was achieved
         */
        bool GetWin();

};