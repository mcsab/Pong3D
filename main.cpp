#include "game.hpp"

int main()
{
    Game* game = Game::createGame();
    game->play();
    delete game;
}