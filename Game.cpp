#include "Game.h"


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


Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}


void Game::Run()
{
	InitCells(20, 20);
	AdjustGrid();

	while (isRunning)
	{
		SDL_Event ev;
		SDL_WaitEvent(&ev);

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

		if (ev.type == SDL_KEYDOWN)
		{
			if (ev.key.keysym.sym == SDLK_ESCAPE)
				isRunning = false;
		}

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);

		Draw();

		SDL_RenderPresent(renderer);
	}
}


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
		}
	}
}


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