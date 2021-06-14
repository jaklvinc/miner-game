#pragma once

#include "inventory.h"
#include <string>

class CEntity
{
    protected:
        int m_PosX;
        int m_PosY;
        int m_Lives;

    public:
        CEntity(); // def contructor
        CEntity( int x, int y): m_PosX (x) , m_PosY (y) {}; //constructor with position

};

class CPlayer: public CEntity
{
    CPlayer():CEntity(){};
    CPlayer( int x, int y):CEntity(x,y){}; 

    bool UpgradeArmor( char EquipementType ); //upgrades our equipement

    protected:
        CInventory m_Inv;
        int m_LvlDrill;
        int m_LvlTank;
        int m_LvlArmor;
};

class CEnemy: public CEntity
{
    private:
        int m_FOV; //field of view of the monster
        int m_BasicDamage; //basic damage
        std::string m_EnemyName; //name of the enemy
        bool follow; //checks if the enemy follows the player
    
    CEnemy():CEntity(){};
    CEnemy( int x, int y):CEntity(x,y){};

    bool InViewRange( CPlayer player ); //checks if player is in range
    bool InAttackRange( CPlayer player ); //checks if player is in attack range
    void Attack( CPlayer player ); // attacks 
};