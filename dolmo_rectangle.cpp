#include"dolmo_rectangle.hpp"
#include<cmath>




void
Point::
transform(double  radian, const Point&  center)
{
  int  tx = x;
  int  ty = y;

  auto  sin_value = std::sin(radian);
  auto  cos_value = std::cos(radian);

  x = ((tx-center.x)*cos_value-(ty-center.y)*sin_value+center.x);
  y = ((tx-center.x)*sin_value+(ty-center.y)*cos_value+center.y);
}




