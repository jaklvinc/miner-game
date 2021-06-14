#pragma once
#include <string>
#include <vector>
#include <memory>
#include "tile.h"


class CMap
{
public:
    CMap(); //constructor

    const CTile &GetOnIndex(int x, int y) const; // return a type of Tile that is on the index
    void SetOnIndex(int x, int y, CTile new_tile);

    bool Load(std::string filename); // loads a map from a file

    bool Save(std::string filename); // saves a map to a file

    void ShowMap(std::vector<std::vector<char>> & toPrint);

    void Regenerate();

    //getters and setters
    const int &getWidth() const { return m_Width; };
    const int &getHeight() const { return m_Height; };

private:
    int m_Height;
    int m_Width;
    std::vector<std::vector<CTile>> m_Map; //2D array of our Tiles
    //std::vector<std::vector<std::shared_ptr<CEnemy>>> m_Enemies;
};