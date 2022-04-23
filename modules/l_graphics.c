

#include <string.h>
#include <stdlib.h>

#include "dmt.h"
#include "miniray.h"
#include "lovesdl.h"
#include "luaobj.h"

extern SDL_Window* gWindow ;


font_t   *graphics_defaultFont;
font_t   *graphics_font;
Color   graphics_backgroundColor;
int       graphics_backgroundColor_rgb[4];
Color   graphics_color;
int       graphics_color_rgb[4];
int       graphics_blendMode;
int       graphics_alpha;
int currentBlend;



    void DrawSolidPolygon(const Vector2* vertices, int vertexCount, const Color color)
    {

     rlBegin(RL_TRIANGLES);

                      Vector2 p3 = vertices[0];
                      for (int i = 1; i < vertexCount-1; i++)
                      {

                        Vector2 p1 = vertices[i+1];
                        Vector2 p2 = vertices[i];
                        Vector2 p3 = vertices[0];
                        rlColor4ub(color.r, color.g, color.b, color.a);
                        rlVertex2f(p1.x,p1.y);
                        rlVertex2f(p2.x,p2.y);
                        rlVertex2f(p3.x,p3.y);

                      }
        rlEnd();


    }
    void DrawLinesPolygon(const Vector2* vertices, int vertexCount, const Color color)
    {

     rlBegin(RL_LINES);

                      int i;
                      for ( i = 0; i < vertexCount; i++)
                      {

                        Vector2 p1 = vertices[i];
                        Vector2 p2 = vertices[i+1];

                        rlColor4ub(color.r, color.g, color.b, color.a);
                        rlVertex2f(p1.x,p1.y);
                        rlVertex2f(p2.x,p2.y);


                      }
                        Vector2 p1 = vertices[i];
                        Vector2 p2 = vertices[0];

                        rlColor4ub(color.r, color.g, color.b, color.a);
                        rlVertex2f(p1.x,p1.y);
                        rlVertex2f(p2.x,p2.y);
        rlEnd();


    }

      void DrawSolidPoints( const Sint16 * vx,const Sint16 * vy, int vertexCount, const Color color)
    {

     rlBegin(RL_TRIANGLES);

                      Vector2 p3 = (Vector2){vx[0],vy[0]};
                      for (int i = 1; i < (vertexCount/2); i++)
                      {

                        Vector2 p1 = (Vector2){vx[i+1],vy[i+1]};
                        Vector2 p2 = (Vector2){vx[i],vy[i]};
                        Vector2 p3 = (Vector2){vx[0],vy[0]};
                        rlColor4ub(color.r, color.g, color.b, color.a);
                        rlVertex2f(p1.x,p1.y);
                        rlVertex2f(p2.x,p2.y);
                        rlVertex2f(p3.x,p3.y);

                      }
        rlEnd();


    }
    void DrawLinesPoints( const Sint16 * vx,const Sint16 * vy, int vertexCount, const Color color)
    {

     rlBegin(RL_LINES);


     int i =0;
                      for ( i = 0; i < (vertexCount)/2; i++)
                      {

                        Vector2 p1 = (Vector2){vx[i],vy[i]};
                        Vector2 p2 = (Vector2){vx[i+1],vy[i+1]};
                        rlColor4ub(color.r, color.g, color.b, color.a);
                        rlVertex2f(p1.x,p1.y);
                        rlVertex2f(p2.x,p2.y);
                      }
                        Vector2 p1 = (Vector2){vx[i],vy[i]};
                        Vector2 p2 = (Vector2){vx[0],vy[0]};

                        rlColor4ub(color.r, color.g, color.b, color.a);
                        rlVertex2f(p1.x,p1.y);
                        rlVertex2f(p2.x,p2.y);

        rlEnd();


    }

int pixel(float x, float y)
{
rlVertex2f(x,y);
rlVertex2f(x+1,y+1);
return 1;
}

int DrawArc(Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Color c)
{
	int result;
	Sint16 cx = 0;
	Sint16 cy = rad;
	Sint16 df = 1 - rad;
	Sint16 d_e = 3;
	Sint16 d_se = -2 * rad + 5;
	Sint16 xpcx, xmcx, xpcy, xmcy;
	Sint16 ypcy, ymcy, ypcx, ymcx;
	Uint8 drawoct;
	int startoct, endoct, oct, stopval_start = 0, stopval_end = 0;
	double dstart, dend, temp = 0.;

	/*
	* Sanity check radius
	*/
	if (rad < 0) {
		return (-1);
	}

	/*
	* Special case for rad=0 - draw a point
	*/
	if (rad == 0)
	{
		DrawPixel( x, y, c);
		return 1;
	}

	/*
	 Octant labeling

	  \ 5 | 6 /
	   \  |  /
	  4 \ | / 7
	     \|/
	------+------ +x
	     /|\
	  3 / | \ 0
	   /  |  \
	  / 2 | 1 \
	      +y

	 Initially reset bitmask to 0x00000000
	 the set whether or not to keep drawing a given octant.
	 For example: 0x00111100 means we're drawing in octants 2-5
	*/
	drawoct = 0;

	/*
	* Fixup angles
	*/
	start %= 360;
	end %= 360;
	/* 0 <= start & end < 360; note that sometimes start > end - if so, arc goes back through 0. */
	while (start < 0) start += 360;
	while (end < 0) end += 360;
	start %= 360;
	end %= 360;

	/* now, we find which octants we're drawing in. */
	startoct = start / 45;
	endoct = end / 45;
	oct = startoct - 1;

    rlBegin(RL_LINES);

	/* stopval_start, stopval_end; what values of cx to stop at. */
	do {
		oct = (oct + 1) % 8;

		if (oct == startoct) {
			/* need to compute stopval_start for this octant.  Look at picture above if this is unclear */
			dstart = (double)start;
			switch (oct)
			{
			case 0:
			case 3:
				temp = sin(dstart * M_PI / 180.);
				break;
			case 1:
			case 6:
				temp = cos(dstart * M_PI / 180.);
				break;
			case 2:
			case 5:
				temp = -cos(dstart * M_PI / 180.);
				break;
			case 4:
			case 7:
				temp = -sin(dstart * M_PI / 180.);
				break;
			}
			temp *= rad;
			stopval_start = (int)temp;

			/*
			This isn't arbitrary, but requires graph paper to explain well.
			The basic idea is that we're always changing drawoct after we draw, so we
			stop immediately after we render the last sensible pixel at x = ((int)temp).
			and whether to draw in this octant initially
			*/
			if (oct % 2) drawoct |= (1 << oct);			/* this is basically like saying drawoct[oct] = true, if drawoct were a bool array */
			else		 drawoct &= 255 - (1 << oct);	/* this is basically like saying drawoct[oct] = false */
		}
		if (oct == endoct) {
			/* need to compute stopval_end for this octant */
			dend = (double)end;
			switch (oct)
			{
			case 0:
			case 3:
				temp = sin(dend * M_PI / 180);
				break;
			case 1:
			case 6:
				temp = cos(dend * M_PI / 180);
				break;
			case 2:
			case 5:
				temp = -cos(dend * M_PI / 180);
				break;
			case 4:
			case 7:
				temp = -sin(dend * M_PI / 180);
				break;
			}
			temp *= rad;
			stopval_end = (int)temp;

			/* and whether to draw in this octant initially */
			if (startoct == endoct)	{
				/* note:      we start drawing, stop, then start again in this case */
				/* otherwise: we only draw in this octant, so initialize it to false, it will get set back to true */
				if (start > end) {
					/* unfortunately, if we're in the same octant and need to draw over the whole circle, */
					/* we need to set the rest to true, because the while loop will end at the bottom. */
					drawoct = 255;
				} else {
					drawoct &= 255 - (1 << oct);
				}
			}
			else if (oct % 2) drawoct &= 255 - (1 << oct);
			else			  drawoct |= (1 << oct);
		} else if (oct != startoct) { /* already verified that it's != endoct */
			drawoct |= (1 << oct); /* draw this entire segment */
		}
	} while (oct != endoct);

	/* so now we have what octants to draw and when to draw them. all that's left is the actual raster code. */

	/*
	* Set color
	*/
	result = 0;


	/*
	* Draw arc
	*/
	do {
		ypcy = y + cy;
		ymcy = y - cy;
		if (cx > 0) {
			xpcx = x + cx;
			xmcx = x - cx;

			/* always check if we're drawing a certain octant before adding a pixel to that octant. */
			if (drawoct & 4)  result |= pixel( xmcx, ypcy);
			if (drawoct & 2)  result |= pixel( xpcx, ypcy);
			if (drawoct & 32) result |= pixel( xmcx, ymcy);
			if (drawoct & 64) result |= pixel( xpcx, ymcy);
		} else {
			if (drawoct & 96) result |= pixel( x, ymcy);
			if (drawoct & 6)  result |= pixel( x, ypcy);
		}

		xpcy = x + cy;
		xmcy = x - cy;
		if (cx > 0 && cx != cy) {
			ypcx = y + cx;
			ymcx = y - cx;
			if (drawoct & 8)   result |= pixel( xmcy, ypcx);
			if (drawoct & 1)   result |= pixel( xpcy, ypcx);
			if (drawoct & 16)  result |= pixel( xmcy, ymcx);
			if (drawoct & 128) result |= pixel( xpcy, ymcx);
		} else if (cx == 0) {
			if (drawoct & 24)  result |= pixel( xmcy, y);
			if (drawoct & 129) result |= pixel( xpcy, y);
		}

		/*
		* Update whether we're drawing an octant
		*/
		if (stopval_start == cx) {
			/* works like an on-off switch. */
			/* This is just in case start & end are in the same octant. */
			if (drawoct & (1 << startoct)) drawoct &= 255 - (1 << startoct);
			else						   drawoct |= (1 << startoct);
		}
		if (stopval_end == cx) {
			if (drawoct & (1 << endoct)) drawoct &= 255 - (1 << endoct);
			else						 drawoct |= (1 << endoct);
		}

		/*
		* Update pixels
		*/
		if (df < 0) {
			df += d_e;
			d_e += 2;
			d_se += 2;
		} else {
			df += d_se;
			d_e += 2;
			d_se += 4;
			cy--;
		}
		cx++;
	} while (cx <= cy);
    rlEnd();
	return (result);
}
int DrawPie( Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end,  Uint8 filled,Color c)
{
	int result;
	double angle, start_angle, end_angle;
	double deltaAngle;
	double dr;
	int numpoints, i;
    Vector2* points;

	/*
	* Sanity check radii
	*/
	if (rad < 0) {
		return (-1);
	}

	/*
	* Fixup angles
	*/
	start = start % 360;
	end = end % 360;

	/*
	* Special case for rad=0 - draw a point
	*/
	if (rad == 0)
	{
		DrawPixel( x, y,c);
		return 1;
	}

	/*
	* Variable setup
	*/
	dr = (double) rad;
	deltaAngle = 3.0 / dr;
	start_angle = (double) start *(2.0 * M_PI / 360.0);
	end_angle = (double) end *(2.0 * M_PI / 360.0);
	if (start > end) {
		end_angle += (2.0 * M_PI);
	}

	/* We will always have at least 2 points */
	numpoints = 2;

	/* Count points (rather than calculating it) */
	angle = start_angle;
	while (angle < end_angle) {
		angle += deltaAngle;
		numpoints++;
	}

	/* Allocate combined vertex array */
	points = (Vector2 *) DMTMALLOC(sizeof(Vector2) * numpoints);
	if (points == NULL)
	{
		return (-1);
	}

	/* Update point to start of vy */
	//vy += numpoints;

	/* Center */
	points[0].x = x;
	points[0].y = y;

	/* First vertex */
	angle = start_angle;
	points[1].x = x + (int) (dr * cos(angle));
	points[1].y = y + (int) (dr * sin(angle));

	if (numpoints<3)
	{
		DrawLine(points[0].x,points[0].y,points[1].x,points[1].y,c);

	}
	else
	{
		/* Calculate other vertices */
		i = 2;
		angle = start_angle;
		while (angle < end_angle) {
			angle += deltaAngle;
			if (angle>end_angle)
			{
				angle = end_angle;
			}
			points[i].x = x + (int) (dr * cos(angle));
			points[i].y = y + (int) (dr * sin(angle));
			i++;
		}

		/* Draw */
		if (filled)
		{
			DrawSolidPolygon(points, numpoints,c);
		} else
		{
		   DrawLinesPolygon(points, numpoints,c);
		}

	}

	/* Free combined vertex array */
	DMTFREE(points);

	return (result);
}



int DrawPiePoints( Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end,  Uint8 filled,Color c)
{
	int result;
	double angle, start_angle, end_angle;
	double deltaAngle;
	double dr;
	int numpoints, i;
    Sint16 *vx, *vy;

	/*
	* Sanity check radii
	*/
	if (rad < 0)
	{
		return (-1);
	}

	/*
	* Fixup angles
	*/
	start = start % 360;
	end = end % 360;

	/*
	* Special case for rad=0 - draw a point
	*/
	if (rad == 0)
	{
		DrawPixel( x, y,c);
		return 1;
	}

	/*
	* Variable setup
	*/
	dr = (double) rad;
	deltaAngle = 3.0 / dr;
	start_angle = (double) start *(2.0 * M_PI / 360.0);
	end_angle = (double) end *(2.0 * M_PI / 360.0);
	if (start > end) {
		end_angle += (2.0 * M_PI);
	}

	/* We will always have at least 2 points */
	numpoints = 2;

	/* Count points (rather than calculating it) */
	angle = start_angle;
	while (angle < end_angle) {
		angle += deltaAngle;
		numpoints++;
	}

	/* Allocate combined vertex array */
    vx = vy = (Sint16 *) DMTMALLOC(2 * sizeof(Uint16) * numpoints);
	if (vx == NULL)
	{
		return (-1);
	}

/* Update point to start of vy */
	vy += numpoints;

	/* Center */
	vx[0] = x;
	vy[0] = y;

	/* First vertex */
	angle = start_angle;
	vx[1] = x + (int) (dr * cos(angle));
	vy[1] = y + (int) (dr * sin(angle));

	if (numpoints<3)
	{		DrawLine(vx[0],vx[0],vx[1],vx[1],c);

   }
	else
	{
		/* Calculate other vertices */
		i = 2;
		angle = start_angle;
		while (angle < end_angle) {
			angle += deltaAngle;
			if (angle>end_angle)
			{
				angle = end_angle;
			}
			vx[i] = x + (int) (dr * cos(angle));
			vy[i] = y + (int) (dr * sin(angle));



			i++;
		}


		/* Draw */
		if (filled)
		{
			DrawSolidPoints(vx,vy, numpoints,c);
		} else
		{
		   DrawLinesPoints(vx,vy, numpoints,c);
		}
	}

	/* Free combined vertex array */
	DMTFREE(vx);


	return (result);
}
static Color getColorFromArgs(lua_State *L, int *rgb, const int *def)
{
  int r, g, b,a;
  if ( lua_isnoneornil(L, 1) )
  {
    r = def[0];
    g = def[1];
    b = def[2];
    a = 255;
  } else
  {
    r = luaL_checknumber(L, 1);
    g = luaL_checknumber(L, 2);
    b = luaL_checknumber(L, 3);
    a = 255;
  }

  if (rgb)
  {
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
    rgb[3] = 255;
  }
  return (Color){r,g,b,a};
}


static int pushColor(lua_State *L, int *rgb)
{
  lua_pushinteger(L, rgb[0]);
  lua_pushinteger(L, rgb[1]);
  lua_pushinteger(L, rgb[2]);
  return 3;
}


int l_graphics_getDimensions(lua_State *L)
 {
  int w,h;
  SDL_GetWindowSize(gWindow,&w,&h);
  lua_pushinteger(L, w);
  lua_pushinteger(L, h);
  return 2;
}


int l_graphics_getWidth(lua_State *L) {
int w,h;
  SDL_GetWindowSize(gWindow,&w,&h);

 lua_pushinteger(L, w);
  return 1;
}


int l_graphics_getHeight(lua_State *L) {
int w,h;
  SDL_GetWindowSize(gWindow,&w,&h);

 lua_pushinteger(L, h);

  return 1;
}


int l_graphics_getBackgroundColor(lua_State *L) {
  return pushColor(L, graphics_backgroundColor_rgb);
}


int l_graphics_setBackgroundColor(lua_State *L)
{
  static const int def[] = { 0, 0, 0 };
  graphics_backgroundColor_rgb[0]=RAYWHITE.r;
  graphics_backgroundColor_rgb[1]=RAYWHITE.g;
  graphics_backgroundColor_rgb[2]=RAYWHITE.b;
  graphics_backgroundColor_rgb[3]=RAYWHITE.r;
  Color idx = getColorFromArgs(L, graphics_backgroundColor_rgb, def);
  graphics_backgroundColor = idx;
  return 0;
}


int l_graphics_getColor(lua_State *L)
{
  return pushColor(L, graphics_color_rgb);
}


int l_graphics_setColor(lua_State *L)
{
  static const int def[] = { 0xff, 0xff, 0xff };
   graphics_color = getColorFromArgs(L, graphics_color_rgb, def);
   return 0;
}

int  l_graphics_setAlpha(lua_State *L)
{
  graphics_alpha = luaL_checknumber(L, 1);
  return 0;
}




int l_graphics_getBlendMode(lua_State *L)
{
  switch (graphics_blendMode)
  {
    default:
    case NONE : lua_pushstring(L, "none");  break;
    case BLEND   : lua_pushstring(L, "blend");    break;
    case ADD    : lua_pushstring(L, "add");     break;
    case MOD     : lua_pushstring(L, "mod");      break;

  }
  return 1;
}

int l_graphics_setBlendModeReset(lua_State *L)
{
  rlEnableColorBlend();
  rlsetBlendsMode(GBLEND_ALPHA,BLENDALPHA_MULTIPLY);
}

int l_graphics_setBlendMode(lua_State *L)
{
  //const char *str = lua_isnoneornil(L, 1);
  const char *str = luaL_checkstring(L, 1);

  int enable =1;

  if (str)
  {


  if (!strcmp(str,"alpha"))
  {
   currentBlend = GBLEND_ALPHA;
  } else
  if (!strcmp(str,"multiply"))
  {
  currentBlend = GBLEND_MULTIPLY;
  } else
  if (!strcmp(str,"subtract"))
  {
  currentBlend = GBLEND_SUBTRACT;
  } else
  if (!strcmp(str,"add"))
  {
  currentBlend = GBLEND_ADD;
  } else
  if (!strcmp(str,"lighten"))
  {
  currentBlend = GBLEND_LIGHTEN;
  } else
  if (!strcmp(str,"darken"))
  {
  currentBlend = GBLEND_DARKEN;
  } else
  if (!strcmp(str,"screen"))
  {
  currentBlend = GBLEND_SCREEN;
  } else
  if (!strcmp(str,"replace"))
  {
  currentBlend = GBLEND_REPLACE;
  }else
  if (!strcmp(str,"none"))
  {
  enable=0;
  currentBlend = GBLEND_NONE;
  } else
  {
   luaL_argerror(L, 1, "Bad blend mode");
   }

}

 if (enable)
 {
  rlEnableColorBlend();
  //rlSetBlendMode (currentBlend);
  rlsetBlendsMode(currentBlend,BLENDALPHA_MULTIPLY);
 } else
 rlDisableColorBlend();

  return 0;
}


int l_graphics_getFont(lua_State *L) {
  lua_pushlightuserdata(L, graphics_font);
  lua_gettable(L, LUA_REGISTRYINDEX);
  return 1;
}


int l_graphics_setFont(lua_State *L)
 {
font_t *oldFont = graphics_font;
  if (lua_isnoneornil(L, 1)) {
    /* If no arguments are given we use the default embedded font, grab it
     * from the registry and set it as the first argument */
    lua_pushlightuserdata(L, &graphics_defaultFont);
    lua_gettable(L, LUA_REGISTRYINDEX);
//    lua_insert(L, 1);
  }
  graphics_font = luaobj_checkudata(L, 1, LUAOBJ_TYPE_FONT);
  /* Remove old font from registry. This is done after we know the args are
   * okay so that the font remains unchanged if an error occurs */
  if (oldFont) {
    lua_pushlightuserdata(L, oldFont);
    lua_pushnil(L);
    lua_settable(L, LUA_REGISTRYINDEX);
  }
  /* Add new font to registry */
  lua_pushlightuserdata(L, graphics_font);
  lua_pushvalue(L, 1);
  lua_settable(L, LUA_REGISTRYINDEX);
  return 0;
}


int l_graphics_getCanvas(lua_State *L) {
  //lua_pushlightuserdata(L, graphics_canvas);
//  lua_gettable(L, LUA_REGISTRYINDEX);
  return 1;
}


int l_graphics_setCanvas(lua_State *L)
{

  return 0;
}


int l_graphics_reset(lua_State *L)
 {
  int (*funcs[])(lua_State*) = {
    l_graphics_setBackgroundColor,
    l_graphics_setColor,
    l_graphics_setBlendModeReset,
    l_graphics_setFont,
    l_graphics_setCanvas,
    NULL,
  };
  int i;
  for (i = 0; funcs[i]; i++) {
    lua_pushcfunction(L, funcs[i]);
    lua_call(L, 0, 0);
  }
  return 0;
}


int l_graphics_clear(lua_State *L)
{

Color idx = getColorFromArgs(L, NULL, graphics_backgroundColor_rgb);


 ClearBackground(idx);
 BeginDrawing();

 return 0;
}



int l_graphics_present(lua_State *L)
{





  EndDrawing();
  SDL_GL_SwapWindow(gWindow);
  return 0;
}


int l_graphics_draw(lua_State *L)
{
	const int top = lua_gettop(L);



  image_t *img = luaobj_checkudata(L, 1, LUAOBJ_TYPE_IMAGE);
  quad_l *quad = NULL;
  int x, y, flip;
  double angle = 0;
  double sx = 1;
  double sy = 1;
  int w;
  int h;
  Vector2 center ={0,0};





  if (!lua_isnone(L, 2) && lua_type(L, 2) != LUA_TNUMBER)
  {
    quad = luaobj_checkudata(L, 2, LUAOBJ_TYPE_QUAD);
    x = luaL_optnumber(L, 3, 0);
    y = luaL_optnumber(L, 4, 0);
    flip = !lua_isnone(L, 5) && lua_toboolean(L, 5);

    if (!lua_isnone(L, 6) && luaL_optnumber(L, 6,0))
    {
      angle = luaL_optnumber(L, 6,0);
    }

     if (!lua_isnone(L, 7) && luaL_optnumber(L, 7,0) && !lua_isnone(L, 8) && luaL_optnumber(L, 8,0))
    {
      sx = luaL_optnumber(L, 7,0);
      sy = luaL_optnumber(L, 8,0);
    }


  } else
  {
    x = luaL_optnumber(L, 2, 0);
    y = luaL_optnumber(L, 3, 0);
    flip = !lua_isnone(L, 4) && lua_toboolean(L, 4);

    if (!lua_isnone(L, 5) && luaL_optnumber(L, 5,0))
    {
      angle = luaL_optnumber(L, 5,0);
    }

         if (!lua_isnone(L, 6) && luaL_optnumber(L, 6,0) && !lua_isnone(L, 7) && luaL_optnumber(L, 7,0))
        {
          sx = luaL_optnumber(L, 6,0);
          sy = luaL_optnumber(L, 7,0);
        }



  }



  if (quad)
  {

      if (img)
    {







           w = quad->clip.width  * sx;
           h = quad->clip.height * sy;

               Rectangle src =(Rectangle) { quad->x,quad->y , quad->width,quad->height };

              if (flip)
              {
                src.width = - quad->width;
              }

           Rectangle dst =(Rectangle) { x, y, w,h };


          center.x = w/2;
          center.y = h/2;


             if (!lua_isnone(L, 9) && luaL_optnumber(L, 9,0) && !lua_isnone(L, 10) && luaL_optnumber(L, 10,0))
            {
              center.x = luaL_optnumber(L, 9 ,0);
              center.x = luaL_optnumber(L, 10,0);
            }

	   DrawTexturePro(img->tex,src,dst,center,angle,graphics_color);


    }

  } else
  {
    if (img)
    {

           w = img->width  ;//* sx;
           h = img->height ;//* sy;

          Rectangle dst =(Rectangle) { x, y, img->width*sx,img->height*sy };


          Rectangle src =(Rectangle) { 0, 0, w,h };



              if (flip)
              {
                src.width = w;
              }




          center.x = dst.width/2;
          center.y = dst.height/2;


         if (!lua_isnone(L, 8) && luaL_optnumber(L, 8,0) && !lua_isnone(L, 9) && luaL_optnumber(L, 9,0))
        {
          center.x = luaL_optnumber(L, 8,0);
          center.x = luaL_optnumber(L, 9,0);
        }


        //DrawTexture(img->tex,x,y,graphics_color);
        DrawTexturePro(img->tex,src,dst,center,angle,graphics_color);


	//SDL_RenderCopyEx( gRenderer, img->tex, NULL, &renderQuad, angle, &center, flipType );



    }
  }
  return 0;
}


int l_graphics_point(lua_State *L) {
  int x = luaL_checknumber(L, 1);
  int y = luaL_checknumber(L, 2);
  DrawPixel(x,y,graphics_color);
  return 0;
}


int l_graphics_line(lua_State *L) {
  int argc = lua_gettop(L);
  int x = luaL_checknumber(L, 1);
  int y = luaL_checknumber(L, 2);
  int x2 = luaL_checknumber(L, 3) ;
  int y2 = luaL_checknumber(L, 4) ;
  DrawLine(x,y,x2,y2,graphics_color);
  return 0;
}


int l_graphics_rectangle(lua_State *L)
{
  const char *mode = luaL_checkstring(L, 1);
  int x = luaL_checknumber(L, 2);
  int y = luaL_checknumber(L, 3);
  int x2 = luaL_checknumber(L, 4) ;
  int y2 = luaL_checknumber(L, 5) ;

  int fill = 0;
  if (!strcmp(mode, "fill"))
  {
    fill = 1;
  } else if (!strcmp(mode, "line"))
  {
    fill = 0;
  } else
  {
    luaL_error(L, "bad mode");
  }


  if (fill==1)
     DrawRectangle(x,y,x2,y2,graphics_color);
     else
     DrawRectangleLines(x,y,x2,y2,graphics_color);

  return 0;
}

int l_graphics_arc(lua_State *L)
{

  int x = luaL_checknumber(L, 1);
  int y = luaL_checknumber(L, 2);
  int radius = luaL_checknumber(L, 3);
  int start = luaL_checknumber(L, 4);
  int end = luaL_checknumber(L, 5);

  DrawArc(x,y,radius,start,end,graphics_color);

 return 0;
}

int l_graphics_pie(lua_State *L)
{
  const char *mode = luaL_checkstring(L, 1);
  int x = luaL_checknumber(L, 2);
  int y = luaL_checknumber(L, 3);
  int radius = luaL_checknumber(L, 4);
  int start = luaL_checknumber(L, 5);
  int end = luaL_checknumber(L, 6);

  int fill = 0;
  if (!strcmp(mode, "fill")) {
    fill = 1;
  } else if (!strcmp(mode, "line")) {
    fill = 0;
  } else {
    luaL_error(L, "bad mode");
  }



  DrawPiePoints(x,y,radius,start,end,fill,graphics_color);


  return 0;
}


int l_graphics_box(lua_State *L)
{

  int x1 = luaL_checknumber(L, 1);
  int y1 = luaL_checknumber(L, 2);
  int x2 = luaL_checknumber(L, 3);
  int y2 = luaL_checknumber(L, 4);


  return 0;
}

int l_graphics_rounded(lua_State *L)
{
  const char *mode = luaL_checkstring(L, 1);
  int x1 = luaL_checknumber(L, 2);
  int y1 = luaL_checknumber(L, 3);
  int x2 = luaL_checknumber(L, 4);
  int y2 = luaL_checknumber(L, 5);
  float rad =(float) luaL_checknumber(L, 6);
  int segments = luaL_checknumber(L, 7);


  int fill = 0;
  if (!strcmp(mode, "fill")) {
    fill = 1;
  } else if (!strcmp(mode, "line")) {
    fill = 0;
  } else {
    luaL_error(L, "bad mode");
  }

  if (fill)
     DrawRectangleRounded((Rectangle){x1,y1,x2,y2},rad,segments,graphics_color);
 else
     DrawRectangleRoundedLines((Rectangle){x1,y1,x2,y2},rad,segments,1.0f,graphics_color);

  return 0;
}



int l_graphics_ellipse(lua_State *L) {
  const char *mode = luaL_checkstring(L, 1);
  int x = luaL_checknumber(L, 2);
  int y = luaL_checknumber(L, 3);
  double rx = luaL_checknumber(L, 4);
  double ry = luaL_checknumber(L, 5);

  int fill = 0;
  if (!strcmp(mode, "fill")) {
    fill = 1;
  } else if (!strcmp(mode, "line")) {
    fill = 0;
  } else {
    luaL_error(L, "bad mode");
  }

  if (fill)
    DrawEllipse(x,y,rx,ry,graphics_color);
  else
    DrawEllipseLines(x,y,rx,ry,graphics_color);

  return 0;
}



int l_graphics_circle(lua_State *L) {
  const char *mode = luaL_checkstring(L, 1);
  int x = luaL_checknumber(L, 2);
  int y = luaL_checknumber(L, 3);
  double radius = luaL_checknumber(L, 4);
  int fill = 0;
  if (!strcmp(mode, "fill")) {
    fill = 1;
  } else if (!strcmp(mode, "line")) {
    fill = 0;
  } else {
    luaL_error(L, "bad mode");
  }



  if (fill)
    DrawCircle(x,y,radius,graphics_color);
  else
    DrawCircleLines(x,y,radius,graphics_color);


  return 0;
}


int l_graphics_polygon(lua_State *L)
{

    int args = lua_gettop(L) -1;
    const char *mode = luaL_checkstring(L, 1);
	bool is_table = false;
	if (lua_istable(L, 2))
	{
		args = (int) luax_objlen(L, 2);
		is_table = true;
	}





	if (args % 2 != 0)
		return luaL_error(L, "Number of points components must be a multiple of two");
	else if (args < 6)
		return luaL_error(L, "Need at least 3 vertices to draw a line");

  int fill = 0;
  if (!strcmp(mode, "fill")) {
    fill = 1;
  } else if (!strcmp(mode, "line")) {
    fill = 0;
  } else {
    luaL_error(L, "bad mode");
  }



	int numvertices = args / 2;
//	SDL_Log("%d %d %d",args,is_table,numvertices);

	Vector2* coords =(Vector2 *) dmt_malloc((numvertices + 1)*sizeof(Vector2));


	if (is_table)
	{
		for (int i = 0; i < numvertices; ++i)
		{
			lua_rawgeti(L, 2, (i * 2) + 1);
			lua_rawgeti(L, 2, (i * 2) + 2);
			coords[i].x = (float)luaL_checknumber(L, -2);
			coords[i].y = (float)luaL_checknumber(L, -1);
			lua_pop(L, 2);
		}
	}
	else
	{
		for (int i = 0; i < numvertices; ++i)
		{
			coords[i].x = (float)luaL_checknumber(L, (i * 2) + 2);
			coords[i].y = (float)luaL_checknumber(L, (i * 2) + 3);
		}
	}

  coords[numvertices].x = coords[0].x;
  coords[numvertices].y = coords[0].y;


  if (fill)
  {
   DrawSolidPolygon(coords,numvertices,graphics_color);
  }
 else
{
DrawLinesPolygon(coords,numvertices,graphics_color);
}


dmt_free(coords);





  return 0;
}

int l_graphics_bezier(lua_State *L)
{

    int args = lua_gettop(L) -1;
    int steps = (int)luaL_checknumber(L, 1);
	bool is_table = false;
	if (lua_istable(L, 2))
	{
		args = (int) luax_objlen(L, 2);
		is_table = true;
	}





	if (args % 2 != 0)
		return luaL_error(L, "Number of points components must be a multiple of two");
	else if (args < 6)
		return luaL_error(L, "Need at least four vertices to draw a line");


	int numvertices = args / 2;
//	SDL_Log("%d %d %d",args,is_table,numvertices);

	double* x_coords =(double *) dmt_malloc((numvertices + 1)*sizeof(double));
	double* y_coords =(double *) dmt_malloc((numvertices + 1)*sizeof(double));

	if (is_table)
	{
		for (int i = 0; i < numvertices; ++i)
		{
			lua_rawgeti(L, 2, (i * 2) + 1);
			lua_rawgeti(L, 2, (i * 2) + 2);
			x_coords[i] = (int)luaL_checknumber(L, -2);
			y_coords[i] = (int)luaL_checknumber(L, -1);
		//	DrawFilledCircle(x_coords[i],y_coords[i],2,RED);
			lua_pop(L, 2);
		}
	}
	else
	{
		for (int i = 0; i < numvertices; ++i)
		{
			x_coords[i] = (int)luaL_checknumber(L, (i * 2) + 2);
			y_coords[i] = (int)luaL_checknumber(L, (i * 2) + 3);
		}
	}

   x_coords[numvertices] = x_coords[0];
   y_coords[numvertices] = y_coords[0];

   DrawBezier(x_coords,y_coords,numvertices,steps,graphics_color);

	dmt_free(x_coords);
	dmt_free(y_coords);


  return 0;
}




void PrintText(Font f,const char *text, int posX, int posY, int fontSize, Color color)
{
    // Check if default font has been loaded
    if (f.texture.id != 0)
    {
        Vector2 position = { (float)posX, (float)posY };

        int defaultFontSize = 10;   // Default Font chars height in pixel
        if (fontSize < defaultFontSize) fontSize = defaultFontSize;
        int spacing = fontSize/defaultFontSize;

        DrawTextEx(f, text, position, (float)fontSize, (float)spacing, color);
    }
}

int l_graphics_print(lua_State *L) {
  luaL_checkany(L, 3);
  int nargs = lua_gettop(L);
  const char *str = luaL_tolstring(L, 1, NULL);
  int x = luaL_checknumber(L, 2);
  int y = luaL_checknumber(L, 3);
  if (graphics_font)
  {
   int size = graphics_font->size;

	if (nargs == 4)
	{
		 size = luaL_checknumber(L, 4);
		return 0;
	}
  PrintText(graphics_font->font,str,x,y,graphics_font->size,graphics_color);
  }

  return 0;
}

extern double  timer_avgLastDt;

int l_graphics_printFPS(lua_State *L)
{
  luaL_checkany(L, 1);
  int x = luaL_checknumber(L, 1);
  int y = luaL_checknumber(L, 2);
  if (graphics_font)
  {

    Color color = WHITE; // good fps
    int fps = (int)(1. / timer_avgLastDt);

    if (fps < 30 && fps >= 15) color = ORANGE;  // warning FPS
    else if (fps < 15) color = RED;    // bad FPS

    PrintText(graphics_font->font,TextFormat("%2i FPS", fps), x, y, 10, color);
  }

  return 0;
}



//********************************************************************************************************************************************************
//transform

int l_graphics_push(lua_State *L)
{


  rlPushMatrix();

  return 0;
}
int l_graphics_pop(lua_State *L)
{
    rlPopMatrix();

  return 0;
}

int l_graphics_translate(lua_State *L)
{
  luaL_checkany(L, 1);
  int x = luaL_checknumber(L, 1);
  int y = luaL_checknumber(L, 2);
  rlTranslatef(x,y, 0.0f);

  return 0;
}

int l_graphics_scale(lua_State *L)
{
  luaL_checkany(L, 1);
  int x = luaL_checknumber(L, 1);
  int y = luaL_checknumber(L, 2);
  rlScalef(x,y,1.0f);

  return 0;
}

int l_graphics_rotate(lua_State *L)
{
  luaL_checkany(L, 1);
  int x = luaL_checknumber(L, 1);
rlRotatef(x, 0.0f, 0.0f, 1.0f);

  return 0;
}
int l_graphics_origin(lua_State *L)
{
  rlLoadIdentity();
  return 0;
}





int l_image_new(lua_State *L);
int l_image_newCanvas(lua_State *L);
int l_quad_new(lua_State *L);
int l_font_new(lua_State *L);
int l_font_default(lua_State *L);

int luaopen_graphics(lua_State *L) {
  luaL_Reg reg[] = {
    { "getDimensions",      l_graphics_getDimensions      },
    { "getWidth",           l_graphics_getWidth           },
    { "getHeight",          l_graphics_getHeight          },
    { "getBackgroundColor", l_graphics_getBackgroundColor },
    { "setBackgroundColor", l_graphics_setBackgroundColor },
    { "push",               l_graphics_push},
    { "pop",               l_graphics_pop},
    { "translate",               l_graphics_translate},
    { "scale",               l_graphics_scale},
    { "rotate",               l_graphics_rotate},
    { "origin",               l_graphics_origin},
    { "getColor",           l_graphics_getColor           },
    { "setColor",           l_graphics_setColor           },
    { "setAlpha",           l_graphics_setAlpha           },
    { "getBlendMode",       l_graphics_getBlendMode       },
    { "setBlendMode",       l_graphics_setBlendMode       },
    { "getFont",            l_graphics_getFont            },
    { "setFont",            l_graphics_setFont            },
    { "getCanvas",          l_graphics_getCanvas          },
    { "setCanvas",          l_graphics_setCanvas          },
    { "reset",              l_graphics_reset              },
    { "clear",              l_graphics_clear              },
    { "present",            l_graphics_present            },
    { "draw",               l_graphics_draw               },
    { "point",              l_graphics_point              },
    { "line",               l_graphics_line               },
    { "bezier",               l_graphics_bezier               },
    { "rectangle",          l_graphics_rectangle          },
    { "circle",             l_graphics_circle             },
    { "print",              l_graphics_print              },
    { "pie"  ,             l_graphics_pie                },
    { "ellipse",              l_graphics_ellipse                         },
    { "arc",              l_graphics_arc                          },
    { "box",              l_graphics_box                },
    { "rounded",                  l_graphics_rounded                },
    { "polygon",              l_graphics_polygon              },
    { "Fps",              l_graphics_printFPS              },
    { "newImage",           l_image_new                   },
    { "newCanvas",          l_image_newCanvas             },
    { "newQuad",            l_quad_new                    },
    { "newFont",            l_font_new                    },
    { 0, 0 },
  };
  luaL_newlib(L, reg);

  graphics_color= WHITE;
  graphics_backgroundColor=WHITE;
  graphics_alpha=255;
  currentBlend = SDL_BLENDMODE_BLEND;
/*
//  lua_pushcfunction(L, l_image_newCanvas);
  lua_pushinteger(L, VGA_WIDTH);
  lua_pushinteger(L, VGA_HEIGHT);
  lua_call(L, 2, 1);
//  graphics_screen = luaobj_checkudata(L, -1, LUAOBJ_TYPE_IMAGE);

 // lua_pushlightuserdata(L, &graphics_screen);
 // lua_pushvalue(L, -2);
 // lua_settable(L, LUA_REGISTRYINDEX);
 // lua_pop(L, 1);
*/



  //lua_pushcfunction(L, l_font_default);
  lua_pushcfunction(L, l_font_new);
  lua_call(L, 0, 1);
  graphics_defaultFont =luaobj_checkudata(L, -1, LUAOBJ_TYPE_FONT);
  graphics_font = graphics_defaultFont;

  lua_pushlightuserdata(L, &graphics_defaultFont);
  lua_pushvalue(L, -2);
  lua_settable(L, LUA_REGISTRYINDEX);
  lua_pop(L, 1); // Pop the Font objec

  lua_pushcfunction(L, l_graphics_reset);
  lua_call(L, 0, 0);

  return 1;
}

