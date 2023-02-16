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
struct Grid
{
	int rows = 0;
	int cols = 0;

	// Position of the grid, in the window
	int posX = 0;
	int posY = 0;

	// Size of the grid, in the window
	int width = 0;
	int height = 0;

	// Size of each cell, in the window
	float cellHeight = 0;
	float cellWidth = 0;

	// Gap between the window and the grid
	int borderSize = 20;
};
/* ****************************** */

/* ****************************** */
class Game
{
public:
	Game();
	~Game();

	void InitCells(int rows, int cols);
	void Draw();
	void Run();
	void AdjustGrid();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	bool isRunning;

	Grid grid;

	vector<vector<Cell>> cells;
};
/* ****************************** */