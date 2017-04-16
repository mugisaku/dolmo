#include"dolmo_node.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_image.hpp"
#include"dolmo_model.hpp"
#include"dolmo_rootmanager.hpp"
#include<cstdlib>
#include<string>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


RootManager
mgr;


void
process_button(const SDL_MouseButtonEvent&  evt)
{
  mgr.press(evt.x,evt.y);
}


void
process_motion(const SDL_MouseMotionEvent&  evt)
{
  mgr.move_pointer(evt.x,evt.y);
}


void
load(char*  path)
{
  auto  f = fopen(path,"rb");

    if(f)
    {
      std::string  s;

        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f) || ferror(f))
            {
              break;
            }


          s.push_back(c);
        }


      fclose(f);

      mgr.load(s.data());
    }


  SDL_free(path);
}


void
main_loop()
{
  static SDL_Event  evt;

    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
      case(SDL_DROPFILE):
          load(evt.drop.file);
          break;
      case(SDL_WINDOWEVENT):
            if(evt.window.event == SDL_WINDOWEVENT_EXPOSED)
            {
              mgr.render(true);
            }
          break;
      case(SDL_MOUSEBUTTONDOWN):
          process_button(evt.button);
          break;
      case(SDL_MOUSEBUTTONUP):
          mgr.unpress();
          break;
      case(SDL_MOUSEMOTION):
          process_motion(evt.motion);
          break;
      case(SDL_KEYDOWN):
            if(evt.key.keysym.sym == SDLK_SPACE)
            {
              mgr.save("__DOLMO");
              mgr.print();
            }
          break;
      case(SDL_QUIT):
          fflush(stdout);
          screen::close();
          quick_exit(EXIT_SUCCESS);
          break;
        }
    }


  mgr.step();
  mgr.render(false);
}


}




int
main(int  argc, char**  argv)
{
  screen::open();
  image::open("dolmo_parts.png");


  int  y = 20;

  screen::make_button(0,y,"increase z max",mgr,&RootManager::increase_z_max);  y += 20;
  screen::make_button(0,y,"decrease z max",mgr,&RootManager::decrease_z_max);  y += 20;
  screen::make_button(0,y,"change to previous",mgr,&RootManager::change_to_previous);  y += 20;
  screen::make_button(0,y,"change to next",mgr,&RootManager::change_to_next);  y += 20;
  screen::make_button(0,y,"insert new to previous",mgr,&RootManager::insert_new_to_previous);  y += 20;
  screen::make_button(0,y,"insert new to next",mgr,&RootManager::insert_new_to_next);  y += 20;
  screen::make_button(0,y,"copy this",mgr,&RootManager::copy_this);  y += 20;
  screen::make_button(0,y,"apply copy",mgr,&RootManager::apply_copy);  y += 20;
  screen::make_button(0,y,"erase this",mgr,&RootManager::erase_this);  y += 20;
  screen::make_button(0,y,"animate",mgr,&RootManager::start_to_animate);  y += 20;


#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,-1,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }
#endif


  return 0;
}




