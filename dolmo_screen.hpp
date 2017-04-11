#ifndef DOLMO_SCREEN_HPP_INCLUDED
#define DOLMO_SCREEN_HPP_INCLUDED


#include<cstdint>


struct Node;
struct Point;


using Callback = void(*)();


namespace screen{


constexpr int  width  = 640;
constexpr int  height = 640;


void   open();
void  close();

void  clear();

void  make_button(int  x, int  y, const char*  text, Callback  cb);

bool  push_button(int  x, int  y);

void   put(const char*  s, int  x, int  y);
void   put(int  cur, int  max, int  x, int  y);
void   put(int  color_index, Node*  nodeptr, int  x, int  y);
Node*  get(int  x, int  y);

void  update();


}




#endif




