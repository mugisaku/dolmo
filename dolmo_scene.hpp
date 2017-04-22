#ifndef DOLMO_SCENE_HPP_INCLUDED
#define DOLMO_SCENE_HPP_INCLUDED


#include<list>
#include<vector>
#include"dolmo_node.hpp"
#include"dolmo_renderer.hpp"


struct Doll;


class
Scene
{
  std::list<Doll*>  doll_list;

  int  z_max;

  bool  needed_to_redraw;

public:
  Scene();

  void  need_to_redraw();

  bool  render(Renderer&  dst, bool  force);

};




#endif




