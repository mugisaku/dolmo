#ifndef DOLMO_SCREEN_HPP_INCLUDED
#define DOLMO_SCREEN_HPP_INCLUDED


#include<cstdint>


struct Node;


namespace screen{


constexpr int  width  = 500;
constexpr int  height = 640;


void   open();
void  close();

void  clear();

void   put(int  cur, int  max, int  x, int  y);
void   put(int  color_index, Node*  nodeptr, int  x, int  y);
Node*  get(int  x, int  y);

void  update();


}




#endif




