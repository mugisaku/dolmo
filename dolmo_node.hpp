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


struct Node;

constexpr int  circle_radius = 12;
constexpr int  square_size   = 16;


constexpr double  pi = 3.14159265358979323846264338327950288;


struct
Node
{
  static bool  needed_to_redraw;

  const char*  name;

  int  z_value;

  Node*                 parent;
  std::vector<Node*>  children;

  Rect  image_rect;//このノードが使用する描画元像画像領域

  Point  image_center;//描画元画像を回転処理するときの中心位置。値は、image_rectからの相対位置
  Point  graph_center;//描画先画像の中心位置。値は、スクリーン上の絶対位置

  Point  base_offset;//親ノードからの相対位置

  double    own_radian;//角度
  double  total_radian;//合計角度。自身の角度と親ノードの合計角度を合わせたもの


  Node(int  x, int  y);
  Node(const char*  name_, int  z, Rect&&  img_rect, Point&&  img_center_);


  Node*  join(Node*  child, int  x, int  y);

  void  change_angle(int  x, int  y);

  void  update();

  void  render_center();
  void  render_image();
  void  render();

  const char*   sscan(const char*  s);
  void  fprint(FILE*  f) const;

  void  print() const;

};




#endif




