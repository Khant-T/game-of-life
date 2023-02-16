#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

int main(int argc, char*[])
{
	cout << "Program started." << endl;

	SDL_Window* myWindow;
	myWindow = SDL_CreateWindow(
		"Hello, world!",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		800,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	SDL_Renderer* myRenderer;
	myRenderer = SDL_CreateRenderer(
		myWindow,
		-1,
		SDL_RENDERER_PRESENTVSYNC
	);


	bool running = true;
	while (running)
	{
		SDL_Event ev;
		SDL_WaitEvent(&ev);

		if (ev.type == SDL_WINDOWEVENT)
		{
			if (ev.window.event == SDL_WINDOWEVENT_CLOSE)
				running = false;
		}

		if (ev.type == SDL_KEYDOWN)
		{
			if (ev.key.keysym.sym == SDLK_ESCAPE)
				running = false;
		}

		SDL_SetRenderDrawColor(myRenderer, 100, 100, 100, 255);
		SDL_RenderClear(myRenderer);

		SDL_RenderPresent(myRenderer);
	}

	cout << "Program closed." << endl;
	SDL_DestroyRenderer(myRenderer);
	SDL_DestroyWindow(myWindow);
	return 0;
}
