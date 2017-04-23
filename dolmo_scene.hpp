#ifndef DOLMO_SCENE_HPP_INCLUDED
#define DOLMO_SCENE_HPP_INCLUDED


#include<list>
#include<vector>
#include<cstdio>


struct Doll;
struct Frame;
struct Renderer;


class
Scene
{
  std::list<Doll>    doll_list;
  std::list<Frame>  frame_list;

public:
  Scene();

        std::list<Frame>*  operator->()      {return &frame_list;}
  const std::list<Frame>*  operator->() const{return &frame_list;}

  void  clear();

  Doll&    allocate_doll();
  void   deallocate_doll(Doll&  target);

  std::list<Frame>::iterator  new_frame(std::list<Frame>::iterator  it);

  void  update();

  void  render(Renderer&  dst, int  z_max);

  void  fprint(FILE*  f) const;
  const char*  sscan(const char*  s);

};




#endif




