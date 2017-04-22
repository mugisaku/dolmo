#include"dolmo_doll.hpp"




Doll::
Doll(Scene*  scn):
scene(scn),
reverse_flag(false),
z_value(0),
root_node(nullptr)
{
}




int
Doll::
get_z_value() const
{
  return z_value;
}


void
Doll::
render(Renderer&  dst, int  z_max) const
{
  root_node->render(dst,z_max);
}




