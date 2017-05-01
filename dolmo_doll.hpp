#ifndef DOLMO_DOLL_HPP_INCLUDED
#define DOLMO_DOLL_HPP_INCLUDED


#include<memory>
#include"dolmo_node.hpp"
#include"dolmo_scene.hpp"
#include"dolmo_renderer.hpp"
#include"dolmo_frame.hpp"




class
Doll
{
  Scene&  scene;

  int  index;

  std::unique_ptr<Node>  root_node;

  int  scale_level;

  bool  reverse_flag;

  int  z_value;

  std::list<DollState>  state_list;

  friend void  DollState::store() const;
  friend Frame::~Frame();
  friend void  Frame::add(Doll&  doll);
  friend void  Frame::remove(Doll&  doll);

  DollStateIterator     new_state(Frame&  frame);
  void               delete_state(DollStateIterator  it);

public:
  Doll(Scene&  scene_, Node*  root, int  z, bool  rev=false);
  Doll(Scene&  scene_, const libjson::Value&  value);
  Doll(const Doll&   rhs) = delete;
  Doll(      Doll&&  rhs) = delete;


  Doll&  operator=(const Doll&   rhs) = delete;
  Doll&  operator=(      Doll&&  rhs) = delete;

        Node&  operator*()      ;
  const Node&  operator*() const;

  void  clear();

  void  change_index(uint32_t  i);
  int      get_index() const{return index;}

  bool  test_reverse_flag() const;

  void  switch_reverse_flag();

  void  increase_scale_level();
  void  decrease_scale_level();

  int  get_scale_level() const{return scale_level;}
  int  get_z_value() const;

  const Node&  get_root_node() const;

  void  change_position(const Point&  pt);
  void  add_to_position(const Point&  pt);


  void  update();

  void  render(Renderer&  dst, int  z_max) const;


  void  scan(const libjson::Value&  val);
  libjson::Value  to_json() const;

};


#endif




