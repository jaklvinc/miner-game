#pragma once

#include <string>
#include "map.h"

class CEntity
{
    protected:
        int m_PosX;
        int m_PosY;
        int m_MaxHealth;
        int m_Health;

    public:
        CEntity(){}; // def contructor
        CEntity( int x, int y); //constructor with position
        
        int GetX(void);
        int GetY(void);
        //movement commands
        virtual void Move( const CMap & map, const char direction ) = 0 ;


};

class CEnemy: public CEntity
{
    private:
        int m_FOV; //field of view of the monster
        int m_BasicDamage; //basic damage
        std::string m_EnemyName; //name of the enemy
        bool follow; //checks if the enemy follows the player
    
    public:
        CEnemy():CEntity(){};
        CEnemy( int x, int y):CEntity(x,y){};

        //bool InViewRange( CPlayer player ); //checks if player is in range
        //bool InAttackRange( CPlayer player ); //checks if player is in attack range
        //void Attack( CPlayer player ); // attacks 
};