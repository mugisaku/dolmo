#include"dolmo_font.hpp"
#include<cstdio>




namespace{


Glyph*
table[0x10000];


uint16_t
fgetc16be(FILE*  f)
{
  uint16_t  v = fgetc(f)<<8;

  v |= fgetc(f);

  return v;
}


void
fputc16be(int  c, FILE*  f)
{
  fputc((c>>8)&0xFF,f);
  fputc((c   )&0xFF,f);
}


struct
Initializer
{
  Initializer()
  {
    auto  f = fopen("/usr/local/share/oat/font.bin","rb");

      if(!f)
      {
        f = fopen("font.bin","rb");
      }


      if(f)
      {
        auto  n = fgetc16be(f);

          while(n--)
          {
            auto  u = fgetc16be(f);

            auto&  dst = table[u];

              if(!dst)
              {
                dst = new Glyph;

                dst->unicode = u;
                dst->count   = 0;

                  for(int  y = 0;  y < Glyph::size;  ++y)
                  {
                    dst->data[y] = fgetc16be(f);
                  }


                  if(u >= 0xFF00)
                  {
                    u = u-0xFF00+' ';

                    dst->unicode = u;

                    table[u] = dst;
                  }

                else
                  if(u == u'─')
                  {
                    u = '-';

                    dst->unicode = u;

                    table[u] = dst;
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
  auto  g = table[c];

    if(g)
    {
      g->count = 1;
    }


  return g;
}


void
print_required_glyphes()
{
  auto  f = fopen("font_glyphes.cpp","wb");

    if(f)
    {
      fprintf(f,"const uint16_t\ntable[] =\n{\n");

        for(auto  gl: table)
        {
            if(gl && gl->count)
            {
              gl->count = 0;

              fprintf(f,"0x%04X,",gl->unicode);

                for(auto  ln: gl->data)
                {
                  fprintf(f,"0x%04X,",gl->unicode);
                }


              fprintf(f,"\n");
            }
        }


      fprintf(f,"};\n\n\n");

      fclose(f);
    }
}




