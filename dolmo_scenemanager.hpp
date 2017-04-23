#ifndef DOLMO_SCENEMANAGER_HPP_INCLUDED
#define DOLMO_SCENEMANAGER_HPP_INCLUDED


#include"dolmo_scene.hpp"
#include"dolmo_frame.hpp"
#include"dolmo_node.hpp"


enum class
Mode
{
  move_position,
  change_angle,
  animation,

};


class
SceneManager
{
  static constexpr int  z_max_max = 4;

  Mode  mode;

  Node*  current_node;
  Node*     copy_node;

  Point  previous_point;
  Point   current_point;

  int  current_index;

  int  z_max;

  uint32_t  last_time;


  using SceneList = std::list<Scene>;
  using FrameList = std::list<Frame>;

  SceneList  scene_list;

  SceneList::iterator  current_scene;

  FrameList::iterator    edition_frame;
  FrameList::iterator  animation_frame;

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

  Mode  get_mode() const;

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




