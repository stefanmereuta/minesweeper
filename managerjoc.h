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
        smiley_dead,
        smiley_unpressed,
        smiley_pressed
    };

    enum border_sprites
    {
        border_horizontal,
        border_vertical,
        border_vertical_right,
        border_vertical_left,
        border_top_left,
        border_top_right,
        border_bottom_right,
        border_bottom_left
    };

    enum number_sprites
    {
        number_0,
        number_1,
        number_2,
        number_3,
        number_4,
        number_5,
        number_6,
        number_7,
        number_8,
        number_9,
        number_minus
    };

    static Tabla t;

    static int offsetX, offsetY, borderHeight, borderWidth, topHeight;

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

    static void renderBorder();

    //The window we'll be rendering to
    static SDL_Window* gWindow;

    //The window renderer
    static SDL_Renderer* gRenderer;

    //Scene sprites
    static SDL_Rect tileSpriteClips[15], smileySpriteClips[6], borderSpriteClips[8], numberSpriteClips[11];
    static LTexture tileSpriteSheetTexture, smileySpriteSheetTexture, borderSpriteSheetTexture, numberSpriteSheetTexture;
};

#endif // MANAGERJOC_H

