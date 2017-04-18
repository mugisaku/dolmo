#ifndef DOLMO_RENDERER_HPP_INCLUDED
#define DOLMO_RENDERER_HPP_INCLUDED


#include<cstdint>
#include<vector>




struct Node;


struct
Cell
{
  int  z = 0;

  int  color_index = 0;

  Node*  nodeptr = nullptr;

};


class
Renderer
{
  std::vector<Cell>  table;

  int  width;
  int  height;

public:
  Renderer(int  w=0, int  h=0);

  void  clear();

  void  resize(int  w, int  h);

  int   get_width() const;
  int   get_height() const;

  void   put(int  color_index, Node*  nodeptr, int  x, int  y);

  const Cell&  get_cell(int  x, int  y) const;

};




#endif




