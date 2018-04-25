#include <windows.h>

#include "player.h"
#include "GameComponents\Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game game(hInstance, L"Game Castlevania", GAME_SCREEN_RESOLUTION_640_480_24, 0, 60);

	game.Init();
	game.Run();

	return 0;
}


