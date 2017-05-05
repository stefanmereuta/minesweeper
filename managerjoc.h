#ifndef MANAGERJOC_H
#define MANAGERJOC_H

#include <SDL.h>
#include <SDL_image.h>
#include "tabla.h"
#include "ltexture.h"

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
        tile_8
    };

    static Tabla t;
    static void start();

    static tiles getSprite(Patratel p)
    {
        if (!p.m_Apasat) {
            return tile_covered;
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

    ManagerJoc(int inaltime, int latime, int bombe);

    //Screen dimension constants
    static const int SCREEN_WIDTH = 16 * 9;//640;
    static const int SCREEN_HEIGHT = 16 * 9;//480;

    //Starts up SDL and creates window
    static bool init();

    //Loads media
    static bool loadMedia();

    //Frees media and shuts down SDL
    static void close();

    //static void start();

    //The window we'll be rendering to
    static SDL_Window* gWindow;

    //The window renderer
    static SDL_Renderer* gRenderer;

    //Scene sprites
    static SDL_Rect gSpriteClips[ 12 ];
    static LTexture gSpriteSheetTexture;
/*
    ManagerGrafica()
    {

    }*/
};

#endif // MANAGERJOC_H

