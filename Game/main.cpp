#include "Game.hpp"


int main()
{
	Game game;

	game.Setup();
	game.GameLoop();
	game.Close();
}