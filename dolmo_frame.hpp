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
  Scene*  scene;

  std::list<DollState>  dollstate_list;

public:
  Frame(Scene*  scene_=nullptr);

  void     add(Doll&  doll);
  void  remove(Doll&  doll);

  void    raise();
  void  unraise();

};




#endif




