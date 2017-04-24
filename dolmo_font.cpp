#include"dolmo_font.hpp"
#include<cstdio>




namespace{


Glyph*
table[0x10000];


struct
Initializer
{
  static uint16_t  getc(FILE*  f)
  {
    uint16_t  v = fgetc(f)<<8;

    v |= fgetc(f);

    return v;
  }

  Initializer()
  {
    auto  f = fopen("/usr/local/share/oat/font.bin","rb");

      if(!f)
      {
        f = fopen("font.bin","rb");
      }


      if(f)
      {
        auto  n = getc(f);

          while(n--)
          {
            auto  u = getc(f);

            auto&  dst = table[u];

              if(!dst)
              {
                dst = new Glyph;

                  for(int  y = 0;  y < Glyph::size;  ++y)
                  {
                    dst->data[y] = getc(f);
                  }


                  if(u >= 0xFF00)
                  {
                    table[u-0xFF00+' '] = dst;
                  }
              }
          }


        fclose(f);
      }
  }

} init;


}


const Glyph*
get_glyph(char16_t  c)
{
  return table[c];
}




