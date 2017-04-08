#include"dolmo_screen.hpp"
#include<SDL.h>




namespace{


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
put(int  x, int  y, int  z, int  color_index, Node*  nodeptr)
{
    if((x >=     0) &&
       (y >=     0) &&
       (x <  width) &&
       (y < height))
    {
      auto&  cell = table[y][x];

        if(!cell.color_index || (cell.z < z))
        {
          cell.color_index = color_index;
          cell.z           = z;
          cell.nodeptr     = nodeptr;
        }
    }
}


void
update()
{
  SDL_memset(surface->pixels,0,surface->pitch*height);

  auto  dst_base = static_cast<uint8_t*>(surface->pixels);

    for(int  y = 0;  y < height;  ++y)
    {
      auto  dst = dst_base                  ;
                  dst_base += surface->pitch;

        for(int  x = 0;  x < width;  ++x)
        {
          auto   i = table[y][x].color_index;

            if(i)
            {
                switch(surface->format->BytesPerPixel)
                {
              case(1):                              *dst = palette[i];break;
              case(2): *reinterpret_cast<uint16_t*>(dst) = palette[i];break;
              case(4): *reinterpret_cast<uint32_t*>(dst) = palette[i];break;
                }
            }


          dst += surface->format->BytesPerPixel;
        }
    }


  SDL_UpdateWindowSurface(window);
}


}




