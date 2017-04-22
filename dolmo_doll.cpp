#include"dolmo_doll.hpp"




Doll::
Doll(Node*  root):
scene(nullptr),
reverse_flag(false),
z_value(0),
root_node(root)
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
update()
{
  root_node->update();
}


void
Doll::
render(Renderer&  dst, int  z_max) const
{
  root_node->render(dst,z_max);
}


void
Doll::
fprint(FILE*  f) const
{
  root_node->fprint(f);
}


const char*
Doll::
sscan(const char*  s)
{
  return root_node->sscan(s);
}




