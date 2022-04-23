#include "miniray.h"
#include "lovesdl.h"
#include "luaobj.h"


//const Uint8 *key_state;


extern SDL_Window* gWindow ;



enum {
  EVENT_NULL,
  EVENT_QUIT,
  EVENT_WINDOW_RESIZE,
  EVENT_KEYBOARD_PRESSED,
  EVENT_KEYBOARD_RELEASED,
  EVENT_KEYBOARD_TEXTINPUT,
  EVENT_MOUSE_MOVED,
  EVENT_MOUSE_PRESSED,
  EVENT_MOUSE_RELEASED,

};

typedef union {
  int type;

  struct {
    int type;
    int status;
  } quit;


  struct {
  int w;
  int h;
  } window;

  struct {
    int type;
    int x, y;
    int dx, dy;
    int button;
  } mouse;

  struct {
    int type;
     char *key;
    char text[64];
    int scancode;
    int isrepeat;
  } keyboard;


} event_t;




#define BUFFER_SIZE 256
#define BUFFER_MASK (BUFFER_SIZE - 1)

static struct {
  event_t buffer[BUFFER_SIZE];
  unsigned writei, readi;
} events;



void event_push(event_t *e) {
  events.buffer[events.writei++ & BUFFER_MASK] = *e;
}

char* toLower(const char* s)
{
  for(char *p=s; *p; p++) *p=tolower(*p);
  return s;
}
char* toUpper(const char* s) {
  for(char *p=s; *p; p++) *p=toupper(*p);
  return s;
}

int event_poll(event_t *e)
{
  if (events.readi != events.writei)
  {
    *e = events.buffer[events.readi++ & BUFFER_MASK];
    return 1;
  }
  return 0;
}

int mouse_inited;
int mouse_x, mouse_y;
int mouse_lastX, mouse_lastY;

void LuaExecuteFile(const char *filename);

int l_event_pump(lua_State *L)
{



  SDL_Event e;
  while  (SDL_PollEvent( &e ) )
  {
    ProcessInputEvents(&e);
    switch (e.type)
    {
         case  SDL_QUIT:
            {
                                 event_t me;
                                  me.type = EVENT_QUIT;
                                  me.quit.status = 1;
                                  event_push(&me);


                                     break;
         }

        case SDL_WINDOWEVENT:
                {
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                    {

                                   event_t me;
                                   me.type = EVENT_WINDOW_RESIZE;
                                   me.window.w=e.window.data1;
                                   me.window.h=e.window.data2;
                                //   SDL_Log("main resize ");
                                   event_push(&me);
                  //  break;
                    }

                break;
                }
	case SDL_DROPFILE:
        {       char* dropped_filedir;
				dropped_filedir = e.drop.file;


				if (!strcmp("lua",GetFileExtension(dropped_filedir)))
				{
				// SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Lua file  dropped on window", dropped_filedir, gWindow);
				// LuaExecuteFile(dropped_filedir);
				}

				SDL_free(dropped_filedir);
				break;
				}
			case SDL_DROPTEXT:
			{
                char* dropped_filedir;
				dropped_filedir = e.drop.file;
				//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Text dropped on window", dropped_filedir, gWindow);
				SDL_free(dropped_filedir);
				break;
            }


              case SDL_KEYDOWN:
                       {

                                  event_t me;
                                  me.type = EVENT_KEYBOARD_PRESSED;
                                  me.keyboard.key = SDL_GetScancodeName(e.key.keysym.scancode);
                                  me.keyboard.scancode =e.key.keysym.scancode;
                                  event_push(&me);

                          break;
                       }
              case SDL_KEYUP:
                       {


                                 event_t me;
                                  me.type = EVENT_KEYBOARD_RELEASED;
                                  me.keyboard.key = SDL_GetScancodeName(e.key.keysym.scancode);
                                  me.keyboard.scancode =e.key.keysym.scancode;
                                  event_push(&me);

                          break;
                       }
              case SDL_MOUSEBUTTONDOWN:
                {


                    event_t me;
                    me.type =  EVENT_MOUSE_PRESSED ;
                    me.mouse.x = e.button.x;
                    me.mouse.y = e.button.y;
                    me.mouse.button = e.button.button;
                    event_push(&me);

               break;
              }
              case SDL_MOUSEBUTTONUP:
              {


                    event_t me;
                    me.type =   EVENT_MOUSE_RELEASED;
                    me.mouse.x = e.button.x;
                    me.mouse.y = e.button.y;
                    me.mouse.button = e.button.button;
                    event_push(&me);
                    break;
              }
              case SDL_MOUSEMOTION:
                        {





                            mouse_x = e.button.x;
                            mouse_y = e.button.y;
                            event_t me;
                            me.type =   EVENT_MOUSE_MOVED;
                            me.mouse.button = e.button.button;
                            me.mouse.x = mouse_x;
                            me.mouse.y = mouse_y;
                            me.mouse.dx = mouse_x - mouse_lastX;
                            me.mouse.dy = mouse_y - mouse_lastY;
                            event_push(&me);
                            mouse_lastX = mouse_x;
                            mouse_lastY = mouse_y;

                                    break;

                        }
    }
}
}




const char* event_typestr(int type) {
  switch (type)
  {
    case EVENT_QUIT               : return "quit";
    case EVENT_WINDOW_RESIZE      : return "windowresize";
    case EVENT_KEYBOARD_PRESSED   : return "keypressed";
    case EVENT_KEYBOARD_RELEASED  : return "keyreleased";
    case EVENT_KEYBOARD_TEXTINPUT : return "textinput";
    case EVENT_MOUSE_MOVED        : return "mousemoved";
    case EVENT_MOUSE_PRESSED      : return "mousepressed";
    case EVENT_MOUSE_RELEASED     : return "mousereleased";
  }
  return "none";
}

int l_event_poll(lua_State *L)
{


  event_t e;
  if (event_poll(&e))
  {
    lua_pushstring(L, event_typestr(e.type));


    switch(e.type)
    {
      case EVENT_QUIT:
        lua_pushnumber(L, 1);
        return 2;

       case EVENT_WINDOW_RESIZE:
        lua_pushnumber(L, e.window.w);
        lua_pushnumber(L, e.window.h);

        return 3;
      case EVENT_KEYBOARD_PRESSED:
      case EVENT_KEYBOARD_RELEASED:
        lua_pushstring(L, e.keyboard.key);
        lua_pushnumber(L, e.keyboard.scancode);
        return 3;

      case EVENT_KEYBOARD_TEXTINPUT:
        lua_pushstring(L, e.keyboard.text);
        return 2;

      case EVENT_MOUSE_MOVED:
        lua_pushnumber(L, e.mouse.x);
        lua_pushnumber(L, e.mouse.y);
        lua_pushnumber(L, e.mouse.dx);
        lua_pushnumber(L, e.mouse.dy);
        return 5;

      case EVENT_MOUSE_PRESSED:
      case EVENT_MOUSE_RELEASED:
        lua_pushnumber(L, e.mouse.x);
        lua_pushnumber(L, e.mouse.y);
        lua_pushnumber(L, e.mouse.button);
        return 4;

    }

    return 1;
  }
  return 0;
}


int l_event_quit(lua_State *L)
{
  int status = luaL_optnumber(L, 1, 0);
  event_t e;
  e.type = EVENT_QUIT;
  e.quit.status = status;
  event_push(&e);
  return 0;
}



int luaopen_event(lua_State *L) {
  luaL_Reg reg[] = {
    { "pump",  l_event_pump  },
    { "poll",  l_event_poll  },
    { "quit",  l_event_quit  },
    { 0, 0 },
  };
  luaL_newlib(L, reg);




  return 1;
}
