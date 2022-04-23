#ifndef LOVESDL_H
#define LOVESDL_H

#include "miniray.h"
#include "dmt.h"
#include "luaobj.h"
#include <SDL2/SDL.h>

typedef unsigned char pixel_t;


#if defined(__STDC__) && __STDC_VERSION__ >= 199901L
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
    typedef enum { false, true } bool;
#endif

#define ASSERT(x)\
  do {\
    if (!(x)) {\
      SDL_Log("%s:%d: %s(): assertion '%s' failed\n",\
              __FILE__, __LINE__, __func__, #x);\
      abort();\
    }\
  } while (0)

#define UNUSED(x)       ((void) (x))
#define MIN(a, b)       ((b) < (a) ? (b) : (a))
#define MAX(a, b)       ((b) > (a) ? (b) : (a))
#define CLAMP(x, a, b)  (MAX(a, MIN(x, b)))
#define LERP(a, b, p)   ((a) + ((b) - (a)) * (p))


#define DMTMALLOC(sz)       dmt_malloc(sz)
#define DMTCALLOC(n,sz)     dmt_calloc(n,sz)
#define DMTREALLOC(ptr,sz)  dmt_realloc(ptr,sz)
#define DMTFREE(ptr)        dmt_free(ptr)

#define RL_MALLOC(sz)       SDL_malloc(sz)
#define RL_CALLOC(n,sz)     SDL_calloc(n,sz)
#define RL_REALLOC(n,sz)    SDL_realloc(n,sz)
#define RL_FREE(p)          SDL_free(p)

enum {
  NONE,
  BLEND,
  ADD,
  MOD,
} BLEND_MODE;


typedef struct {
  Texture2D tex;
  Image mask;
  int width, height;
  bool useMask;
} image_t;

typedef struct {
  Font font;
  int isDefault;
  int size;
} font_t;

typedef struct {
  lua_Number x, y;
  lua_Number width, height;
  Rectangle clip;
} quad_l;


int VGA_HEIGHT;
int VGA_WIDTH;


#endif
