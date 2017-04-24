#include"dolmo_gui.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_font.hpp"
#include"dolmo_rectangle.hpp"
#include"dolmo_sceneEditor.hpp"
#include<vector>
#include<cstring>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif




namespace{


struct
Button: public  Rect
{
  const char16_t*  text;

  SceneEditor&  manager;

  Callback  callback;

  Button(int  x_, int  y_, const char16_t*  text_, SceneEditor&  mgr, Callback  cb):
  Rect(x_,y_,16,16),
  text(text_),
  manager(mgr),
  callback(cb)
  {
    int  len = 0;

      while(*text_++)
      {
        ++len;
      }


    this->w *= len;
  }

};


const Button*
current_button;


const Button*
current_radio_button;


uint32_t
red;


uint32_t
yellow;


std::vector<Button>
button_list;

std::vector<Button>
radio_button_list;


}




namespace gui{




void
initialize()
{
  red    = screen::get_color(0xFF,0x00,0x00);
  yellow = screen::get_color(0xFF,0xFF,0x00);

}




void
make_button(int  x, int  y, const char16_t*  text, SceneEditor&  mgr, Callback  cb)
{
  button_list.emplace_back(x,y,text,mgr,cb);
}


void
add_radio_button(const char16_t*  text, SceneEditor&  mgr, Callback  cb)
{
  radio_button_list.emplace_back(0,0,text,mgr,cb);
}


void
fix_radio_buttons(int  x, int  y)
{
  y -= 16*radio_button_list.size();

    for(auto&  btn: radio_button_list)
    {
      btn.Point::assign(x,y);

      y += 16;
    }


  current_radio_button = &*radio_button_list.begin();
}


bool
touch_button(int  x, int  y, bool  press)
{
  Point  pt(x,y);

    for(auto&  btn: button_list)
    {
        if(btn.test(pt))
        {
          auto  old = current_button       ;
                      current_button = &btn;

            if(press)
            {
              (btn.manager.*btn.callback)();
            }


          return(old != current_button);
        }
    }


    if(current_button)
    {
      current_button = nullptr;

      return true;
    }


    for(auto&  btn: radio_button_list)
    {
        if(btn.test(pt))
        {
            if(press)
            {
              current_radio_button = &btn;

              (btn.manager.*btn.callback)();
            }


          return true;
        }
    }


  return false;
}




void
render_buttons()
{
    for(auto&  btn: button_list)
    {
      auto  flag = (current_button == &btn);

      screen::put_string(btn.text,flag? yellow:screen::white,btn.x,btn.y);

        if(flag)
        {
          screen::draw_rectangle(screen::white,btn.x,btn.y,btn.w,btn.h);
        }
    }


    for(auto&  btn: radio_button_list)
    {
      auto  flag = (current_radio_button == &btn);

      screen::put_string(btn.text,flag? yellow:screen::white,btn.x,btn.y);

        if(flag)
        {
          screen::draw_rectangle(screen::white,btn.x,btn.y,btn.w,btn.h);
        }
    }
}




}




