// Bouncing_Ball.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "ball.h"
#include <sstream>
using namespace ballspace;

int main(int argc, char* args[])
{
	if (!init())
	{
		std::cout << "Initialisation failed. SDL_Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		if (!loadMedia())
		{
			std::cout << "Failed to load media. SDL_Error: " << SDL_GetError() << std::endl;
		}
		else
		{
			int choice = loadStart();
			if (choice == 1)
			{
				std::cout << "Bye" << std::endl;
			}
			else
			{
				Ltexture timeAndScore;
				int startTime = SDL_GetTicks();
				std::stringstream score;
				int startSpeed = 2;
				Ball gameBall;
				Board gameBoard;
				bool quit = false;
				SDL_Event e;
				while (!quit)
				{
					score.str("");
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							quit = true;
							Mix_HaltMusic();
						}
						gameBoard.handleEvent(e);
					}
					SDL_Rect checker = gameBall.getRect();
					if (checker.y > 470)
					{
						SDL_SetRenderDrawColor(grenderer, 0, 0, 0, 0);
						SDL_RenderClear(grenderer);
						gameOver.render((SCREEN_WIDTH - gameOver.getWidth()) / 2, (SCREEN_HEIGHT - gameOver.getWidth()) / 2);
						SDL_RenderPresent(grenderer);
					}
					else
					{
						SDL_Color color = { 0,255,0,255 };

						int td = (SDL_GetTicks() - startTime)/1000;
						score << "Time elapsed(s): " << td << "   Score: " << (td*2)/3;
						timeAndScore.loadFromRenderedText(score.str().c_str(), color);
						gameBall.move(gameBoard.getRect());
						SDL_SetRenderDrawColor(grenderer, 0, 0, 0, 0);
						SDL_RenderClear(grenderer);
						background.render(0, 0);
						gameBoard.render();
						gameBall.render();
						timeAndScore.render(0, 430);
						SDL_RenderPresent(grenderer);

					}
				}
				Mix_FreeMusic(gameMusic);
				timeAndScore.free();
				gameMusic = nullptr;
			}
		}

		close();
	}
	return 0;
}