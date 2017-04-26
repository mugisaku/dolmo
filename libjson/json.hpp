#ifndef JSON_HPP_INCLUDED
#define JSON_HPP_INCLUDED


#include<string>
#include<vector>
#include<cstdio>




namespace libjson{




enum class
ValueKind
{
  undefined,
  null,
  false_,
  true_,
  number,
  string,
  object,
  array,

};


struct Value;
struct ObjectMember;


using Object = std::vector<ObjectMember>;
using Array  = std::vector<Value>;


union
ValueData
{
  double       number;
  std::string  string;
  Object       object;
  Array         array;

   ValueData(){}
  ~ValueData(){}

};


struct Undefined{};
struct Null{};


struct
FilePath
{
  const char*  const string;

constexpr FilePath(const char*  s): string(s){}

};


class
Value
{
  ValueKind  kind;
  ValueData  data;

public:
   Value();
   Value(FilePath  path);
   Value(Null  null);
   Value(bool    b);
   Value(int     i);
   Value(size_t  sz);
   Value(double  f);
   Value(std::string&&  s);
   Value(Object&&  o);
   Value(Array&&  a);
   Value(const Value&   rhs) noexcept;
   Value(      Value&&  rhs) noexcept;
  ~Value();

  Value&  operator=(const Value&   rhs) noexcept;
  Value&  operator=(      Value&&  rhs) noexcept;

  const ValueData&  operator *() const;
  const ValueData*  operator->() const;

  bool  operator==(ValueKind  k) const;
  bool  operator!=(ValueKind  k) const;

  void  clear();

  ValueKind  get_kind() const;

  std::string  to_string(unsigned int  indent_n=0) const;

};


struct
ObjectMember
{
  std::string   name;

  Value  value;

  ObjectMember(std::string&&  name_=std::string(), Value&&  v=Value()):
  name( std::move(name_)),
  value(std::move(    v)){}

std::string  to_string(unsigned int  indent_n=0) const
{
  std::string  s;

  s += "\"";

  s += name;

  s += "\":";

  s += value.to_string(indent_n);

  return std::move(s);
}


};


}




#endif




