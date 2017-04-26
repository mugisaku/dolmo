#include"json_stream.hpp"
#include"json.hpp"
#include<cctype>
#include<cstring>
#include<cstdlib>




namespace libjson{




double
Stream::
scan_number()
{
  int  n;

  double  d;

  auto  res = sscanf(current_pointer,"%lf%n",&d,&n);

    if(!res)
    {
      printf("Numberの読み込みに失敗\n");

      throw *this;
    }


  current_pointer += n;

  return d;
}


bool
Stream::
scan_identifier(const char*  s)
{
  auto  len = std::strlen(s);

    if(std::strncmp(current_pointer,s,len) == 0)
    {
      current_pointer += len;

      auto  c = *current_pointer;

        if(!isalnum(c) && (c != '_'))
        {
          return true;
        }
    }


  printf("識別子\"%s\"の読み込みに失敗\n",s);

  throw *this;

  return false;
}


ObjectMember
Stream::
scan_objectmember()
{
    if(*current_pointer != '\"')
    {
      printf("オブジェクトメンバーの名前の読み込みに失敗\n");

      throw *this;
    }


  current_pointer += 1;


  ObjectMember  m;

  m.name = scan_string();

    if(*current_pointer != ':')
    {
      printf("オブジェクトメンバー\"%s\"の値の読み込みに失敗\n",m.name.data());

      throw *this;
    }


  current_pointer += 1;

  m.value = get_value();

  return std::move(m);
}


Object
Stream::
scan_object()
{
  Object  obj;

    for(;;)
    {
      skip_spaces();

        if(*current_pointer == '}')
        {
          current_pointer += 1;

          break;
        }


      obj.emplace_back(scan_objectmember());

        if(*current_pointer == ',')
        {
          current_pointer += 1;
        }
    }


  skip_spaces();

  return std::move(obj);
}




int
Stream::
scan_hexadecimal_digit()
{
  auto  c = *current_pointer;

    if(!isxdigit(c))
    {
      printf("16進数ではない文字の出現\n");

      throw *this;
    }


  int  i;

    switch(c)
    {
  case('0'): i = 0;break;
  case('1'): i = 1;break;
  case('2'): i = 2;break;
  case('3'): i = 3;break;
  case('4'): i = 4;break;
  case('5'): i = 5;break;
  case('6'): i = 6;break;
  case('7'): i = 7;break;
  case('8'): i = 8;break;
  case('9'): i = 9;break;
  case('a'):
  case('A'): i = 10;break;
  case('b'):
  case('B'): i = 11;break;
  case('c'):
  case('C'): i = 12;break;
  case('d'):
  case('D'): i = 13;break;
  case('e'):
  case('E'): i = 14;break;
  case('f'):
  case('F'): i = 15;break;
    }


  current_pointer += 1;

  return i;
}


int
Stream::
scan_4hexadecimal_digits()
{
  unsigned int  u;

    try
    {
      u  = scan_hexadecimal_digit()<<12;
      u |= scan_hexadecimal_digit()<< 8;
      u |= scan_hexadecimal_digit()<< 4;
      u |= scan_hexadecimal_digit()    ;
    }


    catch(Stream&  str)
    {
      printf("ユニバーサル文字の読み込みに失敗\n");

      throw *this;
    }


  return u;
}


std::string
Stream::
scan_string()
{
  std::string  s;

    for(;;)
    {
      auto  c = *current_pointer;

        if(c == '\"')
        {
          current_pointer += 1;

          break;
        }

      else
        if(c == '\\')
        {
          current_pointer += 1;

            switch(*current_pointer)
            {
          case('\"'): s.push_back('\"');break;
          case('\\'): s.push_back('\\');break;
          case('/'):  s.push_back('/');break;
          case('b'):  s.push_back('\b');break;
          case('f'):  s.push_back('\f');break;
          case('n'):  s.push_back('\n');break;
          case('r'):  s.push_back('\r');break;
          case('t'):  s.push_back('\t');break;
          case('u'):
              current_pointer += 1;
              s.push_back(scan_4hexadecimal_digits());
              break;
          default:;
              printf("Stringの読み込み中、無効なエスケープ文字が出現\n");

              throw *this;
            }


          current_pointer += 1;
        }

      else
        if(iscntrl(c))
        {
          printf("Stringの読み込み中、制御文字が出現\n");

          throw *this;
        }

      else
        {
          current_pointer += 1;

          s.push_back(c);
        }
    }


  skip_spaces();

  return std::move(s);
}


Array
Stream::
scan_array()
{
  Array  arr;

    for(;;)
    {
      skip_spaces();

        if(*current_pointer == ']')
        {
          current_pointer += 1;

          break;
        }
        

      arr.emplace_back(get_value());

        if(*current_pointer == ',')
        {
          current_pointer += 1;
        }
    }


  skip_spaces();

  return std::move(arr);
}



}




