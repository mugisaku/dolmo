#ifndef DOLMO_ROOTMANAGER_HPP_INCLUDED
#define DOLMO_ROOTMANAGER_HPP_INCLUDED


#include<list>
#include<vector>
#include"dolmo_node.hpp"


class
RootManager
{
  static constexpr int  z_max_max = 4;

  static std::vector<Node*>  trash;

  static Node*  raise_node();


  Node*  current_node;
  Node*     copy_node;

  Point  previous_point;
  Point   current_point;

  int  current_index;

  int  z_max;

  uint32_t  last_time;


  using List = std::list<Node*>;

  List  root_list;

  List::iterator         current_root;
  List::const_iterator  current_frame;

  bool  animation_flag;
  bool  needed_to_redraw;

public:
  RootManager();


  void  increase_z_max();
  void  decrease_z_max();
  void  change_to_previous();
  void  change_to_next();
  void  insert_new_to_previous();
  void  insert_new_to_next();
  void  copy_this();
  void  apply_copy();
  void  erase_this();
  void  print();
  void  start_to_animate();


  void  move_pointer(int  x, int  y);

  void    press(int  x, int  y);
  void  unpress(              );

  void  load(const char*  s);

  void  step();
  void  render(bool  force);

};




#endif




