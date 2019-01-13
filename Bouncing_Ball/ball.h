#pragma once
#ifndef BALL_H
#define BALL_H
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

namespace ballspace
{
	extern const int SCREEN_WIDTH;
	extern const int SCREEN_HEIGHT;
	extern SDL_Window* gwindow;
	extern SDL_Renderer* grenderer;
	extern Mix_Music* gameMusic;
	extern TTF_Font* gfont;

	bool init();
	bool loadMedia();
	int checkCollision(SDL_Rect a, SDL_Rect b);
	void close();
	int loadStart();

	class Ltexture
	{
	private:
		SDL_Texture* mtexture;
		int mwidth;
		int mheight;

	public:
		Ltexture();
		bool loadFromFile(std::string path);
		bool loadFromRenderedText(std::string textureText, SDL_Color color);
		void render(int x, int y, SDL_Rect* clipRect = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flipType = SDL_FLIP_NONE);
		int getWidth();
		int getHeight();
		void free();
		~Ltexture();
	};

	class Ball
	{
	private:
		int xPos, yPos;
		int xVel;
		int yVel;
		SDL_Rect ballRect;
		const int dot_width = 19;
		const int dot_height = 19;

	public:
		Ball();
		void move(SDL_Rect a);
		void render();
		SDL_Rect getRect();
	};

	extern Ltexture ballTexture;
	extern Ltexture boardTexture;
	extern Ltexture startTexture;
	extern Ltexture background;
	extern Ltexture gameOver;
	class Board
	{
	private:
		SDL_Rect collider;
		static const int bwidth = 60;
		static const int bheight = 30;

	public:
		Board();
		void handleEvent(SDL_Event& e);
		void render();
		SDL_Rect getRect();
		
	};
}
#endif