#include"dolmo_frame.hpp"
#include"dolmo_doll.hpp"
#include"dolmo_model.hpp"




Frame::
Frame(Scene&  scene_):
scene(scene_)
{
}




void
Frame::
add(Doll&  doll)
{
  dollstate_list.emplace_back(&doll);
}


void
Frame::
remove(Doll&  doll)
{
  auto   it = dollstate_list.begin();
  auto  end = dollstate_list.end();

    while(it != end)
    {
        if(it->target == &doll)
        {
          dollstate_list.erase(it);

          break;
        }


      ++it;
    }
}


void
Frame::
update()
{
  dollstate_list.sort([](const DollState&  a, const DollState&  b){return(a->get_z_value() < b->get_z_value());});

    for(auto&  st: dollstate_list)
    {
      st->update();
    }
}


void
Frame::
render(Renderer&  dst, int  z_max)
{
    for(auto&  st: dollstate_list)
    {
      st->render(dst,z_max);
    }
}


void
Frame::
fprint(FILE*  f) const
{
    for(auto&  st: dollstate_list)
    {
      st->fprint(f);
    }
}


const char*
Frame::
sscan(const char*  s)
{
    for(auto&  st: dollstate_list)
    {
      s = st->sscan(s);
    }
}




