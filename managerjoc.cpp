#include "managerjoc.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>

Tabla ManagerJoc::t = Tabla();

SDL_Window* ManagerJoc::gWindow = NULL;

SDL_Renderer* ManagerJoc::gRenderer = NULL;

SDL_Rect ManagerJoc::tileSpriteClips[12], ManagerJoc::smileySpriteClips[4];
LTexture ManagerJoc::tileSpriteSheetTexture, ManagerJoc::smileySpriteSheetTexture;

int ManagerJoc::offsetX = 0;
int ManagerJoc::offsetY = 32;

int ManagerJoc::SCREEN_WIDTH = 16 * 9 + ManagerJoc::offsetX;
int ManagerJoc::SCREEN_HEIGHT = 16 * 9 + ManagerJoc::offsetY;

bool ManagerJoc::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		//Set texture filtering to linear
		if(!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if(!gWindow)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(!gRenderer) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xC0, 0xC0, 0xC0, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError());
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
	if(!tileSpriteSheetTexture.loadFromFile("minesweeper_tiles.png") || \
    !smileySpriteSheetTexture.loadFromFile("smileys.png")) {
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	} else {
	    for (int i = 0; i < 12; ++i) {
            tileSpriteClips[i].x = (i % 4) * 16;
            tileSpriteClips[i].y = (i / 4) * 16;
            tileSpriteClips[i].w = 16;
            tileSpriteClips[i].h = 16;
	    }

	    for (int i = 0; i < 4; ++i) {
            smileySpriteClips[i].x = i * 32;
            smileySpriteClips[i].y = 0;
            smileySpriteClips[i].w = 32;
            smileySpriteClips[i].h = 32;
	    }
	}

	return success;
}

void ManagerJoc::close()
{
	//Free loaded images
	tileSpriteSheetTexture.free();
	smileySpriteSheetTexture.free();

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void ManagerJoc::start()
{
    t.reset(9, 9, 10);
    smiley_sprites smileySprite = smiley_normal;
    SDL_Rect smileyPos = {t.getLatime() * 8 - 16, 0, 32, 32};
    int gameState = state_new;

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
                int x, y;
                SDL_GetMouseState(&x, &y);
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{

					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					} else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
                        int x, y;
                        SDL_GetMouseState(&x, &y);

					    //click pe patratele
					    if (gameState != state_over && offsetX <= x && x < offsetX + t.getLatime() * 16 && offsetY <= y && y < offsetY + t.getInaltime() * 16) {
                            gameState = state_ongoing;
                            if (e.type == SDL_MOUSEBUTTONUP && e.button.button == 1) {
                                smileySprite = smiley_normal;
                                t.click((x - offsetX) / 16, (y - offsetY) / 16);

                                if (t.m_Over) {
                                    gameState = state_over;

                                    if (t.m_Won) {
                                        smileySprite = smiley_nice;
                                        t.flagMines();
                                    } else {
                                        smileySprite = smiley_dead;
                                        t.revealMines();
                                    }
                                }
                            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == 1) {
                                smileySprite = smiley_surprised;
                            }
                        } else if (smileyPos.x <= x && x < smileyPos.x + smileyPos.w && smileyPos.y <= y && y < smileyPos.y + smileyPos.h) {
                            //std::cout << "da\n";
                            gameState = state_new;
                            smileySprite = smiley_normal;
                            t.reset(t.getLatime(), t.getInaltime(), 10);
                        }
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xC0, 0xC0, 0xC0, 0xC0 );
				SDL_RenderClear( gRenderer );

				smileySpriteSheetTexture.render(t.getLatime() * 8 + offsetX - 16, 0, &smileySpriteClips[smileySprite]);

				for (int i = 0; i != t.getInaltime(); ++i) {
                    for (int j = 0; j != t.getLatime(); ++j) {
                        tileSpriteSheetTexture.render(j * 16 + offsetX, i * 16 + offsetY, &tileSpriteClips[getTileSprite(t.m_Tabla[i][j])]);
                    }
				}

				if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT) && gameState != state_over && offsetX <= x && x < offsetX + t.getLatime() * 16 && offsetY <= y && y < offsetY + t.getInaltime() * 16) {
                    if (!t.m_Tabla[(y - offsetY) / 16][(x - offsetX) / 16].m_Apasat) {
                        tileSpriteSheetTexture.render((x - offsetX) / 16 * 16 + offsetX, (y - offsetY) / 16 * 16 + offsetY, &tileSpriteClips[tile_clear]);
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

ManagerJoc::tiles ManagerJoc::getTileSprite(Patratel p)
{
    if (!p.m_Apasat) {
        if (p.m_Steag) {
            return tile_flag;
        } else {
            return tile_covered;
        }
    } else if (p.m_AreBomba) {
        return tile_mine;
    } else if (!p.m_BombeInJur) {
        return tile_clear;
    } else if (p.m_BombeInJur == 1) {
        return tile_1;
    } else if (p.m_BombeInJur == 2) {
        return tile_2;
    } else if (p.m_BombeInJur == 3) {
        return tile_3;
    } else if (p.m_BombeInJur == 4) {
        return tile_4;
    } else if (p.m_BombeInJur == 5) {
        return tile_5;
    } else if (p.m_BombeInJur == 6) {
        return tile_6;
    } else if (p.m_BombeInJur == 7) {
        return tile_7;
    } else if (p.m_BombeInJur == 8) {
        return tile_8;
    }
    return tile_covered;
}
