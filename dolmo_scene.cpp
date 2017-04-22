#include"dolmo_scene.hpp"
#include"dolmo_doll.hpp"




Scene::
Scene()
{
}




void
Scene::
need_to_redraw()
{
  needed_to_redraw = true;
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


bool
Scene::
render(Renderer&  dst, bool  force)
{
    if(force || needed_to_redraw)
    {
      dst.clear();

        for(auto  doll: doll_list)
        {
          doll->render(dst,z_max);
        }


      needed_to_redraw = false;

      return true;
    }


  return false;
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




