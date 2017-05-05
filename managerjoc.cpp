#include "managerjoc.h"
#include "managergrafica.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>


ManagerJoc::ManagerJoc(int inaltime, int latime, int bombe)
{
    //t = Tabla(inaltime, latime, bombe);

}

SDL_Window* ManagerJoc::gWindow = NULL;

SDL_Renderer* ManagerJoc::gRenderer = NULL;

SDL_Rect ManagerJoc::gSpriteClips[ 4 ];
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
		//Set top left sprite
		gSpriteClips[ 0 ].x =   0;
		gSpriteClips[ 0 ].y =   0;
		gSpriteClips[ 0 ].w = 100;
		gSpriteClips[ 0 ].h = 100;

		//Set top right sprite
		gSpriteClips[ 1 ].x = 100;
		gSpriteClips[ 1 ].y =   0;
		gSpriteClips[ 1 ].w = 100;
		gSpriteClips[ 1 ].h = 100;

		//Set bottom left sprite
		gSpriteClips[ 2 ].x =   0;
		gSpriteClips[ 2 ].y = 100;
		gSpriteClips[ 2 ].w = 100;
		gSpriteClips[ 2 ].h = 100;

		//Set bottom right sprite
		gSpriteClips[ 3 ].x = 100;
		gSpriteClips[ 3 ].y = 100;
		gSpriteClips[ 3 ].w = 100;
		gSpriteClips[ 3 ].h = 100;
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
    //Start up SDL and create window
	if( !ManagerJoc::init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !ManagerJoc::loadMedia() )
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
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( ManagerJoc::gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( ManagerJoc::gRenderer );

				//Render top left sprite
				ManagerJoc::gSpriteSheetTexture.render( 0, 0, &ManagerJoc::gSpriteClips[ 0 ] );

				//Render top right sprite
				ManagerJoc::gSpriteSheetTexture.render( SCREEN_WIDTH - ManagerJoc::gSpriteClips[ 1 ].w, 0, &ManagerJoc::gSpriteClips[ 1 ] );

				//Render bottom left sprite
				ManagerJoc::gSpriteSheetTexture.render( 0, SCREEN_HEIGHT - ManagerJoc::gSpriteClips[ 2 ].h, &ManagerJoc::gSpriteClips[ 2 ] );

				//Render bottom right sprite
				ManagerJoc::gSpriteSheetTexture.render( SCREEN_WIDTH - ManagerJoc::gSpriteClips[ 3 ].w, SCREEN_HEIGHT - ManagerJoc::gSpriteClips[ 3 ].h, &ManagerJoc::gSpriteClips[ 3 ] );

				//Update screen
				SDL_RenderPresent( ManagerJoc::gRenderer );
			}
		}
	}

	//Free resources and close SDL
	ManagerJoc::close();
}

