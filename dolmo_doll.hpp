#ifndef DOLMO_DOLL_HPP_INCLUDED
#define DOLMO_DOLL_HPP_INCLUDED


#include<memory>
#include"dolmo_node.hpp"
#include"dolmo_scene.hpp"
#include"dolmo_renderer.hpp"
#include"dolmo_dollState.hpp"




class
Doll
{
  Scene&  scene;

  std::unique_ptr<Node>  root_node;

  bool  reverse_flag;

  int  z_value;

  friend void  DollState::store() const;


public:
  Doll(Scene&  scene_, Node*  root, int  z, bool  rev=false);
  Doll(const Doll&   rhs) = delete;
  Doll(      Doll&&  rhs) = delete;


  Doll&  operator=(const Doll&   rhs) = delete;
  Doll&  operator=(      Doll&&  rhs) = delete;

        Node&  operator*()      ;
  const Node&  operator*() const;

  void  clear();

  bool  test_reverse_flag() const;

  void  switch_reverse_flag();

  int  get_z_value() const;

  const Node&  get_root_node() const;

  void  change_position(const Point&  pt);
  void  add_to_position(const Point&  pt);

  void  update();

  void  render(Renderer&  dst, int  z_max) const;

  void  fprint(FILE*  f) const;
  const char*   sscan(const char*  s);

};


#endif




