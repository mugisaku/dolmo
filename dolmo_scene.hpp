#ifndef DOLMO_SCENE_HPP_INCLUDED
#define DOLMO_SCENE_HPP_INCLUDED


#include<list>
#include<vector>
#include<cstdio>


struct Doll;
struct Renderer;


class
Scene
{
  std::list<Doll*>  doll_list;

public:
  Scene();
 ~Scene();

  void  clear();

  Doll*  join(Doll*  doll);

  void  update();

  void  render(Renderer&  dst, int  z_max);

  void  fprint(FILE*  f) const;
  const char*  sscan(const char*  s);

};




#endif




