#ifndef DOLMO_FRAME_HPP_INCLUDED
#define DOLMO_FRAME_HPP_INCLUDED


#include<list>
#include<vector>
#include<cstdio>


struct Doll;
struct DollState;
struct Scene;
struct Renderer;


class
Frame
{
  Scene&  scene;

  std::list<DollState>  dollstate_list;

public:
  Frame(Scene&  scene_);

  void  join(Doll*  doll);

  void  update();

  void  render(Renderer&  dst, int  z_max);

  void  fprint(FILE*  f) const;
  const char*  sscan(const char*  s);

};




#endif




