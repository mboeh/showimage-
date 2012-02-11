/* imagelist.h: declaration of the ImageList linked-list structure.
 * Part of showimage+, a simple image viewer using SDL_image.
 * Copyright (C) 2005 Matt Boeh.
 *
 *  This application is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This application is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this application; if not, write to the Free
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Matthew Boeh
 * mboeh@desperance.net
*/

#ifndef _SHOWIMAGEPLUS_IMAGELIST_H_
#define _SHOWIMAGEPLUS_IMAGELIST_H_

#include "image.h"

typedef struct ImageList_t {
  const char*  filename;
  SDL_Surface* image;

  struct ImageList_t* prev;
  struct ImageList_t* next;
} ImageList;

ImageList* new_imagelist(const char* filename);

ImageList* imagelist_add(ImageList* list, const char* filename);
ImageList* imagelist_remove(ImageList* list);
ImageList* imagelist_first(ImageList* list);
ImageList* imagelist_last(ImageList* list);
ImageList* imagelist_forward(ImageList* list, int step);
ImageList* imagelist_back(ImageList* list, int step);

const char* imagelist_get_filename(ImageList* list);
SDL_Surface* imagelist_get_image(ImageList* list);

void imagelist_clear_cache(ImageList* list);
void imagelist_clear_cache_all(ImageList* list);

#if 0
void imagelist_delete(ImageList* list);
void imagelist_delete_all(ImageList* list);
#endif

#endif
