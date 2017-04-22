#ifndef DOLMO_SCREEN_HPP_INCLUDED
#define DOLMO_SCREEN_HPP_INCLUDED


#include<cstdint>
#include"dolmo_scenemanager.hpp"


using Callback = void(SceneManager::*)();


constexpr int  luminance_table_size = 15;

constexpr uint8_t  luminance_table[luminance_table_size] =
{
  0x00,0x00,0x7F,
  0x00,0x00,0x9F,
  0x00,0x00,0xBF,
  0x00,0x00,0xDF,
  0x00,0x00,0xFF,
};


namespace screen{


constexpr int  width  = 540;
constexpr int  height = 540;


void   open();
void  close();

void    lock();
void  unlock();

void  clear();

void   make_button(int  x, int  y, const char*  text, SceneManager&  mgr, Callback  cb);
bool  touch_button(int  x, int  y, bool  press=false);

void  render_buttons();

void   put(const char*  s, int  x, int  y);
void   put(int  cur, int  max, int  x, int  y);
void   put(const Renderer&  src);

void  update();


}




#endif




