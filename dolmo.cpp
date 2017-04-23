#include"dolmo_node.hpp"
#include"dolmo_frame.hpp"
#include"dolmo_scene.hpp"
#include"dolmo_doll.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_renderer.hpp"
#include"dolmo_image.hpp"
#include"dolmo_model.hpp"
#include<cstdlib>
#include<string>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


SceneManager
mgr;


Renderer
renderer(screen::width,screen::height);


bool
hide_menu;


void
render(bool  ok)
{
    if(ok)
    {
      screen::lock();

      screen::clear();

      screen::put(renderer);

        if(!hide_menu && !(mgr.get_mode() == Mode::animation))
        {
          auto  ns = mgr.get_numbers();

          screen::put(ns.first+1,ns.second,0,0);

          screen::render_buttons();
        }


      screen::unlock();


      screen::update();
    }
}


void
process_button(const SDL_MouseButtonEvent&  evt)
{
    if(evt.button == SDL_BUTTON_LEFT)
    {
        if(hide_menu || !screen::touch_button(evt.x,evt.y,true))
        {
          mgr.press(renderer,evt.x,evt.y);
        }
    }

  else
    if(evt.button == SDL_BUTTON_RIGHT)
    {
        if(mgr.get_mode() == Mode::animation)
        {
          mgr.change_mode(Mode::change_angle);
        }

      else
        {
          hide_menu = !hide_menu;
        }


      render(true);
    }
}


void
process_motion(const SDL_MouseMotionEvent&  evt)
{
    if(!hide_menu)
    {
      render(screen::touch_button(evt.x,evt.y,false));
    }


  mgr.move_pointer(evt.x,evt.y);
}


#ifndef EMSCRIPTEN
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

      mgr.sscan(s.data());
    }


  SDL_free(path);
}
#endif


void
main_loop()
{
  static SDL_Event  evt;

    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
#ifndef EMSCRIPTEN
      case(SDL_DROPFILE):
          load(evt.drop.file);
          break;
      case(SDL_WINDOWEVENT):
            if(evt.window.event == SDL_WINDOWEVENT_EXPOSED)
            {
              render(mgr.render(renderer,true));
            }
          break;
      case(SDL_KEYDOWN):
            if(evt.key.keysym.sym == SDLK_SPACE)
            {
              mgr.save_as_png(renderer,"__DOLMO");

              auto  f = fopen("__DOLMO.txt","wb");

                if(f)
                {
                  mgr.fprint(f);

                  fclose(f);
                }
            }
          break;
      case(SDL_QUIT):
          fflush(stdout);
          screen::close();
          quick_exit(EXIT_SUCCESS);
          break;
#endif
      case(SDL_MOUSEBUTTONDOWN):
          process_button(evt.button);
          break;
      case(SDL_MOUSEBUTTONUP):
          mgr.unpress();
          break;
      case(SDL_MOUSEMOTION):
          process_motion(evt.motion);
          break;
        }
    }


  mgr.step();

  render(mgr.render(renderer,false));
}


}




int
main(int  argc, char**  argv)
{
  screen::open();
  image::open("dolmo_parts.png");


  int  y = 20;

  screen::make_button(0,y,"increase z max",mgr,&SceneManager::increase_z_max);  y += 20;
  screen::make_button(0,y,"decrease z max",mgr,&SceneManager::decrease_z_max);  y += 20;
  screen::make_button(0,y,"change to previous",mgr,&SceneManager::change_to_previous);  y += 20;
  screen::make_button(0,y,"change to next",mgr,&SceneManager::change_to_next);  y += 20;
  screen::make_button(0,y,"insert new to previous",mgr,&SceneManager::insert_new_to_previous);  y += 20;
  screen::make_button(0,y,"insert new to next",mgr,&SceneManager::insert_new_to_next);  y += 20;
  screen::make_button(0,y,"copy this",mgr,&SceneManager::copy_this);  y += 20;
  screen::make_button(0,y,"apply copy",mgr,&SceneManager::apply_copy);  y += 20;
  screen::make_button(0,y,"erase this",mgr,&SceneManager::erase_this);  y += 20;
  screen::make_button(0,y,"animate",mgr,&SceneManager::start_to_animate);  y += 20;

  render(true);

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




