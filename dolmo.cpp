#include"dolmo_node.hpp"
#include"dolmo_frame.hpp"
#include"dolmo_scene.hpp"
#include"dolmo_doll.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_renderer.hpp"
#include"dolmo_image.hpp"
#include"dolmo_model.hpp"
#include"dolmo_font.hpp"
#include"dolmo_gui.hpp"
#include"json_stream.hpp"
#include<cstdlib>
#include<string>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


constexpr int  top_panel_height = 32;


Scene*
scn;


SceneEditor
editor;


Renderer
renderer(screen::width,screen::height-top_panel_height);


bool
hide_menu;


uint32_t  gray;


void
render(bool  ok)
{
    if(ok)
    {
      screen::lock();

      screen::clear();

      screen::put_renderer(renderer,0,top_panel_height);

        if(!hide_menu)
        {
          auto  ns = editor.get_numbers();

          screen::fill_rectangle(gray,0,0,screen::width,top_panel_height);

          char  buf[80];

          snprintf(buf,sizeof(buf),"FRAME[%2d/%2d]",ns.first+1,ns.second);

          screen::put_string(buf,screen::white,0,0);

          snprintf(buf,sizeof(buf),"Z_MAX[%2d/%2d]",editor.get_z_max(),SceneEditor::z_max_max);

          screen::put_string(buf,screen::white,0,16);

          gui::render_buttons();
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
        if(hide_menu || !gui::touch_button(evt.x,evt.y,true))
        {
            if(evt.y >= top_panel_height)
            {
              editor.press(renderer,evt.x,evt.y-top_panel_height);
            }
        }
    }

  else
    if(evt.button == SDL_BUTTON_RIGHT)
    {
      hide_menu = !hide_menu;

      render(true);
    }
}


void
process_motion(const SDL_MouseMotionEvent&  evt)
{
    if(!hide_menu)
    {
      render(gui::touch_button(evt.x,evt.y,false));
    }


    if(evt.y >= top_panel_height)
    {
      editor.move_pointer(evt.x,evt.y-top_panel_height);
    }
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


      libjson::Stream  st(s.data());

      auto  val = st.get_value();

      scn->scan(val);
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
              render(editor.render(renderer,true));
            }
          break;
      case(SDL_KEYDOWN):
            if(evt.key.keysym.sym == SDLK_SPACE)
            {
              editor.save_as_png(renderer,"__DOLMO");

              auto  f = fopen("__DOLMO.json","wb");

                if(f)
                {
                  auto  v = scn->to_json();

                  auto  s = v.to_string();

                  fprintf(f,"%s\n",s.data());

                  fclose(f);
                }
            }
          break;
      case(SDL_QUIT):
          print_required_glyphes();
          fflush(stdout);
          screen::close();
          quick_exit(EXIT_SUCCESS);
          break;
#endif
      case(SDL_MOUSEBUTTONDOWN):
          process_button(evt.button);
          break;
      case(SDL_MOUSEBUTTONUP):
          editor.unpress();
          break;
      case(SDL_MOUSEMOTION):
          process_motion(evt.motion);
          break;
        }
    }


  editor.step();

  render(editor.render(renderer,false));
}


}




int
main(int  argc, char**  argv)
{
  screen::open();

  gui::initialize();

  image::open(0,"dolmo_human_parts.png");
  image::open(1,"dolmo_horse_parts.png");

  gray = screen::get_color(0x3F,0x3F,0x3F);

  scn = new Scene;

  editor.open(*scn);


  gui::make_button(16*20, 0,u"Copy",editor,&SceneEditor::copy_this);
  gui::make_button(16*20,16,u"Restore",editor,&SceneEditor::apply_copy);

  gui::make_button(16*12,16,u"-",editor,&SceneEditor::decrease_z_max);
  gui::make_button(16*13,16,u"+",editor,&SceneEditor::increase_z_max);


  int  y = screen::height/2-32;

  gui::make_button(               0,y   ,u"<",editor,&SceneEditor::change_to_previous);
  gui::make_button(screen::width-16,y   ,u">",editor,&SceneEditor::change_to_next);
  gui::make_button(               0,y+64,u"+",editor,&SceneEditor::insert_new_to_previous);
  gui::make_button(screen::width-16,y+64,u"+",editor,&SceneEditor::insert_new_to_next);

  gui::make_button(screen::width-(16*3),screen::height-16,u"DEL",editor,&SceneEditor::erase_this);

  gui::add_radio_button(u"人形(人)配置",editor,&SceneEditor::change_to_allocate_human_doll);
  gui::add_radio_button(u"人形(馬)配置",editor,&SceneEditor::change_to_allocate_horse_doll);
  gui::add_radio_button(u"人形削除",editor,&SceneEditor::change_to_remove_doll);
  gui::add_radio_button(u"角度変更",editor,&SceneEditor::change_to_move_angle);
  gui::add_radio_button(u"位置変更",editor,&SceneEditor::change_to_move_position);
  gui::add_radio_button(u"拡大",editor,&SceneEditor::change_to_increase_scale_level);
  gui::add_radio_button(u"縮小",editor,&SceneEditor::change_to_decrease_scale_level);
  gui::add_radio_button(u"反転切替",editor,&SceneEditor::change_to_switch_reversing);
  gui::add_radio_button(    u"動画",editor,&SceneEditor::change_to_animate);

  gui::fix_radio_buttons(0,top_panel_height);

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




