#include "Game.h"


#define TICK_INTERVAL 20
static Uint32 nextTime;


/* ****************************** */
Uint32 TimeLeft()
{
	Uint32 curTime;
	curTime = SDL_GetTicks();
	if (nextTime <= curTime)
		return 0;
	else
		return nextTime - curTime;
}
/* ****************************** */


/* ****************************** */
Game::Game()
	: window{nullptr}, renderer{nullptr}, isRunning{false}
{
	// Create SDL_Window
	window = SDL_CreateWindow(
		"Game of Life",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		800,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	if (window == nullptr)
	{
		cout << "Failed to create SDL_Window" << endl;
		return;
	}

	// Create SDL_Renderer
	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_PRESENTVSYNC
	);
	if (renderer == nullptr)
	{
		cout << "Failed to create SDL_Renderer" << endl;
		return;
	}

	// Run program
	isRunning = true;
}
/* ****************************** */


/* ****************************** */
Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}
/* ****************************** */


/* ****************************** */
void Game::InitCells(int rows, int cols)
{
	cells.clear();
	grid.rows = rows;
	grid.cols = cols;

	for (int i = 0; i < rows; ++i)
	{
		vector<Cell> columns;
		for (int j = 0; j < grid.cols; ++j)
		{
			columns.push_back(Cell());
		}
		cells.push_back(columns);
	}

	// Initialize some cells
	for (int i = 0; i < rows; ++i)
	{
		cells[i][i].Born();
	}
}
/* ****************************** */


/* ****************************** */
void Game::Run()
{
	InitCells(40, 40);
	AdjustGrid();

	nextTime = SDL_GetTicks() + TICK_INTERVAL;

	while (isRunning)
	{
		// Clear the screen
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);


		// Draw everything
		Draw();
		SDL_RenderPresent(renderer);

		// Handle events
		HandleEvents();

		// Run cell simulation
		if (isRunningSim && !mouseDown)
			RunSimulation();

		// Limit framerate
		SDL_Delay(TimeLeft());
		nextTime += TICK_INTERVAL;
	}

	cout << "The game loop has ended." << endl;
}
/* ****************************** */


/* ****************************** */
void Game::HandleEvents()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		// Mouse events:
		if (ev.type == SDL_MOUSEMOTION)
			SDL_GetMouseState(&mouseX, &mouseY);

		if (ev.type == SDL_MOUSEBUTTONDOWN)
			if (ev.button.button == SDL_BUTTON_LEFT)
				mouseDown = true;

		if (ev.type == SDL_MOUSEBUTTONDOWN)
			if (ev.button.button == SDL_BUTTON_LEFT)
				if (mouseDown)
					mouseDown = false;

		// Window events:
		if (ev.type == SDL_WINDOWEVENT)
		{
			if (ev.window.event == SDL_WINDOWEVENT_CLOSE)
				isRunning = false;

			if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
				AdjustGrid();

			if (ev.window.event == SDL_WINDOWEVENT_RESTORED)
				AdjustGrid();

			if (ev.window.event == SDL_WINDOW_FULLSCREEN)
				AdjustGrid();
		}

		// Key events:
		if (ev.type == SDL_KEYDOWN)
		{
			if (ev.key.keysym.sym == SDLK_ESCAPE)
				isRunning = false;

			if (ev.key.keysym.sym == SDLK_SPACE)
			{
				isRunningSim = !isRunningSim;
				if (isRunningSim)
					SDL_SetWindowTitle(window, "Game of Life (running)");
				else
					SDL_SetWindowTitle(window, "Game of Life (paused)");
			}
		}
	}
}
/* ****************************** */


/* ****************************** */
void Game::Draw()
{
	for (int x = 0; x < grid.rows; ++x)
	{
		for (int y = 0; y < grid.cols; ++y)
		{
			// Define size and position for each cell
			SDL_FRect cellRect = {
				1.0f + (float) grid.posX + ((float) x * grid.cellWidth),
				1.0f + (float) grid.posY + ((float) y * grid.cellHeight),
				grid.cellWidth,
				grid.cellHeight
			};

			 // Color a cell based on its alive status
			if (cells[x][y].isAlive)
				SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
			else
				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
			SDL_RenderFillRectF(renderer, &cellRect);

			// Mouse hover effect
			if (mouseX > cellRect.x && mouseX < cellRect.x + cellRect.w)
			{
				if (mouseY > cellRect.y && mouseY < cellRect.y + cellRect.h)
				{
					mouseCellX = x;
					mouseCellY = y;
					SDL_SetRenderDrawColor(renderer, 150, 150, 150, 15);
					SDL_RenderFillRectF(renderer, &cellRect);
				}
			}
		}
	}

	// Draw grid lines
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	for (int i = 0; i <= grid.rows; ++i)
	{
		SDL_FRect vLine = {
			(float) grid.posX + (i * grid.cellWidth) + 1,
			(float) grid.posY + 1,
			2,
			(float) ((i == grid.rows) ? grid.height + 2 : grid.height)
		};
		SDL_RenderFillRectF(renderer, &vLine);
	}
	for (int i = 0; i <= grid.cols; ++i)
	{
		SDL_FRect vLine = {
			(float) grid.posX + 1,
			(float) grid.posY + (i * grid.cellHeight) + 1,
			(float) ((i == grid.cols) ? grid.width + 2 : grid.width),
			2
		};
		SDL_RenderFillRectF(renderer, &hLine);
	}
}
/* ****************************** */


/* ****************************** */
void Game::AdjustGrid()
// Keep the width and the height equal
// Center the grid in the window
{
	int ww = 0;
	int wh = 0;
	SDL_GetWindowSize(window, &ww, &wh);

	int maxLength = 0;
	if (ww > wh)
		maxLength = wh;
	else
		maxLength = ww;

	grid.posX = grid.borderSize;
	grid.posY = grid.borderSize;

	int botRightX = maxLength - grid.borderSize;
	int botRightY = maxLength - grid.borderSize;

	grid.width = botRightX - grid.posX;
	grid.height = botRightY - grid.posY;

	grid.cellWidth = (float) grid.width/(float) grid.rows;
	grid.cellHeight = (float) grid.height/(float) grid.cols;

	if (ww > wh)
		grid.posX = ((float) ww/2.0f) - ((float) grid.width/2.0f);
	else
		grid.posY = ((float) wh/2.0f) - ((float) grid.height/2.0f);
}
/* ****************************** */


/* ****************************** */
int Game::iX(int i)
{
	if (i < 0)					return 0;
	else if (i > grid.rows - 1)	return grid.rows - 1;
	else						return i;
}

int Game::iY(int i)
{
	if (i < 0)					return 0;
	else if (i > grid.cols - 1)	return grid.cols - 1;
	else						return i;
}
/* ****************************** */


/* ****************************** */
void Game::RunSimulation()
{
	for (int x = 0; x < cells.size(); ++x)
		for (int y = 0; y < cells[x].size(); ++y)
		{
			// Reset the neighbour count
			cells[x][y].neighbours = 0;
			
			// Top left
			if (cells[iX(x - 1)][iY(y - 1)].isAlive)
				cells[x][y].neighbours++;

			// Top
			if (cells[iX(x)][iY(y - 1)].isAlive)
				cells[x][y].neighbours++;

			// Top right
			if (cells[iX(x + 1)][iY(y - 1)].isAlive)
				cells[x][y].neighbours++;

			// Left
			if (cells[iX(x - 1)][iY(y)].isAlive)
				cells[x][y].neighbours++;

			// Right
			if (cells[iX(x + 1)][iY(y)].isAlive)
				cells[x][y].neighbours++;

			// Bottom Left
			if (cells[iX(x - 1)][iY(y + 1)].isAlive)
				cells[x][y].neighbours++;

			// Bottom
			if (cells[iX(x)][iY(y + 1)].isAlive)
				cells[x][y].neighbours++;

			// Bottom right
			if (cells[iX(x + 1)][iY(y + 1)].isAlive)
				cells[x][y].neighbours++;
		}

	while (simTick < simDelay)
	{
		++simTick;
		SDL_Delay(1);
	}

	for (int x = 0; x < cells.size(); x++)
		for (int y = 0; y < cells[x].size(); y++) 
		{
			if (cells[x][y].neighbours == 3)	cells[x][y].Born();
			if (cells[x][y].neighbours < 2)		cells[x][y].Die();
			if (cells[x][y].neighbours > 3)		cells[x][y].Die();
		}
	simTick = 0;
}
/* ****************************** */

