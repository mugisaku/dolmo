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

  Node*                 parent;
  std::vector<Node*>  children;

  Rect  image_rect;

  Point          raw_offset;//
  Point  transformed_offset;//
  Point          raw_center;//
  Point  transformed_center;//

  double          raw_radian;//角度
  double  transformed_radian;//合成角度。自身の角度と親ノードの合成角度を合わせたもの


  Node(Rect&&  rect, Point&&  offset_, Point&&  center_);


  void  join(Node*  child, int  x, int  y);

  void  change_angle(int  x, int  y);

  void  update();

  bool  scan(int  x, int  y);

  void  render_center();
  void  render_image();
  void  render();

};




#endif




