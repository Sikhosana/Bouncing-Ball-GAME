#include "pch.h"
#include "ball.h"
#include <iostream>
#include <cstdlib>
namespace ballspace
{
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Window* gwindow = nullptr;
	SDL_Renderer* grenderer = nullptr;
	Mix_Music* gameMusic = nullptr;
	Ltexture ballTexture;
	Ltexture boardTexture;
	Ltexture startTexture;
	Ltexture background;
	Ltexture gameOver;
	TTF_Font* gfont = nullptr;

	bool init()
	{
		bool success = true;
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		{
			std::cout << "Failed to initialise SDL. SDL_Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			gwindow = SDL_CreateWindow("Bouncy Ball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			if (gwindow == nullptr)
			{
				std::cout << "Failed to create window. SDL_Error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
				{
					std::cout << "Linear Texture Filtering not enabled" << std::endl;
				}
				grenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (grenderer == nullptr)
				{
					std::cout << "Failed to create renderer. SDL_Error: " << SDL_GetError() << std::endl;
					success = false;
				}
				else
				{
					SDL_SetRenderDrawColor(grenderer, 0xff, 0xff, 0xff, 0xff);
					//initialise image loading
					int imgFlags = IMG_INIT_PNG;
					if (!(IMG_Init(imgFlags) & imgFlags))
					{
						std::cout << "SDL_Image could not initialise. SDL_Error: " << IMG_GetError() << std::endl;
						success = false;
					}

					if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
					{
						std::cout << "Failed to initialise SDL_mixer! SDL_Mixer Error: " << Mix_GetError() << std::endl;
						success = false;
					}

					if (TTF_Init() == -1)
					{
						std::cout << "Failed to initialise true type fonts. TTF_Error: " << TTF_GetError() << std::endl;
						success = false;
					}
				}
			}
		}
		return success;
	}
	bool loadMedia()
	{
		bool loaded = true;
		if (!ballTexture.loadFromFile("ball.png"))
		{
			std::cout << "Failed to load ball texture. SDL_Error: " << SDL_GetError() << std::endl;
			loaded = false;
		}
		if (!boardTexture.loadFromFile("board.png"))
		{
			std::cout << "Failed to load ball texture. SDL_Error: " << SDL_GetError() << std::endl;
			loaded = false;
		}
		if (!background.loadFromFile("galaxy.png"))
		{
			std::cout << "Failed to load galaxy texture. SDL_Error: " << SDL_GetError() << std::endl;
			loaded = false;
		}
		if (!startTexture.loadFromFile("start.png"))
		{
			std::cout << "Failed to load start texture. SDL_Error: " << SDL_GetError() << std::endl;
			loaded = false;
		}
		if (!gameOver.loadFromFile("gameOver.png"))
		{
			std::cout << "Failed to load gameOver texture. SDL_Error: " << SDL_GetError() << std::endl;
			loaded = false;
		}
		gfont = TTF_OpenFont("Roboto-Bold.ttf", 20);
		if (gfont == nullptr)
		{
			std::cout << "Failed to open font file. SDL_TTF Error: " << TTF_GetError() << std::endl;
			loaded = false;
		}
		return loaded;
	}
	int loadStart()
	{
		gameMusic = Mix_LoadMUS("Blob-Monsters-Return.mp3");
		Mix_PlayMusic(gameMusic, -1);
		SDL_SetRenderDrawColor(grenderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(grenderer);
		startTexture.render(0, 0);
		SDL_RenderPresent(grenderer);
		SDL_Event c;
		bool chose = false;
		while (!chose)
		{
			while (SDL_PollEvent(&c) != 0)
			{
				if (c.type == SDL_QUIT)
				{
					chose = true;
					Mix_HaltMusic();
					Mix_FreeMusic(gameMusic);
					gameMusic = nullptr;
					return 1;
				}
				if (c.type == SDL_KEYDOWN)
				{
					switch (c.key.keysym.sym)
					{
					case SDLK_RETURN:
						chose = true;
						break;
					}
				}
			}
		}
		return 2;
	}
	void close()
	{
		ballTexture.free();

		//DESTROY THE WINDOW AND RENDERER
		SDL_DestroyRenderer(grenderer);
		SDL_DestroyWindow(gwindow);
		grenderer = nullptr;
		gwindow = nullptr;

		//quit the SDL subsystems
		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	Ltexture::Ltexture()
	{
		mtexture = nullptr;
		mwidth = 0;
		mheight = 0;
	}
	bool Ltexture::loadFromFile(std::string path)
	{
		free();
		SDL_Texture* finalTexture = nullptr;

		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == nullptr)
		{
			std::cout << "Failed to load surface. SDL_Error: " << IMG_GetError() << std::endl;
		}
		else
		{
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xff, 0xff, 0xff));

			finalTexture = SDL_CreateTextureFromSurface(grenderer, loadedSurface);
			if (finalTexture == nullptr)
			{
				std::cout << "Failed to create texture from surface. SDL_Error: " << SDL_GetError() << std::endl;
			}
			else
			{
				mwidth = loadedSurface->w;
				mheight = loadedSurface->h;
			}
			//free the texture
			SDL_FreeSurface(loadedSurface);
			loadedSurface = nullptr;
		}
		mtexture = finalTexture;
		return mtexture != nullptr;
	}
	bool Ltexture::loadFromRenderedText(std::string textureText, SDL_Color color)
	{
		free();
		SDL_Texture* finalTexture = nullptr;
		SDL_Surface* loadedSurface = TTF_RenderText_Solid(gfont, textureText.c_str(), color);
		if (loadedSurface == nullptr)
		{
			std::cout << "Failed to load text surface. TTF_Error: " << TTF_GetError() << std::endl;
		}
		else
		{
			finalTexture = SDL_CreateTextureFromSurface(grenderer, loadedSurface);
			if (finalTexture == nullptr)
			{
				std::cout << "Failed to create texture from surface. SDL_Error: " << SDL_GetError() << std::endl;
			}
			else
			{
				mwidth = loadedSurface->w;
				mheight = loadedSurface->h;
			}
			SDL_FreeSurface(loadedSurface);
			loadedSurface = nullptr;
		}
		mtexture = finalTexture;
		return mtexture != nullptr;
	}
	void Ltexture::render(int x, int y, SDL_Rect* clipRect, double angle, SDL_Point* center, SDL_RendererFlip flipType)
	{
		SDL_Rect renderQuad = { x,y,mwidth,mheight };
		if (clipRect != nullptr)
		{
			renderQuad.w = clipRect->w;
			renderQuad.h = clipRect->h;
		}
		SDL_RenderCopyEx(grenderer, mtexture, clipRect, &renderQuad, angle, center, flipType);
	}
	int Ltexture::getWidth()
	{
		return mwidth;
	}
	int Ltexture::getHeight()
	{
		return mheight;
	}
	void Ltexture::free()
	{
		if (mtexture != nullptr)
		{
			SDL_DestroyTexture(mtexture);
			mtexture = nullptr;
			mwidth = 0;
			mheight = 0;
		}
	}
	Ltexture::~Ltexture()
	{
		free();
	}

	Ball::Ball()
	{

		xPos = 40;
		yPos = 0;
		xVel = 2;
		yVel = 2;
		ballRect = { 0, 0, dot_width, dot_height };
	}
	void Ball::move(SDL_Rect b)
	{
		xPos += xVel;
		if (xPos < 0 || (xPos + dot_width) >= SCREEN_WIDTH)
		{
			xVel = xVel * -1;
		}
		
		if (yPos < 0 || (checkCollision(ballRect, b) == 1))
		{
			yVel = yVel * -1;
		}
		yPos += yVel;

		ballRect.x = xPos;
		ballRect.y = yPos;
	}

	void Ball::render()
	{
		ballTexture.render(xPos, yPos);
	}
	SDL_Rect Ball::getRect() { return ballRect; }

	Board::Board()
	{
		collider = { 0, 400, bwidth, bheight };
	}
	void Board::handleEvent(SDL_Event& e)
	{
		if (e.type == SDL_KEYDOWN || e.key.repeat == 0)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
			{
				collider.x -= 10;
				if (collider.x < 0)
					collider.x = 0;
			}
			break;
			case SDLK_RIGHT:
			{
				collider.x += 10;
				if (collider.x + bwidth >= SCREEN_WIDTH)
					collider.x = 590;
				break;
			}
			}
		}
	}
	void Board::render()
	{
		boardTexture.render(collider.x, collider.y);
	}
	SDL_Rect Board::getRect() { return collider; }

	int checkCollision(SDL_Rect ballRect, SDL_Rect boardRect)
	{
		if (((ballRect.x + 10) >= boardRect.x) && ((ballRect.x + 10) <= (boardRect.x + boardRect.w)))
		{
			if ((ballRect.y + 20) == boardRect.y)
			{
				return 1;
			}
			return 2;
		}
		else if (ballRect.y == (SCREEN_HEIGHT-20))
		{
			return 3;
		}
		return 0;
	}

}