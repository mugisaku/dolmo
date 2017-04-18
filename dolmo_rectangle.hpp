#ifndef DOLMO_RECTANGLE_HPP_INCLUDED
#define DOLMO_RECTANGLE_HPP_INCLUDED




constexpr double  pi = 3.14159265358979323846264338327950288;


struct
Point
{
  int  x;
  int  y;

  constexpr Point(int  x_=0, int  y_=0):
  x(x_), y(y_){}


  Point  operator+(const Point&  rhs) const;
  Point  operator-(const Point&  rhs) const;

  Point&  operator+=(const Point&  rhs);
  Point&  operator-=(const Point&  rhs);

  Point  transform(double  radian, const Point&  center) const;
  Point  transform(double  sin_value, double  cos_value, const Point&  center) const;

  void  assign(int  x_, int  y_);

  void  print(const char*  s="") const;

};


struct
Rect: public Point
{
  int  w;
  int  h;

  constexpr Rect(int  x_=0, int  y_=0, int  w_=0, int  h_=0):
  Point(x_,y_), w(w_), h(h_){}

  constexpr bool  test(const Point&  pt) const
  {
    return((pt.x >= (x  )) &&
           (pt.y >= (y  )) &&
           (pt.x <  (x+w)) &&
           (pt.y <  (y+h)));
  }

};




#endif




