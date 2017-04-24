#include"dolmo_screen.hpp"
#include"dolmo_node.hpp"
#include"dolmo_font.hpp"
#include"dolmo_renderer.hpp"
#include<SDL.h>
#include<vector>
#include<cstring>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif




namespace{


SDL_Window*    window;
SDL_Surface*  surface;

int  bpp;

uint8_t*  row_table[screen::height];


struct
Button: public  Rect
{
  const char*  text;

  SceneEditor&  manager;

  Callback  callback;

  Button(int  x_, int  y_, const char*  text_, SceneEditor&  mgr, Callback  cb):
  Rect(x_,y_,16*std::strlen(text_),16),
  text(text_),
  manager(mgr),
  callback(cb)
  {
  }

};


const Button*
current_button;


uint32_t
palette[luminance_table_size];


uint32_t
red;


uint32_t
yellow;


std::vector<Button>
button_list;


bool
needed_to_redraw;


uint32_t
map_rgb(uint8_t  l)
{
  return SDL_MapRGB(surface->format,l,l,l);
}


}




namespace screen{




void
open()
{
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("dolmo - " __DATE__,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);

  surface = SDL_GetWindowSurface(window);

  bpp = surface->format->BytesPerPixel;

  red    = SDL_MapRGB(surface->format,0xFF,0x00,0x00);
  yellow = SDL_MapRGB(surface->format,0xFF,0xFF,0x00);

    for(int  i = 0;  i < luminance_table_size;   ++i)
    {
      palette[i] = map_rgb(luminance_table[i]);
    }


  auto  p = static_cast<uint8_t*>(surface->pixels);

    for(int  y = 0;  y < surface->h;  ++y)
    {
      row_table[y] = p                  ;
                     p += surface->pitch;
    }
}


void
close()
{
  SDL_DestroyWindow(window);

  SDL_Quit();
}


uint32_t
get_color(uint8_t  r, uint8_t  g, uint8_t  b)
{
  return SDL_MapRGB(surface->format,r,g,b);
}


void
lock()
{
  SDL_LockSurface(surface);
}


void
unlock()
{
  SDL_UnlockSurface(surface);
}


void
clear()
{
  SDL_memset(surface->pixels,0,surface->pitch*height);
}




void
make_button(int  x, int  y, const char*  text, SceneEditor&  mgr, Callback  cb)
{
  button_list.emplace_back(x,y,text,mgr,cb);
}


bool
touch_button(int  x, int  y, bool  press)
{
  Point  pt(x,y);

    for(auto&  btn: button_list)
    {
        if(btn.test(pt))
        {
          auto  old = current_button       ;
                      current_button = &btn;

            if(press)
            {
              (btn.manager.*btn.callback)();
            }


          return(old != current_button);
        }
    }


    if(current_button)
    {
      current_button = nullptr;

      return true;
    }


  return false;
}


void
put_color(uint32_t  color, int  x, int  y)
{
  auto  dst = row_table[y]+(bpp*x);

    switch(bpp)
    {
  case(1):                              *dst = color;break;
  case(2): *reinterpret_cast<uint16_t*>(dst) = color;break;
  case(4): *reinterpret_cast<uint32_t*>(dst) = color;break;
    }
}


void
render(const Glyph*  gl, uint32_t  color, int  x, int  y)
{
    if(!gl)
    {
      return;
    }


  const uint16_t*  p = gl->data;

  x += 2;
  y += 2;

    for(int  yy = 0;  yy < Glyph::size;  ++yy)
    {
      auto  v = *p++;

        for(int  xx = 0;  xx < Glyph::size;  ++xx)
        {
            if(v&0x8000)
            {
              put_color(color,x+xx,y+yy);
            }


          v <<= 1;
        }
    }
}


void
put_string(const char*  s, uint32_t  color, int  x, int  y)
{
  bool  strong_flag = false;

    while(*s)
    {
      auto  c = *s++;

        if(c == '*')
        {
          strong_flag = !strong_flag;
        }

      else
        {
          render(get_glyph(c),strong_flag? red:color,x,y);

          x += 16;
        }
    }
}


void
put_string(const char16_t*  s, uint32_t  color, int  x, int  y)
{
  bool  strong_flag = false;

    while(*s)
    {
      auto  c = *s++;

        if(c == '*')
        {
          strong_flag = !strong_flag;
        }

      else
        {
          render(get_glyph(c),strong_flag? red:color,x,y);

          x += 16;
        }
    }
}


void
put_renderer(const Renderer&  src, int  x, int  y)
{
  auto  w = src.get_width();
  auto  h = src.get_height();

    for(int  yy = 0;  yy < h;  ++yy){
    for(int  xx = 0;  xx < w;  ++xx){
      auto   i = src.get_cell(xx,yy).color_index;

        if(i)
        {
          put_color(palette[i],x+xx,y+yy);
        }
    }}
}


void
draw_rectangle(uint32_t  color, int  x, int  y, int  w, int  h)
{
    for(int  n = 0;  n < w;  ++n)
    {
      put_color(color,x+n,y    );
      put_color(color,x+n,y+h-1);
    }


  y += 1;
  h -= 2;

    while(h--)
    {
      put_color(color,x    ,y  );
      put_color(color,x+w-1,y++);
    }
}


void
fill_rectangle(uint32_t  color, int  x, int  y, int  w, int  h)
{
  SDL_Rect  rect = {x,y,w,h};

  SDL_FillRect(surface,&rect,color);
}


void
render_buttons()
{
    for(auto&  btn: button_list)
    {
      auto  flag = (current_button == &btn);

      put_string(btn.text,flag? yellow:white,btn.x,btn.y);

        if(flag)
        {
          draw_rectangle(white,btn.x,btn.y,btn.w,btn.h);
        }
    }
}


void
update()
{
  SDL_UpdateWindowSurface(window);


#ifdef EMSCRIPTEN
  emscripten_run_script(
  "var  cv = document.getElementById(\"canvas\");"
  "var  ln = document.getElementById(\"link\");"
  "ln.href = cv.toDataURL();"
  );
#endif
}


}




