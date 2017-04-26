#ifndef DOLMO_SCENEEDITOR_HPP_INCLUDED
#define DOLMO_SCENEEDITOR_HPP_INCLUDED


#include"dolmo_scene.hpp"
#include"dolmo_frame.hpp"
#include"dolmo_node.hpp"


class
SceneEditor
{
public:
  static constexpr int  z_max_max = 4;

  enum class Mode{
    allocate_doll,
    remove_doll,
    move_position,
    move_angle,
    switch_reversing,
    animation,
  };


private:
  Scene*  target;

  Mode  mode;

  Node*  current_node;

  Point  previous_point;
  Point   current_point;

  int  current_index;

  int  z_max;

  uint32_t  last_time;


  using FrameList = std::list<Frame>;

  FrameList::iterator    current_frame;
  FrameList::iterator  animation_frame;

  bool  needed_to_redraw;

public:
  SceneEditor();


  void  open(Scene&  new_target);

  void  increase_z_max();
  void  decrease_z_max();
  void  change_to_previous();
  void  change_to_next();
  void  insert_new_to_previous();
  void  insert_new_to_next();
  void  copy_this();
  void  apply_copy();
  void  erase_this();
  void  change_to_animate();
  void  change_to_move_angle();
  void  change_to_move_position();
  void  change_to_switch_reversing();
  void  change_to_remove_doll();
  void  change_to_allocate_doll();

  std::pair<int,int>  get_numbers() const;

  int  get_z_max() const{return z_max;}

  Mode  get_mode() const{return mode;}

  const Doll*  get_current_doll() const;

  void  change_mode(Mode  m);

  void  move_pointer(int  x, int  y);

  void    press(Renderer&  renderer, int  x, int  y);
  void  unpress(                                   );

  void         fprint(FILE*  f) const;
  const char*  sscan(const char*  s);

  void  save_as_png(Renderer&  r, const char*  base);

  void  step();
  bool  render(Renderer&  dst, bool  force);

};




#endif




