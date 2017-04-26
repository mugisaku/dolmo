#include"json_stream.hpp"
#include"json.hpp"
#include<cctype>
#include<cstring>
#include<cstdlib>




namespace libjson{




Stream::
Stream(const char*  p):
base_pointer(p),
current_pointer(p),
row_number(0)
{
}




Value
Stream::
get_value()
{
  skip_spaces();

  auto  c = *current_pointer;

    if(c == 'n')
    {
        if(scan_identifier("null"))
        {
          return Value(Null());
        }
    }

  else
    if(c == 't')
    {
        if(scan_identifier("true"))
        {
          return Value(true);
        }
    }

  else
    if(c == 'f')
    {
        if(scan_identifier("false"))
        {
          return Value(false);
        }
    }

  else
    if(c == '{')
    {
      current_pointer += 1;

      return Value(scan_object());
    }

  else
    if(c == '[')
    {
      current_pointer += 1;

      return Value(scan_array());
    }

  else
    if(c == '\"')
    {
      current_pointer += 1;

      return Value(scan_string());
    }

  else
    if((c == '-') ||
       isdigit(c))
    {
      return Value(scan_number());
    }


  return Value();
}


void
Stream::
skip_spaces()
{
    for(;;)
    {
      auto  c = *current_pointer;

        if(c == '\n')
        {
                         current_pointer += 1;
          base_pointer = current_pointer     ;

          row_number += 1;
        }

      else
        if((c ==  ' ') ||
           (c == '\t') ||
           (c == '\r') ||
           (c == '\v'))
        {
          current_pointer += 1;
        }

      else
        {
          break;
        }
    }
}


struct char_is_not_utf8_head{};
struct char_is_not_utf8_data{};


size_t
Stream::
get_utf8_byte_number(int  c)
{
       if((c&0b10000000) == 0b00000000){return 1;}
  else if((c&0b11000000) == 0b10000000){throw char_is_not_utf8_head();}
  else if((c&0b11100000) == 0b11000000){return 2;}
  else if((c&0b11110000) == 0b11100000){return 3;}
  else if((c&0b11111000) == 0b11110000){return 4;}
  else if((c&0b11111100) == 0b11111000){return 5;}
  else if((c&0b11111110) == 0b11111100){return 6;}


  throw char_is_not_utf8_data();
}


void
Stream::
print() const
{
  auto  p = base_pointer;

  int  column_number = 0;

    while(p != current_pointer)
    {
      column_number += 1;

      int  n = get_utf8_byte_number(*p);

        while(n--)
        {
          fputc(*p++,stdout);
        }
    }


  printf("\n[行%4d:列%4d]\n",row_number,column_number);
}




}




