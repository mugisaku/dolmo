#ifndef DOLMO_SCENEMANAGER_HPP_INCLUDED
#define DOLMO_SCENEMANAGER_HPP_INCLUDED


#include"dolmo_scene.hpp"


class
SceneManager
{
  static constexpr int  z_max_max = 4;


  Node*  current_node;
  Node*     copy_node;

  Point  previous_point;
  Point   current_point;

  int  current_index;

  int  z_max;

  uint32_t  last_time;


  using List = std::list<Scene*>;

  List  scene_list;

  List::iterator        current_scene;
  List::const_iterator  current_frame;

  bool  animation_flag;
  bool  needed_to_redraw;

public:
  SceneManager();


  void  increase_z_max();
  void  decrease_z_max();
  void  change_to_previous();
  void  change_to_next();
  void  insert_new_to_previous();
  void  insert_new_to_next();
  void  copy_this();
  void  apply_copy();
  void  erase_this();
  void  start_to_animate();

  std::pair<int,int>  get_numbers() const;

  bool  test_animation_flag() const;

  void  unset_animation_flag();

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




