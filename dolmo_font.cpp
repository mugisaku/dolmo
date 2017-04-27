#include"dolmo_font.hpp"
#include<cstdio>




namespace{


Glyph*
pointer_table[0x10000];


#ifdef EMSCRIPTEN
#include"builddir/font_glyphes.cpp"
#endif


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
#ifdef EMSCRIPTEN
    auto  n = sizeof(table)/(sizeof(uint16_t)*(Glyph::size+1));

    const uint16_t*  p = table;

      while(n--)
      {
        auto  u = *p++;

        auto  gl = new Glyph;

        gl->unicode = u;

          for(int  y = 0;  y < Glyph::size;  ++y)
          {
            gl->data[y] = *p++;
          }


        pointer_table[u] = gl;
      }
#else
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

            auto&  dst = pointer_table[u];

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

                    pointer_table[u] = dst;
                  }

                else
                  if(u == u'─')
                  {
                    u = '-';

                    dst->unicode = u;

                    pointer_table[u] = dst;
                  }
              }
          }


        fclose(f);
      }
#endif
  }

} init;


}


const Glyph*
get_glyph(char16_t  c)
{
  auto  g = pointer_table[c];

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

        for(auto  gl: pointer_table)
        {
            if(gl && gl->count)
            {
              gl->count = 0;

              fprintf(f,"0x%04X,",gl->unicode);

                for(auto  ln: gl->data)
                {
                  fprintf(f,"0x%04X,",ln);
                }


              fprintf(f,"\n");
            }
        }


      fprintf(f,"};\n\n\n");

      fclose(f);
    }
}




