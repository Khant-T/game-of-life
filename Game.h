#pragma once

#include <iostream>
#include <vector>

using namespace std;

#include <SDL2/SDL.h>

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool isRunning;
};