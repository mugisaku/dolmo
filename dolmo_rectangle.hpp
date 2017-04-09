#ifndef DOLMO_RECTANGLE_HPP_INCLUDED
#define DOLMO_RECTANGLE_HPP_INCLUDED




struct
Point
{
  int  x;
  int  y;

  constexpr Point(int  x_=0, int  y_=0):
  x(x_), y(y_){}


  Point  transform(double  radian, const Point&  center) const;

  void  assign(int  x_, int  y_);

  Point  operator+(const Point&  rhs) const;
  Point  operator-(const Point&  rhs) const;

  Point&  operator+=(const Point&  rhs);
  Point&  operator-=(const Point&  rhs);

  void  print(const char*  s="") const;

};


struct
Rect: public Point
{
  int  w;
  int  h;

  constexpr Rect(int  x_=0, int  y_=0, int  w_=0, int  h_=0):
  Point(x_,y_), w(w_), h(h_){}

};




#endif




