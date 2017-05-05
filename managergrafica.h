/*#ifndef MANAGERGRAFICA_H
#define MANAGERGRAFICA_H

#include "ltexture.h"

class ManagerGrafica
{
public:
    //Screen dimension constants
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 480;

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
    static SDL_Rect gSpriteClips[ 4 ];
    static LTexture gSpriteSheetTexture;

    ManagerGrafica()
    {

    }
};

#endif // MANAGERGRAFICA_H
*/
