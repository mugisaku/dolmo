#ifndef DOLMO_SCREEN_HPP_INCLUDED
#define DOLMO_SCREEN_HPP_INCLUDED


#include<cstdint>
#include"dolmo_sceneEditor.hpp"


using Callback = void(SceneEditor::*)();


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


constexpr uint32_t  white = 0xFFFFFFFF;
constexpr uint32_t  black = 0;


void   open();
void  close();

uint32_t  get_color(uint8_t  r, uint8_t  g, uint8_t  b);


void    lock();
void  unlock();

void  clear();

void   put_color(uint32_t  color, int  x, int  y);
void   put_string(const char*      s, uint32_t  color, int  x, int  y);
void   put_string(const char16_t*  s, uint32_t  color, int  x, int  y);
void   put_renderer(const Renderer&  src, int  x, int  y);

void   draw_rectangle(uint32_t  color, int  x, int  y, int  w, int  h);
void   fill_rectangle(uint32_t  color, int  x, int  y, int  w, int  h);

void  update();


}




#endif




