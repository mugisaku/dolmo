#ifndef DOLMO_NODE_HPP_INCLUDED
#define DOLMO_NODE_HPP_INCLUDED


#include<cstdint>
#include<cstdio>
#include<cmath>
#include<vector>
#include<SDL.h>
#include<SDL_image.h>
#include"dolmo_rectangle.hpp"




extern int  reporting_count;

#define report printf("%6d %s %s %d\n",reporting_count++,__FILE__,__func__,__LINE__)


enum class
JoiningKind
{
  none,
  upward,
  downward,
  to_left,
  to_right,

};


struct Doll;
struct Renderer;


constexpr int  scale_level_max = 8;


constexpr int
get_scaled_value(int  src, int  lv)
{
  constexpr int  shift_amount = 22;

  return (((src<<shift_amount)/scale_level_max*lv)>>shift_amount);
}


constexpr int
get_reverse_scaled_value(int  src, int  lv)
{
  constexpr int  shift_amount = 22;

  return (((src<<shift_amount)/lv*scale_level_max)>>shift_amount);
}




class
Node
{
  static const double  sin_value_table[];
  static const double  cos_value_table[];

  const char*  name;

  int  z_value;

  JoiningKind  joining_kind;

  Doll*  doll;

  Node*                 parent;
  std::vector<Node*>  children;

  int  picture_index;//このノードが使用する画像を指すインデックス

  Rect  image_rect;//このノードが使用する描画元像画像領域

  Point  image_center;//描画元画像を回転処理するときの中心位置。値は、image_rectからの相対位置
  Point  graph_center;//描画先画像の中心位置。値は、仮想スクリーン上の絶対位置 -> マシンが計算する

  Point  base_offset;//親ノードのgraph_centerからの相対位置

  bool  angle_fixed;

  int    own_degree;//角度(度数法)
  int  total_degree;//合計角度。自身の角度と親ノードの合計角度を合わせたもの

  double           sin_value;
  double  reversed_sin_value;
  double           cos_value;
  double  reversed_cos_value;

public:
  Node(int  x=0, int  y=0);
  Node(const char*  name_, int  z, int  picture_index_, Rect&&  img_rect, Point&&  img_center_);
  Node(const Node&  rhs) noexcept;
 ~Node();


  Node&  operator=(const Node&  rhs) noexcept;

  void  clear();

  int  get_z_value() const;

  Doll*  get_doll() const;

  void  fix_angle();
  void  change_degree(int  v);

  Node*  join(Node*  child                                                       );
  Node*  join(Node*  child, int  x, int  y, JoiningKind  jk=JoiningKind::downward);

  void  change_doll(Doll&  doll_);

  void  change_base_offset(const Point&  pt);
  void  add_to_base_offset(const Point&  pt);

  const Point&  get_base_offset() const;
  const Point&  get_graph_center() const;

  void  change_angle(const Point&  pt, int  scale_level, JoiningKind  jk=JoiningKind::none);

  void  update(int  scale_level, bool  reversing);

  void  render_image(Renderer&  dst, const Point&  dst_offset, int  scale_level, bool  reversing);
  void  render(Renderer&  dst, const Point&  dst_offset, int  scale_level, bool  reversing, int  z_max);

  void  print() const;


  void     read(const int*&  it                 )      ;
  size_t  write(      int*&  it, const int*  end) const;

};




#endif




