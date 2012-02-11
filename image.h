
#ifndef _SHOWIMAGEPLUS_IMAGE_H_
#define _SHOWIMAGEPLUS_IMAGE_H_

#include "SDL.h"

int img_initialize(void);
SDL_Surface *img_load(const char *fname);
void img_free(SDL_Surface* img);
int img_deinitialize(void);

#endif
