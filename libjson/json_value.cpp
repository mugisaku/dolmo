#include"json.hpp"
#include"json_stream.hpp"
#include<cctype>
#include<cstring>
#include<cstdlib>
#include<new>




namespace libjson{


namespace{
std::string
load(const char*  path)
{
  std::string  s;

  auto  f = fopen(path,"rb");

    if(f)
    {
        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          s.push_back(c);
        }


      fclose(f);
    }


  return std::move(s);
}
}


Value::Value(): kind(ValueKind::undefined){}
Value::Value(FilePath  path): kind(ValueKind::undefined){*this = Stream(load(path.string).data()).get_value();}
Value::Value(Null  null): kind(ValueKind::null){}
Value::Value(bool    b): kind(b? ValueKind::true_:ValueKind::false_){}
Value::Value(int     i): kind(ValueKind::number){data.number = i;}
Value::Value(size_t sz): kind(ValueKind::number){data.number = sz;}
Value::Value(double  f): kind(ValueKind::number){data.number = f;}
Value::Value(std::string&&  s): kind(ValueKind::string){new(&data.string) std::string(std::move(s));}
Value::Value(Object&&       o): kind(ValueKind::object){new(&data.object) Object(std::move(o));}
Value::Value(Array&&        a): kind(ValueKind::array ){new(&data.array ) Array(std::move(a));}


Value::
Value(const Value&   rhs) noexcept:
kind(ValueKind::null)
{
  *this = rhs;
}


Value::
Value(Value&&  rhs) noexcept:
kind(ValueKind::null)
{
  *this = std::move(rhs);
}


Value::
~Value()
{
  clear();
}




Value&
Value::
operator=(const Value&  rhs) noexcept
{
  clear();

  kind = rhs.kind;

    switch(kind)
    {
  case(ValueKind::undefined): break;
  case(ValueKind::null  ): break;
  case(ValueKind::true_ ): break;
  case(ValueKind::false_): break;
  case(ValueKind::number): data.number = rhs.data.number;break;
  case(ValueKind::string): new(&data.string) std::string(rhs.data.string);break;
  case(ValueKind::object): new(&data.object) Object(rhs.data.object);break;
  case(ValueKind::array ): new(&data.array)  Array(rhs.data.array);break;
    }


  return *this;
}


Value&
Value::
operator=(Value&&  rhs) noexcept
{
  clear();

  kind = rhs.kind                  ;
         rhs.kind = ValueKind::null;

    switch(kind)
    {
  case(ValueKind::undefined): break;
  case(ValueKind::null  ): break;
  case(ValueKind::true_ ): break;
  case(ValueKind::false_): break;
  case(ValueKind::number): data.number = std::move(rhs.data.number);break;
  case(ValueKind::string): new(&data.string) std::string(std::move(rhs.data.string));break;
  case(ValueKind::object): new(&data.object) Object(std::move(rhs.data.object));break;
  case(ValueKind::array ): new(&data.array ) Array(std::move(rhs.data.array ));break;
    }


  return *this;
}


const ValueData&  Value::operator *() const{return  data;}
const ValueData*  Value::operator->() const{return &data;}


bool  Value::operator==(ValueKind  k) const{return(kind == k);}
bool  Value::operator!=(ValueKind  k) const{return(kind != k);}


void
Value::
clear()
{
    switch(kind)
    {
  case(ValueKind::undefined): break;
  case(ValueKind::null  ): break;
  case(ValueKind::true_ ): break;
  case(ValueKind::false_): break;
  case(ValueKind::string): data.string.~basic_string();break;
  case(ValueKind::object): data.object.~vector();break;
  case(ValueKind::number): break;
  case(ValueKind::array ): data.array.~vector();break;
    }


  kind = ValueKind::undefined;
}


ValueKind
Value::
get_kind() const
{
  return kind;
}


std::string
Value::
to_string(unsigned int  indent_n) const
{
  constexpr int  add_value = 4;

  indent_n += add_value;

  std::string  s;

    switch(kind)
    {
  case(ValueKind::undefined): s = "undefined";break;
  case(ValueKind::null  ): s = "null";break;
  case(ValueKind::true_ ): s = "true";break;
  case(ValueKind::false_): s = "false";break;
  case(ValueKind::number): s = std::to_string(data.number);break;
  case(ValueKind::string):
      s += '\"';
      s += data.string;
      s += '\"';
      break;
  case(ValueKind::object):
    {
      s += "{\n";

      auto         it = data.object.cbegin();
      auto  const end = data.object.cend();

        if(it != end)
        {
          s.append(indent_n,' ');

          s += it++->to_string(indent_n);

            while(it != end)
            {
              s += ",\n";

              s.append(indent_n,' ');

              s += it++->to_string(indent_n);
            }


          s += "\n";

          s.append(indent_n-add_value,' ');
        }

      s += "}";
    } break;
  case(ValueKind::array):
    {
      s += "[\n";

      auto         it = data.array.cbegin();
      auto  const end = data.array.cend();

        if(it != end)
        {
          s.append(indent_n,' ');

          s += it++->to_string(indent_n);

            while(it != end)
            {
              s += ",\n";

              s.append(indent_n,' ');

              s += it++->to_string(indent_n);
            }


          s += "\n";

          s.append(indent_n-add_value,' ');
        }

      s += "]";
    } break;
    }


  return std::move(s);
}




}




