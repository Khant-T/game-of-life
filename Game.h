#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

using namespace std;

/* ****************************** */
struct Cell
{
	Cell(bool alive = false)
		: isAlive{alive}, neighbours{0}
	{  }

	void Die() { isAlive = false; }
	void Born() { isAlive = true; }
	void Flip() { isAlive = !isAlive; }

	bool isAlive;
	int neighbours;
};
/* ****************************** */

/* ****************************** */

/* ****************************** */

/* ****************************** */
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
/* ****************************** */