#ifndef DOLMO_FONT_HPP_INCLUDED
#define DOLMO_FONT_HPP_INCLUDED


#include<cstdint>
#include<string>


struct
Glyph
{
  static constexpr int  size = 14;

  uint16_t  data[size];

};




const Glyph*  get_glyph(char16_t  c);




#endif




