#include"dolmo_image.hpp"
#include<libpng/png.h>
#include<vector>
#include<cstdint>




namespace{


int  width;
int  height;


std::vector<uint8_t>
table;


}


namespace image{




void
open(const char*  path)
{
  auto  f = fopen(path,"rb");

    if(!f)
    {
      return;
    }


  auto  png      = png_create_read_struct(PNG_LIBPNG_VER_STRING,nullptr,nullptr,nullptr);
  auto  png_info = png_create_info_struct(png);

  png_init_io(png,f);

  png_read_info(png,png_info);


  const auto           w = png_get_image_width( png,png_info);
  const auto           h = png_get_image_height(png,png_info);
  const auto       depth = png_get_bit_depth(   png,png_info);
  const auto  color_type = png_get_color_type(  png,png_info);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
    {
      width  = w;
      height = h;

      table.resize(w*h);

        if(depth < 8)
        {
          png_set_packing(png);
        }


      auto  buffer = new uint8_t[w];

      auto  dst = table.begin();

        for(int  y = 0;  y < h;  ++y)
        {
          png_read_row(png,buffer,nullptr);

          const uint8_t*  src = buffer;

            for(int  x = 0;  x < w;  ++x)
            {
              *dst++ = *src++;
            }
        }


      delete[]  buffer;
    }


  png_read_end(png,png_info);
  png_destroy_read_struct(&png,&png_info,nullptr);

  fclose(f);
}


int
get(int  x, int  y)
{
  return table[width*y+x];
}




}






