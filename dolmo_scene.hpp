#ifndef DOLMO_SCENE_HPP_INCLUDED
#define DOLMO_SCENE_HPP_INCLUDED


#include<list>
#include<vector>
#include<cstdio>
#include"dolmo_frame.hpp"


struct Doll;
struct Node;
struct Renderer;


class
Scene
{
  uint32_t  index;

  std::list<Doll>    doll_list;
  std::list<Frame>  frame_list;

  Frame  copybuffer_frame;

public:
  Scene();
  Scene(const libjson::Value&  value);

        std::list<Frame>*  operator->()      {return &frame_list;}
  const std::list<Frame>*  operator->() const{return &frame_list;}

        std::list<Frame>&  operator*()      {return frame_list;}
  const std::list<Frame>&  operator*() const{return frame_list;}

  void  clear();

  Doll&    allocate_doll(const Node&  model, int  x, int  y);
  void   deallocate_doll(Doll&  target);

  std::list<Frame>::iterator     new_frame(std::list<Frame>::iterator  it);
  std::list<Frame>::iterator  delete_frame(std::list<Frame>::iterator  it);

  void   backup_frame(const Frame&  frame) const;
  void  restore_frame(const Frame&  frame) const;

  void  update_index();

  void  render(Renderer&  dst, int  z_max);


  void  scan(const libjson::Value&  val);
  libjson::Value  to_json() const;

};




#endif




