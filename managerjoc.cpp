#include "managerjoc.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

Tabla ManagerJoc::t = Tabla();

SDL_Window* ManagerJoc::gWindow = NULL;

SDL_Renderer* ManagerJoc::gRenderer = NULL;

SDL_Rect ManagerJoc::tileSpriteClips[15];
SDL_Rect ManagerJoc::smileySpriteClips[6];
SDL_Rect ManagerJoc::borderSpriteClips[8];
SDL_Rect ManagerJoc::numberSpriteClips[11];
LTexture ManagerJoc::tileSpriteSheetTexture;
LTexture ManagerJoc::smileySpriteSheetTexture;
LTexture ManagerJoc::borderSpriteSheetTexture;
LTexture ManagerJoc::numberSpriteSheetTexture;

int ManagerJoc::offsetX = 0;
int ManagerJoc::offsetY = 32;
int ManagerJoc::borderHeight = 16;
int ManagerJoc::borderWidth = 16;
int ManagerJoc::topHeight = 48;

int ManagerJoc::cifreBombe[3] = {0}, ManagerJoc::cifreCeas[3] = {0};

int ManagerJoc::SCREEN_WIDTH = 16 * 9 + 2 * ManagerJoc::borderWidth;
int ManagerJoc::SCREEN_HEIGHT = 16 * 9 + 3 * ManagerJoc::borderHeight + ManagerJoc::topHeight;

bool ManagerJoc::init()
{
    srand(time(NULL));

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
		gWindow = SDL_CreateWindow( "Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
	if (!tileSpriteSheetTexture.loadFromFile("minesweeper_tiles.png") || \
        !smileySpriteSheetTexture.loadFromFile("smileys.png") || \
        !borderSpriteSheetTexture.loadFromFile("borders16.png") || \
        !numberSpriteSheetTexture.loadFromFile("numbers.png")
    ) {
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	} else {
	    for (int i = 0; i < 15; ++i) {
            tileSpriteClips[i].x = (i % 4) * 16;
            tileSpriteClips[i].y = (i / 4) * 16;
            tileSpriteClips[i].w = 16;
            tileSpriteClips[i].h = 16;
	    }

	    for (int i = 0; i < 6; ++i) {
            smileySpriteClips[i].x = (i % 4) * 32;
            smileySpriteClips[i].y = (i / 4) * 32;
            smileySpriteClips[i].w = 32;
            smileySpriteClips[i].h = 32;
	    }

	    for (int i = 0; i < 8; ++i) {
            borderSpriteClips[i].x = (i % 4) * 16;
            borderSpriteClips[i].y = (i / 4) * 16;
            borderSpriteClips[i].w = 16;
            borderSpriteClips[i].h = 16;
	    }

	    for (int i = 0; i < 11; ++i) {
            numberSpriteClips[i].x = i * 13;
            numberSpriteClips[i].y = 0;
            numberSpriteClips[i].w = 13;
            numberSpriteClips[i].h = 23;
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
    SDL_Rect smileyPos = {borderWidth + t.getLatime() * 8 - 16, borderHeight + topHeight / 2 - 16, 32, 32};
    int gameState = state_new;
    clock_t timp_inceput = 0;
    int timp = 0;

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
			bool quit = 0;
            int mouseX, mouseY;
            bool peTabla, peSmiley;
            int tablaX, tablaY;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
                SDL_GetMouseState(&mouseX, &mouseY);

                if (!(gameState == state_over)) {
                    tablaX = floor((double) (mouseX - borderWidth) / 16);
                    tablaY = floor((double) (mouseY - 2 * borderHeight - topHeight) / 16);
                }

                peTabla = (tablaX >= 0 && tablaX < t.getLatime() && tablaY >= 0 && tablaY < t.getInaltime());
                peSmiley = (smileyPos.x <= mouseX && mouseX < smileyPos.x + smileyPos.w && \
                smileyPos.y <= mouseY && mouseY < smileyPos.y + smileyPos.h);

                if (gameState == state_ongoing) {
                    timp = (clock() - timp_inceput) / CLOCKS_PER_SEC;
                }

				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{

					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					} else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
					    //click pe patratele
					    if (gameState != state_over && peTabla) {
                            //daca releasezi middle click sau
                            //releasezi unul din right si left click in timp ce celalalt e apasat
                            if ((e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT && \
                                SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) || \
                                (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT && \
                                SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) || \
                                (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_MIDDLE) \
                                ) {
                                t.clickMijloc(tablaX, tablaY);

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
                            } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                                if (gameState == state_new) {
                                    gameState = state_ongoing;
                                    timp_inceput = clock();
                                }

                                smileySprite = smiley_normal;

                                if (!t.eInitializat()) {
                                    t.initializeaza(tablaX, tablaY);
                                }

                                t.click(tablaX, tablaY);

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
                            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                                smileySprite = smiley_surprised;
                            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) {
                                t.schimbaSemn(tablaX, tablaY);
                            }
                        } else if (peSmiley && e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                            gameState = state_new;
                            smileySprite = smiley_normal;
                            t.reset(t.getLatime(), t.getInaltime(), 10);
                            timp = 0;
                        }
					}
				}

				numberSprites(timp, cifreCeas);
				numberSprites(t.getBombeRamase(), cifreBombe);

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xC0, 0xC0, 0xC0, 0xFF );
				SDL_RenderClear( gRenderer );

				renderBorder();

				for (int i = 0; i < 3; ++i) {
                    numberSpriteSheetTexture.render(borderWidth + 6 + i * 13, borderHeight + topHeight / 2 - 12, &numberSpriteClips[cifreBombe[i]]);
                    numberSpriteSheetTexture.render(borderWidth - 6 + t.getLatime() * 16 - 39 + i * 13, borderHeight + topHeight / 2 - 12, &numberSpriteClips[cifreCeas[i]]);
				}

				if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && peSmiley) {
                    smileySpriteSheetTexture.render(smileyPos.x, smileyPos.y, &smileySpriteClips[smiley_pressed]);
                    smileySpriteSheetTexture.render(smileyPos.x + 1, smileyPos.y + 1, &smileySpriteClips[smileySprite]);
				} else {
                    smileySpriteSheetTexture.render(smileyPos.x, smileyPos.y, &smileySpriteClips[smiley_unpressed]);
                    smileySpriteSheetTexture.render(smileyPos.x, smileyPos.y, &smileySpriteClips[smileySprite]);
				}

				for (int i = 0; i != t.getInaltime(); ++i) {
                    for (int j = 0; j != t.getLatime(); ++j) {
                        tileSpriteSheetTexture.render(j * 16 + borderWidth, i * 16 + 2 * borderHeight + topHeight, \
                        &tileSpriteClips[getTileSprite(t.m_Tabla[i][j])]);
                    }
				}

				//daca e apasat left click, arata ca apasat patratelul
				if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && gameState != state_over && peTabla) {
                    if (!t.m_Tabla[tablaY][tablaX].m_Apasat && \
                        !t.m_Tabla[tablaY][tablaX].m_Steag && \
                        !t.m_Tabla[tablaY][tablaX].m_Intrebare) {
                        tileSpriteSheetTexture.render(tablaX * 16 + borderWidth, tablaY * 16 + 2 * borderHeight + topHeight, &tileSpriteClips[tile_clear]);
                    }
				}

                //daca e apasat middle click sau left click si right click impreuna, arata ca apasate patratelele din 3x3
				if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE) || \
                (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && \
                SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)))
                && gameState != state_over && peTabla) {
                    for (int xdif = -1; xdif <= 1; ++xdif) {
                        for (int ydif = -1; ydif <= 1; ++ydif) {
                            if (t.validCoords(tablaX + xdif, tablaY + ydif) && \
                                !t.m_Tabla[tablaY + ydif][tablaX + xdif].m_Apasat && \
                                !t.m_Tabla[tablaY + ydif][tablaX + xdif].m_Steag && \
                                !t.m_Tabla[tablaY + ydif][tablaX + xdif].m_Intrebare) {
                                tileSpriteSheetTexture.render((tablaX + xdif) * 16 + borderWidth, \
                                (tablaY + ydif) * 16 + 2 * borderHeight + topHeight, &tileSpriteClips[tile_clear]);
                            }
                        }
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
        } else if (p.m_Intrebare) {
            return tile_question;
        } else {
            return tile_covered;
        }
    } else if (p.m_AreBomba) {
        if (p.m_BombaRosie) {
            return tile_mine_red;
        } else {
            return tile_mine;
        }
    } else if (p.m_BombaX) {
        return tile_mine_x;
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

void ManagerJoc::renderBorder()
{
    int latimeTabla = t.getLatime() * 16, inaltimeTabla = t.getInaltime() * 16;

    borderSpriteSheetTexture.render(0, 0, &borderSpriteClips[border_top_left]);
    borderSpriteSheetTexture.render(borderWidth + latimeTabla, 0, &borderSpriteClips[border_top_right]);
    borderSpriteSheetTexture.render(0, 2 * borderHeight + topHeight + inaltimeTabla, &borderSpriteClips[border_bottom_left]);
    borderSpriteSheetTexture.render(borderWidth + latimeTabla, 2 * borderHeight + topHeight + inaltimeTabla, &borderSpriteClips[border_bottom_right]);

    borderSpriteSheetTexture.render(0, borderHeight + topHeight, &borderSpriteClips[border_vertical_right]);
    borderSpriteSheetTexture.render(borderWidth + latimeTabla, borderHeight + topHeight, &borderSpriteClips[border_vertical_left]);

    for (int x = borderWidth; x < borderWidth + latimeTabla; x += borderWidth) {
        borderSpriteSheetTexture.render(x, 0, &borderSpriteClips[border_horizontal]);
        borderSpriteSheetTexture.render(x, borderHeight + topHeight, &borderSpriteClips[border_horizontal]);
        borderSpriteSheetTexture.render(x, 2 * borderHeight + topHeight + inaltimeTabla, &borderSpriteClips[border_horizontal]);
    }

    for (int y = borderHeight; y < borderHeight + topHeight; y += borderHeight) {
        borderSpriteSheetTexture.render(0, y, &borderSpriteClips[border_vertical]);
        borderSpriteSheetTexture.render(borderWidth + latimeTabla, y, &borderSpriteClips[border_vertical]);
    }

    for (int y = 2 * borderHeight + topHeight; y < 2 * borderHeight + topHeight + inaltimeTabla; y += borderHeight) {
        borderSpriteSheetTexture.render(0, y, &borderSpriteClips[border_vertical]);
        borderSpriteSheetTexture.render(borderWidth + latimeTabla, y, &borderSpriteClips[border_vertical]);
    }
}

void ManagerJoc::numberSprites(int n, int cifre[3])
{
    if (n >= 0) {
        if (n >= 999) {
            cifre[0] = cifre[1] = cifre[2] = number_9;
        } else {
            cifre[0] = n / 100;
            cifre[1] = (n % 100) / 10;
            cifre[2] = n % 10;
        }
    } else {
        cifre[0] = number_minus;

        if (n <= -99) {
            cifre[1] = cifre[2] = number_9;
        } else {
            cifre[1] = -n / 10;
            cifre[2] = -n % 10;
        }
    }
}
