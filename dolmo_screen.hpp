#ifndef DOLMO_SCREEN_HPP_INCLUDED
#define DOLMO_SCREEN_HPP_INCLUDED


#include<cstdint>
#include"dolmo_rootmanager.hpp"


using Callback = void(RootManager::*)();


namespace screen{


constexpr int  width  = 540;
constexpr int  height = 540;


constexpr uint8_t  luminance_table[24] =
{
  0x00,0x00,0x3F,0x7F,
  0x00,0x00,0x4F,0x9F,
  0x00,0x00,0x5F,0xBF,
  0x00,0x00,0x6F,0xDF,
  0x00,0x00,0x7F,0xFF,
  0x00,0x00,0x7F,0xFF,
};


void   open();
void  close();

void  clear();

void  make_button(int  x, int  y, const char*  text, RootManager&  mgr, Callback  cb);
bool  touch_button(int  x, int  y, bool  press=false);

void   clear();

void   put(const char*  s, int  x, int  y);
void   put(int  cur, int  max, int  x, int  y);
void   put(const Renderer&  src);

void  update(bool  show_menu);


}




#endif




