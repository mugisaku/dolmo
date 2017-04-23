#ifndef DOLMO_DOLL_HPP_INCLUDED
#define DOLMO_DOLL_HPP_INCLUDED


#include<list>
#include<vector>
#include"dolmo_node.hpp"
#include"dolmo_scene.hpp"
#include"dolmo_renderer.hpp"


struct DollState;


class
Doll
{
  Scene*  scene;

  Node*  root_node;

  bool  reverse_flag;

  int  z_value;

  friend Doll*  Scene::join(Doll*  doll);

public:
  Doll(Node*  root=nullptr);
  Doll(Scene*  scene_, Node*  root);
  Doll(Doll&&  rhs) noexcept;
 ~Doll(                   );


  Doll&  operator=(Doll&&  rhs) noexcept;
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




