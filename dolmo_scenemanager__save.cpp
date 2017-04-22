#include"dolmo_scenemanager.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_renderer.hpp"
#include<cstring>
#include<cstdio>


#ifndef EMSCRIPTEN
#include<libpng/png.h>
#include<zlib.h>
#endif



#ifndef EMSCRIPTEN
namespace{
void
set(png_color&  target, uint8_t  l)
{
  target.red   = l;
  target.green = l;
  target.blue  = l;
}
}
#endif


void
SceneManager::
save_as_png(Renderer&  renderer, const char*  base)
{
#ifndef EMSCRIPTEN
  static int  n;

  char  buf[80];

  snprintf(buf,sizeof(buf),"%s%03d.png",base,n++);

  auto  f = fopen(buf,"wb");

    if(!f)
    {
      return;
    }


  png_color  png_palette[luminance_table_size+1];

    for(int  i = 0;  i < luminance_table_size;  ++i)
    {
      set(png_palette[i],luminance_table[i]);
    }


  png_palette[0].red   =   0;
  png_palette[0].green =   0;
  png_palette[0].blue  = 255;

  png_palette[luminance_table_size].red   = 255;
  png_palette[luminance_table_size].green =   0;
  png_palette[luminance_table_size].blue  =   0;

  png_structp  png = png_create_write_struct(PNG_LIBPNG_VER_STRING,nullptr,nullptr,nullptr);

  png_infop  png_info = png_create_info_struct(png);

  png_init_io(png,f);


  const int  ren_w = renderer.get_width();
  const int  ren_h = renderer.get_height();

  const int  cv_w = ren_w*scene_list.size();


  png_set_compression_level(png,Z_BEST_COMPRESSION);

  png_set_IHDR(png,png_info,cv_w,ren_h,8,
               PNG_COLOR_TYPE_PALETTE,
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);

  png_set_PLTE(png,png_info,png_palette,luminance_table_size+1);

  png_write_info(png,png_info);

  png_set_packing(png);

  std::vector<uint8_t>  buffer(cv_w*ren_h);

  int  x_base = 0;

    for(auto  scene: scene_list)
    {
      renderer.clear();

      scene->render(renderer,z_max_max);

        for(int  y = 0;  y < ren_h;  ++y)
        {
            for(int  x = 0;  x < ren_w;  ++x)
            {
              buffer[(cv_w*y)+(x_base+x)] = renderer.get_cell(x,y).color_index;
            }


          buffer[(cv_w*y)+(x_base        )] = luminance_table_size;
          buffer[(cv_w*y)+(x_base+ren_w-1)] = luminance_table_size;
        }


        for(int  x = 0;  x < ren_w;  ++x)
        {
          buffer[                 (x_base+x)] = luminance_table_size;
          buffer[(cv_w*(ren_h-1))+(x_base+x)] = luminance_table_size;
        }


      x_base += ren_w;
    }


    for(int  y = 0;  y < ren_h;  ++y)
    {
      png_write_row(png,&buffer[cv_w*y]);
    }


  png_write_end(png,png_info);
  png_destroy_write_struct(&png,&png_info);

  fclose(f)         ;
#endif
}




