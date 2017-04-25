#include"dolmo_dollState.hpp"
#include"dolmo_doll.hpp"




void
DollState::
load()
{
  base_offset = (**target).get_base_offset();

  int*  p = number_table;

  number_count = (**target).write(p,std::end(number_table));

  reverse_flag = target->test_reverse_flag();
  z_value      = target->get_z_value();
}


void
DollState::
store() const
{
    if(number_count)
    {
      target->change_position(base_offset);

      const int*  p = number_table;

      (**target).read(p);

      target->reverse_flag = reverse_flag;
      target->z_value      =     z_value;


      target->update();
    }
}




