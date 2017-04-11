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

  Callback  callback;

  Button(int  x_, int  y_, const char*  text_, Callback  cb):
  Rect(x_,y_,16*std::strlen(text_),16),
  text(text_),
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


uint32_t
palette[4];


std::vector<Button>
button_list;


}




namespace screen{




void
open()
{
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("dolmo - " __DATE__,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);

  surface = SDL_GetWindowSurface(window);

  palette[1] = SDL_MapRGB(surface->format,63,63,63);
  palette[2] = SDL_MapRGB(surface->format,127,127,127);
  palette[3] = SDL_MapRGB(surface->format,255,255,255);
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
make_button(int  x, int  y, const char*  text, Callback  cb)
{
  button_list.emplace_back(x,y,text,cb);
}


bool
push_button(int  x, int  y)
{
  Point  pt(x,y);

    for(auto&  btn: button_list)
    {
        if(btn.test(pt))
        {
          btn.callback();

          return true;
        }
    }


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
              (dst0  )->color_index = 3;
              (dst0+1)->color_index = 3;
              (dst1  )->color_index = 3;
              (dst1+1)->color_index = 3;
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
  auto  i10 = d/10+'0';
  auto  i1  = d%10+'0';

    if(i10)
    {
      render(get_glyph(i10),x,y);
    }


  render(get_glyph(i1),x+16,y);
}


void
put(const char*  s, int  x, int  y)
{
    while(*s)
    {
      render(get_glyph(*s++),x,y);

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
          cell.color_index = color_index;
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
update()
{
    for(auto&  btn: button_list)
    {
      put(btn.text,btn.x,btn.y);
    }


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


  SDL_UpdateWindowSurface(window);
}


}




