#include"dolmo_scene.hpp"
#include"dolmo_frame.hpp"
#include"dolmo_doll.hpp"
#include"dolmo_model.hpp"




Scene::
Scene()
{
  frame_list.emplace_back(*this);
}




void
Scene::
clear()
{
   doll_list.clear();
  frame_list.clear();
}


Doll*
Scene::
join(Doll*  doll)
{
//  doll_list.emplace_back();

  return doll;
}


void
Scene::
update()
{
    for(auto&  doll: doll_list)
    {
      doll.update();
    }
}


void
Scene::
render(Renderer&  dst, int  z_max)
{
    for(auto&  doll: doll_list)
    {
      doll.render(dst,z_max);
    }
}


void
Scene::
fprint(FILE*  f) const
{
    for(auto&  doll: doll_list)
    {
      doll.fprint(f);
    }
}


const char*
Scene::
sscan(const char*  s)
{
    for(auto&  doll: doll_list)
    {
      s = doll.sscan(s);
    }
}




