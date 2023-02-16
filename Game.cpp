#include "Game.h"

Game::Game()
	: window{nullptr}, renderer{nullptr}, isRunning{false}
{
	// create SDL_Window
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

	// create SDL_Renderer
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

	// run program
	isRunning = true;
}

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Game::Run()
{
	while (isRunning)
	{
		SDL_Event ev;
		SDL_WaitEvent(&ev);

		if (ev.type == SDL_WINDOWEVENT)
		{
			if (ev.window.event == SDL_WINDOWEVENT_CLOSE)
				isRunning = false;
		}

		if (ev.type == SDL_KEYDOWN)
		{
			if (ev.key.keysym.sym == SDLK_ESCAPE)
				isRunning = false;
		}

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);

		SDL_RenderPresent(renderer);
	}
}