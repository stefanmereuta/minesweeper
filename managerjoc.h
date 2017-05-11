#ifndef MANAGERJOC_H
#define MANAGERJOC_H

#include <SDL.h>
#include <SDL_image.h>
#include "tabla.h"
#include "ltexture.h"

struct coords
{
    int x, y;
};

class ManagerJoc
{
public:
    enum tiles
    {
        tile_covered,
        tile_flag,
        tile_mine,
        tile_clear,
        tile_1,
        tile_2,
        tile_3,
        tile_4,
        tile_5,
        tile_6,
        tile_7,
        tile_8,
        tile_mine_red,
        tile_question,
        tile_mine_x
    };

    enum game_states
    {
        state_new,
        state_ongoing,
        state_over
    };

    enum smiley_sprites
    {
        smiley_normal,
        smiley_surprised,
        smiley_nice,
        smiley_dead
    };

    static Tabla t;

    static int offsetX, offsetY;

    static void start();

    static tiles getTileSprite(Patratel p);

    //Screen dimension constants
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;

    //Starts up SDL and creates window
    static bool init();

    //Loads media
    static bool loadMedia();

    //Frees media and shuts down SDL
    static void close();

    //The window we'll be rendering to
    static SDL_Window* gWindow;

    //The window renderer
    static SDL_Renderer* gRenderer;

    //Scene sprites
    static SDL_Rect tileSpriteClips[15], smileySpriteClips[4];
    static LTexture tileSpriteSheetTexture, smileySpriteSheetTexture;
};

#endif // MANAGERJOC_H

