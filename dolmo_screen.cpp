#include"dolmo_screen.hpp"
#include"dolmo_node.hpp"
#include"dolmo_font.hpp"
#include<SDL.h>
#include<vector>
#include<cstring>




namespace{


SDL_Window*    window;
SDL_Surface*  surface;


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


struct
Cell
{
  int  z = 0;

  int  color_index = 0;

  Node*  nodeptr = nullptr;

};


Cell
table[screen::height][screen::width];


const Button*
current_button;


uint32_t
palette[4*5];


std::vector<Button>
button_list;


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

  palette[1] = map_rgb(0x00);
  palette[2] = map_rgb(0x3F);
  palette[3] = map_rgb(0x7F);

  palette[5] = map_rgb(0x00);
  palette[6] = map_rgb(0x4F);
  palette[7] = map_rgb(0x9F);

  palette[ 9] = map_rgb(0x00);
  palette[10] = map_rgb(0x5F);
  palette[11] = map_rgb(0xBF);

  palette[13] = map_rgb(0x00);
  palette[14] = map_rgb(0x6F);
  palette[15] = map_rgb(0xDF);

  palette[17] = map_rgb(0x00);
  palette[18] = map_rgb(0x7F);
  palette[19] = map_rgb(0xFF);
}


void
close()
{
  SDL_DestroyWindow(window);

  SDL_Quit();
}


void
clear()
{
  auto  it  = &table[0][0];

  constexpr auto  end = &table[height-1][width];

    while(it != end)
    {
      it->color_index = 0;
      it->z           = 0;
      it->nodeptr     = nullptr;

      ++it;
    }
}




void
make_button(int  x, int  y, const char*  text, RootManager&  mgr, Callback  cb)
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
          current_button = &btn;

            if(press)
            {
              (btn.manager.*btn.callback)();
            }


          return true;
        }
    }


  current_button = nullptr;

  return false;
}




void
render(const Glyph&  gl, int  x, int  y)
{
  auto  dst_base = &table[y][x];

  const uint8_t*  p = gl.data;

    for(int  yy = 0;  yy < Glyph::size;  ++yy)
    {
      auto  v = *p++;

      auto  dst0 = dst_base;
      auto  dst1 = dst_base+width;

      dst_base += width*2;

        for(int  xx = 0;  xx < Glyph::size;  ++xx)
        {
            if(v&0x80)
            {
              (dst0  )->color_index = 19;
              (dst0+1)->color_index = 19;
              (dst1  )->color_index = 19;
              (dst1+1)->color_index = 19;
            }


          dst0 += 2;
          dst1 += 2;

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
      render(get_glyph(i10+'0'),x,y);
    }


  render(get_glyph(i1+'0'),x+16,y);
}


void
put(const char*  s, int  x, int  y)
{
    while(*s)
    {
      auto  c = *s++;

      render(get_glyph(c),x,y);

      x += 16;
    }
}


void
put(int  cur, int  max, int  x, int  y)
{
  render(           cur,x   ,y);
  render(get_glyph('/'),x+32,y);
  render(           max,x+48,y);
}


void
put(int  color_index, Node*  nodeptr, int  x, int  y)
{
    if((x >=     0) &&
       (y >=     0) &&
       (x <  width) &&
       (y < height))
    {
      auto&  cell = table[y][x];

      auto  z = nodeptr->z_value;

        if(!cell.color_index || (cell.z < z))
        {
          cell.color_index = color_index|(z<<2);
          cell.z           = z;
          cell.nodeptr     = nodeptr;
        }
    }
}


Node*
get(int  x, int  y)
{
  return table[y][x].nodeptr;
}


void
save(const char*  path)
{
  SDL_SaveBMP(surface,path);
}


void
render()
{
  SDL_memset(surface->pixels,0,surface->pitch*height);

  auto  dst_base = static_cast<uint8_t*>(surface->pixels);

  const auto  bpp = surface->format->BytesPerPixel;

    for(int  y = 0;  y < height;  ++y)
    {
      auto  dst = dst_base                  ;
                  dst_base += surface->pitch;

        for(int  x = 0;  x < width;  ++x)
        {
          auto   i = table[y][x].color_index;

            if(i)
            {
                switch(bpp)
                {
              case(1):                              *dst = palette[i];break;
              case(2): *reinterpret_cast<uint16_t*>(dst) = palette[i];break;
              case(4): *reinterpret_cast<uint32_t*>(dst) = palette[i];break;
                }
            }


          dst += bpp;
        }
    }
}


void
update(bool  show_menu)
{
    if(show_menu)
    {
        for(auto&  btn: button_list)
        {
          put(btn.text,btn.x,btn.y);
        }
    }


  render();

  SDL_UpdateWindowSurface(window);
}


}




