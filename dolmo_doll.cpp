#include"dolmo_doll.hpp"




Doll::
Doll(Scene&  scene_, Node*  root, int  z, bool  rev):
scene(scene_),
reverse_flag(rev),
z_value(z),
root_node(root)
{
    if(root_node)
    {
      root_node->change_doll(*this);
    }
}




const Node&
Doll::
operator*() const
{
  return *root_node;
}




int
Doll::
get_z_value() const
{
  return z_value;
}


const Node&
Doll::
get_root_node() const
{
  return *root_node;
}


void
Doll::
change_position(const Point&  pt)
{
  root_node->change_base_offset(pt);
}


void
Doll::
add_to_position(const Point&  pt)
{
  root_node->add_to_base_offset(pt);
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




