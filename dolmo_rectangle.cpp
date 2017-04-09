#include"dolmo_rectangle.hpp"
#include<cmath>
#include<cstdio>




Point
Point::
transform(double  radian, const Point&  center) const
{
  Point  res;

  auto  sin_value = std::sin(radian);
  auto  cos_value = std::cos(radian);

  res.x = ((x-center.x)*cos_value-(y-center.y)*sin_value+center.x);
  res.y = ((x-center.x)*sin_value+(y-center.y)*cos_value+center.y);

  return res;
}




void
Point::
assign(int  x_, int  y_)
{
  x = x_;
  y = y_;
}


Point
Point::
operator+(const Point&  rhs) const
{
  return Point(x+rhs.x,y+rhs.y);
}


Point
Point::
operator-(const Point&  rhs) const
{
  return Point(x-rhs.x,y-rhs.y);
}


Point&
Point::
operator+=(const Point&  rhs)
{
  x += rhs.x;
  y += rhs.y;

  return *this;
}


Point&
Point::
operator-=(const Point&  rhs)
{
  x -= rhs.x;
  y -= rhs.y;

  return *this;
}


void
Point::
print(const char*  s) const
{
  printf("[print %s point] %4d %4d\n",s,x,y);
}




