#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Input.h"
#include <functional>
#include "World.h"
#include "Point.h"
#include "TextureManager.h"
#include "Globals.h"
#include "Renderer.h"
#include "Player.h"
#include "Camera.h"
#include "MoveableCamera.h"
#define CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace GameEngine;

//Screen dimension constants
//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;

const int texturesize = 16;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

GameEngine::World* game_world = nullptr;

GameEngine::TextureManager* texture_manager = nullptr;
GameEngine::Renderer* renderer = nullptr;
//using namespace GameEngine;
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: %s\n" << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			std::cout << "Window could not be created! SDL Error: %s\n" << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL Error: %s\n" << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "SDL_image could not initialize! SDL_image Error: %s\n" << IMG_GetError() << std::endl;
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	gTexture = loadTexture("Art/Maybe_Grass.png");
	if (gTexture == NULL)
	{
		std::cout << "Failed to load texture image!" << std::endl;
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Unable to load image %s! SDL_image Error: %s\n" << path.c_str() << IMG_GetError() << std::endl;
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			std::cout << "Unable to create texture from %s! SDL Error: %s\n" << path.c_str() << SDL_GetError() << std::endl;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

void lalala(GameEngine::KeyClickArgs* args, int i) {
	SDL_Log(std::to_string(args->down).c_str());
	SDL_Log(std::to_string(args->scan_code).c_str());
}

void mouseLis(MouseClickArgs* args, int i) {
	std::string cord = std::to_string(args->point.x) + "," + std::to_string(args->point.y);
	SDL_Log(cord.c_str());
	SDL_Log(std::to_string(args->down).c_str());
	SDL_Log(std::to_string(args->scan_code).c_str());
}

void mouseMov(MouseMoveArgs* args, int i) {
	SDL_Log(std::to_string(args->dragging).c_str());
	std::string cord = std::to_string(args->point.x) + "," + std::to_string(args->point.y);
	SDL_Log(cord.c_str());
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		std::cout << "Failed to initialize!" << std::endl;
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			std::cout << "Failed to load media!" << std::endl;
		}
		else
		{
			Point p{ 4 ,3 };
			std::cout << p.toStr() << std::endl;

			game_world = new World();
			renderer = new Renderer(gRenderer);
			//texture_manager = new TextureManager(gRenderer);

			Global::texture_manager.setupDefault(gRenderer);
			int grass_texture_ID =	Global::texture_manager.add(loadTexture("Art/Maybe_Grass.png"), "grass");
			int player_texture_ID = Global::texture_manager.add(loadTexture("Art/Little_Player.png"), "player");

			for (int x = 0; x < 140; x++) {
				for (int y = 0; y < 120; y++) {
					Tile* tile = new Tile("grass");
					game_world->map->set(tile, Point(x, y));
				}
			}

			Player* player = new Player(10, 10, "player");
			game_world->entity_list->entities.push_back(player);

			Camera* camera = new MoveableCamera(100, 0);
			game_world->camera = camera;
			game_world->entity_list->entities.push_back(camera);

			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			decltype(GameEngine::keyClickListeners)::callbackType func = lalala;
			decltype(mouseClickListeners)::callbackType mfunc = mouseLis;
			keyUpListeners.add( func);
			keyDownListeners.add(func);
			mouseDownListeners.add(mfunc);
			mouseUpListeners.add(mfunc);
			decltype(mouseMoveListeners)::callbackType mofunc = mouseMov;
			int mofunc_id = mouseMoveListeners.add(mofunc);

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else {
						GameEngine::handle_SDL_Event(&e);
					}
				}

				//Clear screen
				SDL_RenderClear(gRenderer);

				SingleTexture& texture = Global::texture_manager.get(1);
				//renderer->renderTileMap(*game_world->map);
				//renderer->renderEntityList(*game_world->entity_list);
				renderer->renderWorld(*game_world);

				//for (int x = 0; x < SCREEN_WIDTH; x += texturesize) {
				//	for (int y = 0; y < SCREEN_HEIGHT; y += texturesize) {
				//		SDL_Rect rect = { x, y, texturesize, texturesize };
				//		//SDL_RenderCopy(gRenderer, gTexture, NULL, &rect);
				//		//renderSingleTexture(texture, &rect);
				//		renderer->renderSingleTexture(&texture, rect);
				//	}//TODO: Fix this block
				//}
				//SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	_CrtDumpMemoryLeaks();

	return 0;
}