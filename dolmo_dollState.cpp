#include"dolmo_dollState.hpp"
#include"dolmo_doll.hpp"
#include<cstring>




DollState::
DollState(Doll&  target_, Frame&  frame_):
target(target_),
frame(frame_),
number_count(0)
{
  load();
}




void
DollState::
load()
{
  base_offset = (*target).get_base_offset();

  int*  p = number_table;

  number_count = (*target).write(p,std::end(number_table));

  scale_level  = target.get_scale_level();
  reverse_flag = target.test_reverse_flag();
  z_value      = target.get_z_value();
}


void
DollState::
store() const
{
    if(number_count)
    {
      target.change_position(base_offset);

      const int*  p = number_table;

      (*target).read(p);

      target.scale_level  =  scale_level;
      target.reverse_flag = reverse_flag;
      target.z_value      =      z_value;


      target.update();
    }
}


void
DollState::
copy(const DollState&  src)
{
  std::memcpy(&number_table,&src.number_table,sizeof(*number_table)*src.number_count);

  number_count = src.number_count;

  base_offset  =  src.base_offset;
  scale_level  =  src.scale_level;
  z_value      =      src.z_value;
  reverse_flag = src.reverse_flag;
}




void
DollState::
scan(const libjson::Value&  val)
{
/*
  int  n;

  int  rev;

    if(sscanf(s," %d %d %d %d %n",&base_offset.x,&base_offset.y,&z_value,&rev,&number_count,&n) >= 4)
    {
      reverse_flag = rev;

      s += n;

        for(int  i = 0;  i < number_count;  ++i)
        {
            if(sscanf(s," %d %n",&number_table[i],&n) >= 1)
            {
              s += n;
            }

          else
            {
              break;
            }
        }
    }
*/
}


libjson::Value
DollState::
to_json() const
{
  libjson::Object  obj;

  obj.emplace_back(std::string("x"),libjson::Value(base_offset.x));
  obj.emplace_back(std::string("y"),libjson::Value(base_offset.y));
  obj.emplace_back(std::string("z value"),libjson::Value(z_value));
  obj.emplace_back(std::string("reversing"),libjson::Value(reverse_flag));

  libjson::Array  arr;

    for(int  i = 0;  i < number_count;  ++i)
    {
      arr.emplace_back(libjson::Value(number_table[i]));
    }


  obj.emplace_back(std::string("number table"),libjson::Value(std::move(arr)));

  return libjson::Value(std::move(obj));
}




