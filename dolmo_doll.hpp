#ifndef DOLMO_DOLL_HPP_INCLUDED
#define DOLMO_DOLL_HPP_INCLUDED


#include<memory>
#include"dolmo_node.hpp"
#include"dolmo_scene.hpp"
#include"dolmo_renderer.hpp"


struct DollState;


class
Doll
{
  Scene&  scene;

  std::unique_ptr<Node>  root_node;

  bool  reverse_flag;

  int  z_value;

public:
  Doll(Scene&  scene_, Node*  root, int  z, bool  rev=false);
  Doll(const Doll&   rhs) = delete;
  Doll(      Doll&&  rhs) = delete;


  Doll&  operator=(const Doll&   rhs) = delete;
  Doll&  operator=(      Doll&&  rhs) = delete;

  const Node&  operator*() const;

  void  clear();

  int  get_z_value() const;

  const Node&  get_root_node() const;

  void  change_position(const Point&  pt);
  void  add_to_position(const Point&  pt);

  void  update();

  void  render(Renderer&  dst, int  z_max) const;

  void  fprint(FILE*  f) const;
  const char*   sscan(const char*  s);

  template<typename  T>void  push(const T*&  it){root_node->read(it);}

};


struct
DollState
{
  Frame*  frame;

  Doll*  const target;

  int  number_table[400];

  size_t  number_count;

  bool  reverse_flag;

  int  z_value;

  DollState(Doll*  target_): target(target_){}

  Doll*  operator->() const{return target;}

  void  push() const
  {
    const int*  p = number_table;

    target->push(p);
  }

  void  pull()
  {
    int*  p = number_table;

    number_count = (**target).write(p,std::end(number_table));
  }

};


#endif




