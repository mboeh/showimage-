
#include "image.h"
#include "SDL_image.h"

int img_initialize(void)
{
    return 0;
}

SDL_Surface *img_load(const char *fname)
{
    return IMG_Load(fname);
}

void img_free(SDL_Surface* img)
{
  if (img) SDL_FreeSurface(img);
}

int img_deinitialize(void)
{
    return 0;
}
