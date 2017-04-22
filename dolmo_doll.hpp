#ifndef DOLMO_DOLL_HPP_INCLUDED
#define DOLMO_DOLL_HPP_INCLUDED


#include<list>
#include<vector>
#include"dolmo_node.hpp"
#include"dolmo_renderer.hpp"


struct Scene;


class
Doll
{
  Scene*  scene;

  Node*  root_node;

  bool  reverse_flag;

  int  z_value;

public:
  Doll(Scene*  scn=nullptr);

  int  get_z_value() const;

  void  update();

  void  render(Renderer&  dst, int  z_max) const;

  void  fprint(FILE*  f) const;
  const char*   sscan(const char*  s);

};




#endif




