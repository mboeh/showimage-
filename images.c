/*
 Last edited: <29 Oct 2003 14:49:07 m. boeh>
*/
#include "showimageplus.h"

void draw_background(SDL_Surface * screen, int pattern)
{
  Uint8 *dst = screen->pixels;
  int x, y;
  int bpp = screen->format->BytesPerPixel;
  Uint32 col[2];

  switch (pattern) {
  case 0:
    /* This was all here when I got here. -- Matt */
    col[0] = SDL_MapRGB(screen->format, 0x66, 0x66, 0x66);
    col[1] = SDL_MapRGB(screen->format, 0x99, 0x99, 0x99);
    for (y = 0; y < screen->h; y++) {
        for (x = 0; x < screen->w; x++) {
            /* use an 8x8 checkerboard pattern */
            Uint32 c = col[((x ^ y) >> 3) & 1];
            switch (bpp) {
            case 1:
                dst[x] = c;
                break;
            case 2:
                ((Uint16 *) dst)[x] = c;
                break;
            case 3:
                if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
                    dst[x * 3] = c;
                    dst[x * 3 + 1] = c >> 8;
                    dst[x * 3 + 2] = c >> 16;
                } else {
                    dst[x * 3] = c >> 16;
                    dst[x * 3 + 1] = c >> 8;
                    dst[x * 3 + 2] = c;
                }
                break;
            case 4:
                ((Uint32 *) dst)[x] = c;
                break;
            }
        }
        dst += screen->pitch;
    }
    break;
  case 1:
    SDL_FillRect(screen, NULL, 0);
    break;
  case 2:
    SDL_FillRect(screen, NULL, 0xFFFFFFFF);
    break;
  }
}

