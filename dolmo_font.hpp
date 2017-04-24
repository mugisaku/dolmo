#ifndef DOLMO_FONT_HPP_INCLUDED
#define DOLMO_FONT_HPP_INCLUDED


#include<cstdint>
#include<string>


struct
Glyph
{
  static constexpr int  size = 14;

  uint16_t  unicode;

  uint16_t  data[size];

  int  count;
};




const Glyph*  get_glyph(char16_t  c);

void  print_required_glyphes();


#endif




