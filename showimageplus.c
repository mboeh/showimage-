/*
 *  showimage+:  A very simple image viewer using SDL_image.
 *  Copyright (C) 1999, 2000, 2001  Sam Lantinga
 *  Copyright (C) 2003 Matt Boeh
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
 * Sam Lantinga
 * slouken@libsdl.org
 *
 * Matt Boeh
 * mboeh@desperance.net
*/

#include "showimageplus.h"

/* trigger_next:
   Callback for slideshow that sends a fake "space pressed" message to SDL,
   making the program view the next image.
*/
Uint32 trigger_next(Uint32 interval, void *param)
{
  SDL_Event e;
  e.type = SDL_KEYUP;
  e.key.keysym.sym = SDLK_SPACE;
  SDL_PushEvent(&e);
  return interval;
}

struct option_s {
  int interval;
  int resize;
  int alpha;
  int alphapattern;
  int nocache;
};

void usage(const char *zero)
{
  fprintf(stderr,
          "Usage: %s [-s msec] [-r] <image_file> [...]\n"
              PROGRAM_NAME " " VERSION "\n" "\tOptions:\n"
              "\t -s[lideshow] msec: causes the next image to be automatically shown after msec milliseconds\n"
              "\t -r[esize]: allows the viewport to be resized.\n",
              zero);
}

ImageList* process_args(int argc, char **argv, struct option_s *opt) {
  ImageList* ilist = NULL;
  int i = 0;
  int lastoption = 0;

  for (i = 1; i < argc; ++i) {
    if (strncmp(argv[i], "-slideshow", 2) == 0) {
      opt->interval = atoi(argv[++i]);
      lastoption = i;
      continue;
    } else if (strncmp(argv[i], "-resize", 2) == 0) {
      opt->resize = 1;
      lastoption = i;
      continue;
    } else if (strncmp(argv[i], "-nocache", 2) == 0) {
      opt->nocache = 1;
      lastoption = i;
      continue;
    }
    
    if (!ilist) {
      ilist = new_imagelist(argv[i]);
    } else {
      ilist = imagelist_add(ilist, argv[i]);
    }
  }

  return ilist;
}

int main(int argc, char *argv[])
{
  struct option_s opt;
  
  Uint32 flags;
  SDL_Surface *screen, *image;
  int depth, done, dirty;
  SDL_Event event;
  SDL_Rect src = { 0, 0, 0, 0 };
  int xhop, yhop;
  SDL_TimerID death = NULL;
  ImageList* ilist = NULL;
  int end_app = 0;
  int step = 0;
  int new_w, new_h;
  new_w = new_h = 0;
  xhop = yhop = 0;
  opt.resize = 0;
  opt.alpha = 1;
  opt.alphapattern = 0;

  /* Check command line usage */
  if (!argv[1]) {
      usage(argv[0]);
      return (1);
  }

  /* Initialize the SDL library */
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      return (255);
  }
  
  img_initialize();
  death = SDL_AddTimer(99999, trigger_next, NULL);
    
  ilist = process_args(argc, argv, &opt);

  if (!ilist) {
    usage(argv[0]);
  } else {

    ilist = imagelist_first(ilist);
    
    while(ilist && !end_app)  {
      flags = SDL_SWSURFACE;

      /* Open the image file */
      image = imagelist_get_image(ilist);
      if (!image) {
          fprintf(stderr, "Couldn't load %s: %s\n",
                  imagelist_get_filename(ilist), SDL_GetError());
          ilist = imagelist_remove(ilist);
          continue;
      }
      
      /* Create a display for the image */
      src.w = new_w;
      if (!src.w || image->w < src.w) {
          src.w = image->w;
      }
      src.h = new_h;
      if (!src.h || image->h < src.h) {
          src.h = image->h;
      }

      xhop = src.w / 2;
      yhop = src.h / 2;

      depth = SDL_VideoModeOK(src.w, src.h, 32, flags);
      /* Use the deepest native mode, except that we emulate 32bpp
         for viewing non-indexed images on 8bpp screens */
      if (depth == 0) {
          if (image->format->BytesPerPixel > 1) {
              depth = 32;
          } else {
              depth = 8;
          }
      } else if ((image->format->BytesPerPixel > 1) && (depth == 8)) {
          depth = 32;
      }

      if (depth == 8)
        flags |= SDL_HWPALETTE;
      if (opt.resize)
        flags |= SDL_RESIZABLE;
      if (opt.alpha)
        flags |= SDL_SRCALPHA;
      
      SDL_SetAlpha(image, opt.alpha ? SDL_SRCALPHA : 0, 255);

      screen = SDL_SetVideoMode(src.w, src.h, depth, flags);
     
      if (screen == NULL) {
          fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",
                  src.w, src.h, depth, SDL_GetError());
          continue;
      }

      /* Set the palette, if one exists */
      if (image->format->palette) {
          SDL_SetColors(screen, image->format->palette->colors,
                        0, image->format->palette->ncolors);
      }

      /* Draw a background pattern if the surface has transparency */
      if (image->flags & (SDL_SRCALPHA | SDL_SRCCOLORKEY))
          draw_background(screen, opt.alphapattern);

      /* Display the image */

      done = 0;
      dirty = 1;

      new_w = new_h = 0;
      
      if (death) {
          SDL_RemoveTimer(death);
          if (opt.interval)
              death = SDL_AddTimer(opt.interval, trigger_next, NULL);
      }
      
      while (!done) {
          if (dirty) {
              SDL_BlitSurface(image, &src, screen, NULL);
              SDL_UpdateRect(screen, 0, 0, 0, 0);
              dirty = 0;
          }
          if (SDL_PollEvent(&event)) {
              switch (event.type) {
              case SDL_VIDEORESIZE:
                new_w = event.resize.w;
                new_h = event.resize.h;
                done = 1;
                break;
              case SDL_KEYUP:
                  switch (event.key.keysym.sym) {
                  case SDLK_LEFT:
                    if (ilist != imagelist_first(ilist)) {
                      if (opt.nocache) imagelist_clear_cache(ilist);
                      ilist = imagelist_back(ilist, step ? step : 1);
                      done = 1;
                      step = 0;
                    }
                    break;
                  case SDLK_RIGHT:
                    if (ilist != imagelist_last(ilist)) {
                      if (opt.nocache) imagelist_clear_cache(ilist);
                      ilist = imagelist_forward(ilist, step ? step : 1);
                      done = 1;
                      step = 0;
                    }
                    break;
                  case SDLK_ESCAPE:
                  case SDLK_q:
                    end_app = 1;
                    done = 1;
                    break;
                  case SDLK_SPACE:
                    step = 0;
                    break;
                  case SDLK_TAB:
                    if (opt.alphapattern < 2) opt.alphapattern++;
                    else opt.alphapattern = 0;

                    done = 1;
                    break;
                  case SDLK_p:
                    if (death) {
                        SDL_RemoveTimer(death);
                        death = NULL;
                    } else if (opt.interval)
                        death =
                            SDL_AddTimer(opt.interval, trigger_next,
                                         (void *) &done);
                    break;
                  case SDLK_a:
                    opt.alpha = !opt.alpha;
                    done = 1;
                    break;
                  case SDLK_c:
                    imagelist_clear_cache_all(ilist);
                    done = 1;
                    break;
                  case SDLK_COMMA:
                      if (src.x - xhop <= 0)
                          src.x = 0;
                      else
                          src.x -= xhop;
                      dirty = 1;
                      break;
                  case SDLK_PERIOD:
                      if (src.x + xhop + src.w >= image->w)
                          src.x = image->w - src.w;
                      else
                          src.x += xhop;
                      dirty = 1;
                      break;
                  case SDLK_UP:
                      if (src.y - yhop <= 0)
                          src.y = 0;
                      else
                          src.y -= yhop;
                      dirty = 1;
                      break;
                  case SDLK_DOWN:
                      if (src.y + yhop + src.h >= image->h)
                          src.y = image->h - src.h;
                      else
                          src.y += yhop;
                      dirty = 1;
                      break;
                  case SDLK_HOME:
                      if (opt.nocache) imagelist_clear_cache(ilist);
                      ilist = imagelist_first(ilist);
                      done = 1;
                      break;
                  case SDLK_END:
                      if (opt.nocache) imagelist_clear_cache(ilist);
                      ilist = imagelist_last(ilist);
                      done = 1;
                      break;
                  case SDLK_DELETE:
                      ilist = imagelist_remove(ilist);
                      done = 1;
                      break;
                  case SDLK_0:
                  case SDLK_1:
                  case SDLK_2:
                  case SDLK_3:
                  case SDLK_4:
                  case SDLK_5:
                  case SDLK_6:
                  case SDLK_7:
                  case SDLK_8:
                  case SDLK_9:
                      step *= 10;
                      step += event.key.keysym.sym - 48; 
                      break;

                  default:
                      break;
                  }
                  break;
              case SDL_MOUSEMOTION:
                if (event.motion.state & SDL_BUTTON(1)) {
                  if (src.y + src.h - event.motion.yrel >= image->h)
                    src.y = image->h - src.h;
                  else if (src.y - event.motion.yrel < 0)
                    src.y = 0;
                  else
                    src.y -= event.motion.yrel;
                  if (src.x + src.w - event.motion.xrel >= image->w)
                    src.x = image->w - src.w;
                  else if (src.x - event.motion.xrel < 0)
                    src.x = 0;
                  else
                    src.x -= event.motion.xrel;

                  dirty = 1;
                } 
                break;
              case SDL_QUIT:
                end_app = 1;
                done = 1;
                break;
              default:
                break;
              }
          } else {
              SDL_Delay(10);
          }
      }
      src.x = src.y = 0;
    }
  }

  /* We're done! */
  img_deinitialize();
  SDL_Quit();
  return (0);
}
