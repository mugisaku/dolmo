#include"dolmo_doll.hpp"




Doll::
Doll(Scene&  scene_, Node*  root, int  z, bool  rev):
scene(scene_),
scale_level(scale_level_max),
reverse_flag(rev),
z_value(z),
root_node(root)
{
    if(root_node)
    {
      root_node->change_doll(*this);
    }
}


Doll::
Doll(Scene&  scene_, const libjson::Value&  value):
scene(scene_)
{
}




Node&
Doll::
operator*()
{
  return *root_node;
}


const Node&
Doll::
operator*() const
{
  return *root_node;
}




bool
Doll::
test_reverse_flag() const
{
  return reverse_flag;
}


void
Doll::
switch_reverse_flag()
{
  reverse_flag = !reverse_flag;
}


void
Doll::
increase_scale_level()
{
    if(scale_level < scale_level_max)
    {
      ++scale_level;
    }
}


void
Doll::
decrease_scale_level()
{
    if(scale_level > 1)
    {
      --scale_level;
    }
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
  offset = pt;
}


void
Doll::
add_to_position(const Point&  pt)
{
  offset += pt;
}


void
Doll::
update()
{
  root_node->update(scale_level,reverse_flag);
}




DollStateIterator
Doll::
new_state(Frame&  frame)
{
  return state_list.emplace(state_list.cend(),*this,frame);
}


void
Doll::
delete_state(DollStateIterator  it)
{
  state_list.erase(it);
}




void
Doll::
render(Renderer&  dst, int  z_max) const
{
  root_node->render(dst,offset,scale_level,reverse_flag,z_max);
}




void
Doll::
scan(const libjson::Value&  val)
{
}


libjson::Value
Doll::
to_json() const
{
  libjson::Object  obj;


  libjson::Array  arr;

    for(auto&  st: state_list)
    {
      arr.emplace_back(st->to_json());
    }


  obj.emplace_back(std::string("state list"),std::move(arr));

  return libjson::Value(std::move(obj));
}




