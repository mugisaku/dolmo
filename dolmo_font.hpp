#ifndef DOLMO_FONT_HPP_INCLUDED
#define DOLMO_FONT_HPP_INCLUDED


#include<cstdint>


struct
Glyph
{
  static constexpr int  size = 8;

  uint8_t  data[size];

};




const Glyph&  get_glyph(char  c);




#endif




