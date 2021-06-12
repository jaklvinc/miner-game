#pragma once
#include <string>
#include <vector>
#include <memory>
#include "tile.h"


class CMap
{
public:
    CMap(); //constructor

    const std::shared_ptr<CTile> &GetOnIndex(int x, int y) const; // return a type of Tile that is on the index
    void SetOnIndex(int x, int y, std::shared_ptr<CTile> new_tile);

    bool Load(std::string filename); // loads a map from a file

    bool Save(std::string filename); // saves a map to a file

    void ShowMap(int playerX, int playerY, int lightLvl);

    void Regenerate();

    //getters and setters
    const int &getWidth() const { return m_Width; };
    const int &getHeight() const { return m_Height; };

private:
    int m_Height;
    int m_Width;
    std::vector<std::vector<std::shared_ptr<CTile>>> m_Map; //2D array of our Tiles
    //std::vector<std::vector<std::shared_ptr<CEnemy>>> m_Enemies;
};