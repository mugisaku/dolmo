#include"dolmo_image.hpp"
#include<SDL.h>
#include<SDL_image.h>
#include<vector>




namespace{


#ifdef EMSCRIPTEN
#include"domo_image_embedded.cpp"
#else
int  width;
int  height;
std::vector<uint8_t>
buffer;
#endif


}


namespace image{




void
open(const char*  path)
{
#ifndef EMSCRIPTEN
  IMG_Init(IMG_INIT_PNG);

  auto  bmp = IMG_Load(path);

  width  = bmp->w;
  height = bmp->h;

  buffer.resize(width*height);

  auto  dst      = buffer.begin();
  auto  src_base = static_cast<const uint8_t*>(bmp->pixels);

    for(int  y = 0;  y < bmp->h;  ++y)
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
#endif
}


int
get(int  x, int  y)
{
  return buffer[width*y+x];
}




}




