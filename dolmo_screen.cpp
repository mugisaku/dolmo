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

  RootManager&  manager;

  Callback  callback;

  Button(int  x_, int  y_, const char*  text_, RootManager&  mgr, Callback  cb):
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
make_button(int  x, int  y, const char*  text, SceneManager&  mgr, Callback  cb)
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
render(const Glyph&  gl, uint32_t  color, int  x, int  y)
{
  const uint8_t*  p = gl.data;

    for(int  yy = 0;  yy < Glyph::size;  ++yy)
    {
      auto  v = *p++;

        for(int  xx = 0;  xx < Glyph::size;  ++xx)
        {
            if(v&0x80)
            {
              int  xxx = x+(xx*2);
              int  yyy = y+(yy*2);

              put_color(color,xxx  ,yyy  );
              put_color(color,xxx+1,yyy  );
              put_color(color,xxx  ,yyy+1);
              put_color(color,xxx+1,yyy+1);
            }


          v <<= 1;
        }
    }
}


void
render(int  d, int  x, int  y)
{
  auto  i10 = d/10;
  auto  i1  = d%10;

    if(i10)
    {
      render(get_glyph(i10+'0'),0xFFFFFFFF,x,y);
    }


  render(get_glyph(i1+'0'),0xFFFFFFFF,x+16,y);
}


void
put(const char*  s, uint32_t  color, int  x, int  y)
{
    while(*s)
    {
      auto  c = *s++;

      render(get_glyph(c),color,x,y);

      x += 16;
    }
}


void
put(int  cur, int  max, int  x, int  y)
{
  render(           cur,           x   ,y);
  render(get_glyph('/'),0xFFFFFFFF,x+32,y);
  render(           max,           x+48,y);
}


void
put(const Renderer&  src)
{
    for(int  y = 0;  y < height;  ++y){
    for(int  x = 0;  x <  width;  ++x){
      auto   i = src.get_cell(width-1-x,y).color_index;

        if(i)
        {
          put_color(palette[i],x,y);
        }
    }}
}


void
render_buttons()
{
    for(auto&  btn: button_list)
    {
      auto  flag = (current_button == &btn);

      put(btn.text,flag? yellow:0xFFFFFFFF,btn.x,btn.y);
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




