
#include "imagelist.h"
#include <stdlib.h>

ImageList* new_imagelist(const char* filename)
{
  ImageList* list;

  list = (ImageList*) malloc(sizeof(ImageList));

  list->filename = filename;
  list->image = NULL;
  list->prev = NULL;
  list->next = NULL;

  return list;
}

ImageList* imagelist_add(ImageList* list, const char* filename)
{
  ImageList* newitem;

  newitem = (ImageList*) malloc(sizeof(ImageList));

  newitem->filename = filename;
  newitem->image = NULL;
  newitem->prev = list;
  newitem->next = NULL;

  list->next = newitem;

  return newitem;
}

ImageList* imagelist_remove(ImageList* list)
{
  ImageList* other;
  if (list->prev) {
    list->prev->next = list->next;
    if (list->next)
      list->next->prev = list->prev;
  } else {
    if (list->next)
      list->next->prev = NULL;
  }
  
  other = list->next ? list->next : list->prev;
  
  if (list->image)
    free(list->image);
  free(list);
  return other;
}

ImageList* imagelist_first(ImageList* list)
{
  while(list->prev)
    list = list->prev;

  return list;
}

ImageList* imagelist_last(ImageList* list)
{
  while(list->next)
    list = list->next;

  return list;
}

ImageList* imagelist_forward(ImageList* list, int step)
{
  for (; step; step--)  {
    if (list->next) {
      list = list->next;
    } else {
      break;
    }
  }
  
  return list;
}

ImageList* imagelist_back(ImageList* list, int step)
{
  for (; step; step--)  {
    if (list->prev) {
      list = list->prev;
    } else {
      break;
    }
  }
  
  return list;
}

const char* imagelist_get_filename(ImageList* list)
{
  return list->filename;
}

SDL_Surface* imagelist_get_image(ImageList* list)
{
  if (!list->image) {
    list->image = img_load(list->filename);
  }

  return list->image;
}

void imagelist_clear_cache(ImageList* list)
{
  if (list->image) {
    img_free(list->image);
    list->image = NULL;
  }
}

void imagelist_clear_cache_all(ImageList* list)
{
  list = imagelist_first(list);

  do {
    imagelist_clear_cache(list);
  } while (list->next && (list = imagelist_forward(list, 1)));

}
