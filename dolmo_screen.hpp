#ifndef DOLMO_SCREEN_HPP_INCLUDED
#define DOLMO_SCREEN_HPP_INCLUDED


#include<cstdint>
#include"dolmo_rootmanager.hpp"


using Callback = void(RootManager::*)();


namespace screen{


constexpr int  width  = 540;
constexpr int  height = 540;


void   open();
void  close();

void  clear();

void  make_button(int  x, int  y, const char*  text, RootManager&  mgr, Callback  cb);
bool  touch_button(int  x, int  y, bool  press=false);

void   put(const char*  s, int  x, int  y);
void   put(int  cur, int  max, int  x, int  y);
void   put(int  color_index, Node*  nodeptr, int  x, int  y);
Node*  get(int  x, int  y);

void  update(bool  show_menu);


}




#endif




