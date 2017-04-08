#ifndef DOLMO_SCREEN_HPP_INCLUDED
#define DOLMO_SCREEN_HPP_INCLUDED


#include<cstdint>


struct Node;


namespace screen{


constexpr int  width  = 600;
constexpr int  height = 600;


void   open();
void  close();

void  clear();

void  put(int  x, int  y, int  z, int  color_index, Node*  nodeptr);

void  update();


}




#endif




