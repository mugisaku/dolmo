#ifndef DOLMO_DOLLSTATE_HPP_INCLUDED
#define DOLMO_DOLLSTATE_HPP_INCLUDED


#include<cstddef>
#include"dolmo_rectangle.hpp"


struct Doll;


class
DollState
{
  Doll*  target;

  Point  base_offset;

  int  number_table[400];

  size_t  number_count;

  bool  reverse_flag;

  int  z_value;

public:
  DollState(Doll*  target_=nullptr): target(target_){if(target){load();}}

  Doll&  operator*()  const{return *target;}
  Doll*  operator->() const{return  target;}

  void   load()      ;
  void  store() const;

};


#endif




