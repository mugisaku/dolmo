#ifndef JSON_STREAM_HPP_INCLUDED
#define JSON_STREAM_HPP_INCLUDED


#include"json.hpp"




namespace libjson{




class
Stream
{
  const char*     base_pointer;
  const char*  current_pointer;

  int  row_number;

  static size_t  get_utf8_byte_number(int  c);

  void  skip_spaces();

  int   scan_hexadecimal_digit();
  int   scan_4hexadecimal_digits();
  bool  scan_identifier(const char*  s);

  double        scan_number();
  std::string   scan_string();
  Object        scan_object();
  ObjectMember  scan_objectmember();
  Array         scan_array();

public:
  Stream(const char*  p);

  Value  get_value();

  void  print() const;

};


}


#endif




