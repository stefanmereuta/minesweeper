#include "managerjoc.h"
#include "managergrafica.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>

Tabla ManagerJoc::t = Tabla();

ManagerJoc::ManagerJoc(int inaltime, int latime, int bombe)
{
    //t = Tabla(inaltime, latime, bombe);

}

SDL_Window* ManagerJoc::gWindow = NULL;

SDL_Renderer* ManagerJoc::gRenderer = NULL;

SDL_Rect ManagerJoc::gSpriteClips[ 12 ];
LTexture ManagerJoc::gSpriteSheetTexture;

bool ManagerJoc::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool ManagerJoc::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "minesweeper_tiles.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{
	    for (int i = tile_covered; i <= tile_8; ++i) {
            gSpriteClips[i].x = (i % 4) * 16;
            gSpriteClips[i].y = (i / 4) * 16;
            gSpriteClips[i].w = 16;
            gSpriteClips[i].h = 16;
	    }
	}

	return success;
}

void ManagerJoc::close()
{
	//Free loaded images
	gSpriteSheetTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void ManagerJoc::start()
{
    t.reset(9, 9, 10);

    //Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					} else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == 1) {
					    int x, y;
					    SDL_GetMouseState(&x, &y);
                        t.click(x / 16, y / 16);
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				for (int i = 0; i != t.getInaltime(); ++i) {
                    for (int j = 0; j != t.getLatime(); ++j) {
                        gSpriteSheetTexture.render(j * 16, i * 16, &gSpriteClips[getSprite(t.m_Tabla[i][j])]);
                    }
				}

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();
}

