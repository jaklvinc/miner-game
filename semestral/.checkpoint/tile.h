#pragma once 

class CTile
{
    protected:
        char m_TileType;
        bool m_IsSolid;

    public:
        CTile();
        CTile( char type , bool solid) : m_TileType(type), m_IsSolid(solid) {};

        virtual bool MoveInto( int DrillLvl ) = 0;
        virtual bool MoveInto( void ) = 0; 

};



class CInteractable : public CTile
{
    protected:
        int m_MineTime; // sets the energy used to mine certain Tile

    public:
        //TODO: set minetime depending on Tile type
        CInteractable():CTile(){};
        CInteractable(char type, bool solid ):CTile(type,solid){};

        virtual bool MoveInto( int DrillLvl ) override ;//when a player moves into this 
};



class CNonInteractable : public CTile
{
    public:
    virtual bool MoveInto( void ) override ;

    CNonInteractable():CTile(){};
    CNonInteractable(char type, bool solid ):CTile(type,solid){};
};