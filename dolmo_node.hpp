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

  Rect  image_rect;//このノードが使用する描画元像画像領域

  Point  image_center;//描画元画像を回転処理するときの中心位置。値は、image_rectからの相対位置
  Point  graph_center;//描画先画像の中心位置。値は、スクリーン上の絶対位置

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
  Node(const char*  name_, int  z, Rect&&  img_rect, Point&&  img_center_);
  Node(const Node&  rhs) noexcept;
 ~Node();


  Node&  operator=(const Node&  rhs) noexcept;

  void  clear();

  int  get_z_value() const;

  Doll*  get_doll() const;

  void  fix_angle();
  void  change_degree(int  v);

  void  reform(const Node&  rhs);

  Node*  join(Node*  child                                                       );
  Node*  join(Node*  child, int  x, int  y, JoiningKind  jk=JoiningKind::downward);

  void  change_doll(Doll&  doll_);

  void  change_base_offset(const Point&  pt);
  void  add_to_base_offset(const Point&  pt);

  const Point&  get_base_offset() const;
  const Point&  get_graph_center() const;

  void  change_angle(const Point&  pt);

  void  update();

  void  render_image(Renderer&  dst);
  void  render(Renderer&  dst, int  z_max);

  void  fprint(FILE*  f) const;
  const char*   sscan(const char*  s);

  void  print() const;


  template<typename  T>
  void  read(const T*&  it)
  {
    own_degree = *it++;

    T  n = *it++;

      if(n != children.size())
      {
        printf("子要素数が一致しません\n");

        throw;
      }


      for(auto  child: children)
      {
        child->read(it);
      }
  }


  template<typename  T>
  size_t  write(T*&  it, const T*  end) const
  {
    size_t  count = 0;

    *it++ = own_degree;

    ++count;

      if(it != end)
      {
        *it++ = children.size();

        ++count;

          if(it != end)
          {
              for(auto  child: children)
              {
                count += child->write(it,end);
              }
          }
      }


    return count;
  }

};




#endif




