#include "caGlPort_Texture.h"


#include "sdl/sdl_image.h"
#include "sdl/sdl_rwops.h"
#include "sdl/sdl_video.h"
//#include "sdl/sdl_ttf.h"


#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL.lib")
//#pragma comment(lib, "SDL_ttf.lib")



#define IS_NOT_POWER_OF_2(a) ((a)-1 & (a))
#define IS_POWER_OF_2(a) (!IS_NOT_POWER_OF_2(a))

glTexture::~glTexture()
{
    if (tex)
    {
        glDeleteTextures(1, &tex);
    }
}
void glTexture::loadTexture(const char *filename)
{
    if (!tex)
    {
        glGenTextures(1, &tex);
    }
    if (tex)
    {
        SDL_RWops *sdl_rwops = SDL_RWFromFile(filename, "rb");
        if (!sdl_rwops)
            return;
         
        SDL_Surface *sdl_surface = NULL;
        bool valid_type = true;
        //* these are...
#define SEEK SDL_RWseek(sdl_rwops, 0, SEEK_SET)
        /* something like the below if..else if..else cases
        if (SEEK, IMG_isPNG(sdl_rwops))
        {
            SEEK;
            sdl_surface = IMG_LoadPNG_RW(sdl_rwops);
        }
        else
            valid_type = false;
        */
#define TRY_LOAD_IMG(surface, op, type) \
    (SEEK, IMG_is##type(op)) && (SEEK, 0 != (surface = IMG_Load##type##_RW(op)))

        TRY_LOAD_IMG(sdl_surface, sdl_rwops, PNG) ||
        //(SEEK, Utilities::ogl2dIMG_isTGA(sdl_rwops)) && (SEEK, sdl_surface = Utilities::ogl2dIMG_LoadTGA_RW(sdl_rwops)) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, BMP) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, PCX) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, GIF) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, JPG) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, TIF) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, LBM) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, PNM) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, XPM) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, XCF) ||
        0 == (valid_type = false);

        SDL_RWclose(sdl_rwops);

        if (!valid_type || sdl_surface == NULL || sdl_surface->pixels == NULL)
            return;

        if (sdl_surface->w == 0 || sdl_surface->h == 0 || 
            sdl_surface->pitch != sdl_surface->format->BytesPerPixel * sdl_surface->w ||        
            IS_NOT_POWER_OF_2(sdl_surface->w) || IS_NOT_POWER_OF_2(sdl_surface->h)) // not valid
        {
            assert(0 && "glport only support textures with 2-power-sized pictures");
            SDL_FreeSurface(sdl_surface);
            return;
        }

        int nchannels = sdl_surface->format->BytesPerPixel;

        glBindTexture(GL_TEXTURE_2D, tex);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        switch(nchannels)
        {
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width = sdl_surface->w, height = sdl_surface->h, 
                0/*No border*/, GL_RGB, GL_UNSIGNED_BYTE, sdl_surface->pixels);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width = sdl_surface->w, height = sdl_surface->h, 
                0/*No border*/, GL_RGBA, GL_UNSIGNED_BYTE, sdl_surface->pixels);
            break;
        }

        SDL_FreeSurface(sdl_surface);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST
    }
}