
#ifndef _SHOWIMAGEPLUS_H_
#define _SHOWIMAGEPLUS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SDL.h"
#include "imagelist.h"

#define PROGRAM_NAME "showimage+"

#define SIP_USE_FONT_SUPPORT

/* draw_background(screen):
   blits a checkered gray pattern that fills the given surface. This pattern
   will appear in transparent regions of the image.
*/
void draw_background(SDL_Surface * screen, int pattern);

#endif
