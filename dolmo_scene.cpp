#include"dolmo_scene.hpp"
#include"dolmo_doll.hpp"
#include"dolmo_model.hpp"




Scene::
Scene()
{
  join(new Doll(new Node(get_model())));

  update();
}


Scene::
~Scene()
{
  clear();
}




void
Scene::
clear()
{
    for(auto  doll: doll_list)
    {
      delete doll;
    }


  doll_list.clear();
}


Doll*
Scene::
join(Doll*  doll)
{
  doll->scene = this;

  doll_list.emplace_back(doll);

  return doll;
}


void
Scene::
update()
{
  doll_list.sort([](const Doll*  a, const Doll*  b){return(a->get_z_value() < b->get_z_value());});

    for(auto  doll: doll_list)
    {
      doll->update();
    }
}


void
Scene::
render(Renderer&  dst, int  z_max)
{
    for(auto  doll: doll_list)
    {
      doll->render(dst,z_max);
    }
}


void
Scene::
fprint(FILE*  f) const
{
    for(auto  doll: doll_list)
    {
      doll->fprint(f);
    }
}


const char*
Scene::
sscan(const char*  s)
{
    for(auto  doll: doll_list)
    {
      s = doll->sscan(s);
    }
}




