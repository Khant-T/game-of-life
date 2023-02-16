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

	void Die()  { isAlive = false; }
	void Born() { isAlive = true; }
	void Flip() { isAlive = !isAlive; }

	bool isAlive	= 0;
	int neighbours	= 0;
};
/* ****************************** */

/* ****************************** */
struct Grid
{
	int rows 			= 0;
	int cols			= 0;

	// Position of the grid, in the window
	int posX 			= 0;
	int posY 			= 0;

	// Size of the grid
	int width 			= 0;
	int height 			= 0;

	// Size of each cell
	float cellHeight 	= 0;
	float cellWidth 	= 0;

	// Gap between the window and the grid
	int borderSize 		= 20;
};
/* ****************************** */

/* ****************************** */
class Game
{
public:
	Game();
	~Game();

	void InitCells(int rows, int cols);
	void Run();
	void HandleEvents();
	void Draw();
	void AdjustGrid();

	// Helper functions: limit the indices to rows & cols:
	int iX(int i);
	int iY(int i);

	// Run the simulation
	void RunSimuation();

private:
	bool isRunning  		= false;
	bool isRunningSim		= false;

	SDL_Window*   window	= nullptr;
	SDL_Renderer* renderer  = nullptr;

	int mouseX		= 0;
	int mouseY		= 0;
	int mouseCellX 	= 0;
	int mouseCellY	= 0;
	bool mouseDown	= false;


	Grid grid;
	vector<vector<Cell>> cells;
};
/* ****************************** */