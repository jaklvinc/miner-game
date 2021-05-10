#pragma once

#include <string>
#include <vector>
#include "tile.h"

class CMap
{
    public:
        CMap();//constructor

        CTile & GetOnIndex( int x , int y); // return a type of Tile that is on the index

        bool Load( std::string fileName ); // loads a map from a file

    private:
        int m_Height;
        int m_Width;
        std::vector<std::vector<CTile>> m_Map; //2D array of our Tiles
};