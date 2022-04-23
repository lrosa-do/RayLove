

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "luaobj.h"
#include "lovesdl.h"


#define CLASS_TYPE  LUAOBJ_TYPE_QUAD
#define CLASS_NAME  "Quad"


int l_quad_new(lua_State *L)
{
  quad_l *self = luaobj_newudata(L, sizeof(*self));
  luaobj_setclass(L, CLASS_TYPE, CLASS_NAME);
  self->x = luaL_checknumber(L, 1);
  self->y = luaL_checknumber(L, 2);
  self->width = luaL_checknumber(L, 3);
  self->height = luaL_checknumber(L, 4);
  self->clip=(Rectangle){self->x,self->y,self->width,self->height};
  return 1;
}


int l_quad_setViewport(lua_State *L)
{
  quad_l *self = luaobj_checkudata(L, 1, CLASS_TYPE);
  self->x = luaL_checknumber(L, 2);
  self->y = luaL_checknumber(L, 3);
  self->width = luaL_checknumber(L, 4);
  self->height = luaL_checknumber(L, 5);
  self->clip=(Rectangle){self->x,self->y,self->width,self->height};
  return 0;
}


int l_quad_getViewport(lua_State *L)
{
  quad_l *self = luaobj_checkudata(L, 1, CLASS_TYPE);
  lua_pushnumber(L, self->x);
  lua_pushnumber(L, self->y);
  lua_pushnumber(L, self->width);
  lua_pushnumber(L, self->height);
  return 4;
}


int luaopen_quad(lua_State *L) {
  luaL_Reg reg[] = {
    { "new",          l_quad_new          },
    { "getViewport",  l_quad_getViewport  },
    { "setViewport",  l_quad_setViewport  },
    { 0, 0 },
  };
  luaobj_newclass(L, CLASS_NAME, NULL, l_quad_new, reg);
  return 1;
}

