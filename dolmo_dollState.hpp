#ifndef DOLMO_DOLLSTATE_HPP_INCLUDED
#define DOLMO_DOLLSTATE_HPP_INCLUDED


#include<cstddef>
#include<cstdio>
#include"dolmo_rectangle.hpp"
#include"json.hpp"


struct Doll;
struct Frame;


class
DollState
{
  Doll&   target;
  Frame&   frame;

  Point  offset;

  int  number_table[400];

  size_t  number_count;

  int  scale_level;

  bool  reverse_flag;

  int  z_value;

public:
  DollState(Doll&  target_, Frame&  frame_);

  Doll&  operator*()  const{return  target;}
  Doll*  operator->() const{return &target;}

  void   load()      ;
  void  store() const;

  void  copy(const DollState&  src);

  void  scan(const libjson::Value&  val);
  libjson::Value  to_json() const;

};


#endif




