#ifndef DOLMO_FRAME_HPP_INCLUDED
#define DOLMO_FRAME_HPP_INCLUDED


#include<list>
#include<vector>
#include<cstdio>
#include"dolmo_dollState.hpp"
#include"json.hpp"


struct Scene;
struct Renderer;


using DollStateIterator = std::list<DollState>::iterator;


class
Frame
{
  Scene&  scene;

  int  index;

  std::list<DollStateIterator>  dollstate_list;

public:
  Frame(Scene&  scene_);
  Frame(const Frame&  rhs)=delete;
 ~Frame();


  Frame&  operator=(const Frame&  rhs)=delete;

  void  change_index(uint32_t  i);
  int      get_index() const{return index;}

  void     add(Doll&  doll);
  void  remove(Doll&  doll);

  void    raise();
  void  unraise();

};




#endif




