#include"dolmo_doll.hpp"




Doll::
Doll(Node*  root):
scene(nullptr),
reverse_flag(false),
z_value(0),
root_node(root)
{
    if(root_node)
    {
      root_node->change_doll(*this);
    }
}


Doll::
Doll(Scene*  scene_, Node*  root):
scene(scene_),
root_node(root)
{
}


Doll::
Doll(Doll&&  rhs) noexcept:
root_node(nullptr)
{
  *this = std::move(rhs);
}


Doll::
~Doll()
{
  delete root_node;
}




const Node&
Doll::
operator*() const
{
  return *root_node;
}


Doll&
Doll::
operator=(Doll&&  rhs) noexcept
{
  clear();

  scene = rhs.scene;

  std::swap(root_node,rhs.root_node);

  z_value      = rhs.z_value;
  reverse_flag = rhs.reverse_flag;
}




void
Doll::
clear()
{
  scene = nullptr;

  delete root_node          ;
         root_node = nullptr;
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




