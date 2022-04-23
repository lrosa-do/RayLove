

 #include "luaobj.h"


int l_audio_setVolume(lua_State *L)
{
  double n = luaL_checknumber(L, 1);

  return 0;
}


int l_source_new(lua_State *L);

int luaopen_audio(lua_State *L) {
  luaL_Reg reg[] = {
    { "newSource",  l_source_new        },
    { "setVolume",  l_audio_setVolume   },
    { 0, 0 },
  };
  luaL_newlib(L, reg);
  return 1;
}

