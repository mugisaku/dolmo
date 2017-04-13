#include<SDL.h>
#include<SDL_image.h>
#include<cstdio>
#include<cstdint>




namespace{


void
save(const SDL_Surface*  bmp, FILE*  f)
{
  fprintf(f,"constexpr int   width = %d;\n",bmp->w);
  fprintf(f,"constexpr int  height = %d;\n",bmp->h);

  auto  src_base = static_cast<const uint8_t*>(bmp->pixels);

  fprintf(f,"const uint8_t\nbuffer[width*height] = {\n");

    for(int  y = 0;  y < bmp->h;  ++y)
    {
      auto  src = src_base              ;
                  src_base += bmp->pitch;

        for(int  x = 0;  x < bmp->w;  ++x)
        {
          fprintf(f,"%d,",*src++);
        }


      fprintf(f,"\n");
    }


  fprintf(f,"\n};\n");
}


}


int
main(int  argc, char**  argv)
{
    if(argc == 2)
    {
      auto  f = fopen("dolmo_image_embedded.cpp","wb");

        if(f)
        {
          SDL_Init(0);

          IMG_Init(IMG_INIT_PNG);

          auto  path = argv[1];

          auto  bmp = IMG_Load(path);

            if(bmp)
            {
              save(bmp,f);

              printf("%s is saved\n",path);

              SDL_FreeSurface(bmp);
            }


          IMG_Quit();
          SDL_Quit();

          fclose(f);
        }
    }


  return 0;
}




