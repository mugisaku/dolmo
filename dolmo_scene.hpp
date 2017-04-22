#ifndef DOLMO_SCENE_HPP_INCLUDED
#define DOLMO_SCENE_HPP_INCLUDED


#include<list>
#include<vector>
#include"dolmo_doll.hpp"


class
Scene
{
  std::list<Doll*>  doll_list;

  int  z_max;

  bool  needed_to_redraw;

public:
  Scene();

  void  need_to_redraw();

  void  update();

  bool  render(Renderer&  dst, bool  force);

  void  fprint(FILE*  f) const;
  const char*  sscan(const char*  s);

};




#endif




