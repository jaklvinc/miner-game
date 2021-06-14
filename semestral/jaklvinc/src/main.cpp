#include "game.h"

int main(int argc, char *args[])
{
    CGame game;

    game.Welcome();
    if (!game.StartGame())
        return 0;

    game.Play();

    if ( game.GetWin() )
        game.Win();
    else
        game.SaveAndQuit();

    return 0;
}