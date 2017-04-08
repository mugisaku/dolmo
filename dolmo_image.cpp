#include"dolmo_image.hpp"
#include<SDL.h>
#include<SDL_image.h>
#include<vector>




namespace{


int  width;
int  height;


std::vector<uint8_t>
buffer;


}


namespace image{




void
open(const char*  path)
{
  IMG_Init(IMG_INIT_PNG);

  auto  bmp = IMG_Load(path);

  width  = bmp->w;
  height = bmp->h;

  buffer.resize(width*height);

  auto  dst      = buffer.begin();
  auto  src_base = static_cast<const uint8_t*>(bmp->pixels);

    for(int  y = 0;  y < height;  ++y)
    {
      auto  src = src_base              ;
                  src_base += bmp->pitch;

        for(int  x = 0;  x < width;  ++x)
        {
          *dst++ = *src++;
        }
    }


  SDL_FreeSurface(bmp);

  IMG_Quit();
}


int
get(int  x, int  y)
{
  return buffer[width*y+x];
}




}




