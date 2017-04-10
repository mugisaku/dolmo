#include"dolmo_screen.hpp"
#include"dolmo_node.hpp"
#include<SDL.h>




namespace{


constexpr int  glyph_size = 8;


const uint8_t
slash_glyph[] =
{
  0b00000000,
  0b00000011,
  0b00000110,
  0b00000110,
  0b00011100,
  0b00110000,
  0b00110000,
  0b01100000,

};


const uint8_t
digit_glyph[] =
{
  0b00000000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b01000010,
  0b01000010,
  0b00100100,
  0b00011000,

  0b00000000,
  0b00001000,
  0b00011000,
  0b00101000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00111110,

  0b00000000,
  0b00011100,
  0b00100010,
  0b01000010,
  0b00000010,
  0b00011100,
  0b00100000,
  0b01111111,

  0b00000000,
  0b00111110,
  0b01000001,
  0b00000001,
  0b00001110,
  0b01000001,
  0b01000001,
  0b00111110,

  0b00000000,
  0b00000100,
  0b00001100,
  0b00010100,
  0b00100100,
  0b01111111,
  0b00000100,
  0b00000100,

  0b00000000,
  0b01111111,
  0b01000000,
  0b01011110,
  0b01100001,
  0b00000001,
  0b01000001,
  0b00111110,

  0b00000000,
  0b00111111,
  0b01000000,
  0b01000000,
  0b01111110,
  0b01000001,
  0b01000001,
  0b00111110,

  0b00000000,
  0b01111111,
  0b00000001,
  0b00000010,
  0b00000100,
  0b00001000,
  0b00010000,
  0b00100000,

  0b00000000,
  0b00111110,
  0b01000001,
  0b00111110,
  0b01000001,
  0b01000001,
  0b01000001,
  0b00111110,

  0b00000000,
  0b00111110,
  0b01000001,
  0b01000001,
  0b00111111,
  0b00000001,
  0b01000001,
  0b00111110,

};


SDL_Window*    window;
SDL_Surface*  surface;


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
render(const uint8_t*  p, int  x, int  y)
{
  auto  dst_base = &table[y][x];

    for(int  yy = 0;  yy < glyph_size;  ++yy)
    {
      auto  v = *p++;

      auto  dst0 = dst_base;
      auto  dst1 = dst_base+width;

      dst_base += width*2;

        for(int  xx = 0;  xx < glyph_size;  ++xx)
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
  auto  i10 = d/10;
  auto  i1  = d%10;

    if(i10)
    {
      render(&digit_glyph[glyph_size*i10],x,y);
    }


  render(&digit_glyph[glyph_size*i1],x+16,y);
}


void
put(int  cur, int  max, int  x, int  y)
{
  render(        cur,x   ,y);
  render(slash_glyph,x+32,y);
  render(        max,x+48,y);
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




