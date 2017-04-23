#include"dolmo_scene.hpp"
#include"dolmo_frame.hpp"
#include"dolmo_doll.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_model.hpp"




Scene::
Scene()
{
  frame_list.emplace_back(*this);

  allocate_doll(screen::width/2,screen::width/2-32);
}




void
Scene::
clear()
{
   doll_list.clear();
  frame_list.clear();
}


Doll&
Scene::
allocate_doll(int  x, int  y)
{
  doll_list.emplace_back(*this,new Node(get_model()),0);

  auto&  bk = doll_list.back();

  bk.change_position(Point(x,y));

  bk.update();

    for(auto&  frame: frame_list)
    {
      frame.add(bk);
    }


  return bk;
}


void
Scene::
deallocate_doll(Doll&  target)
{
    for(auto&  doll: doll_list)
    {
        if(&doll == &target)
        {
            for(auto&  frame: frame_list)
            {
              frame.remove(doll);
            }


          return;
        }
    }
}


std::list<Frame>::iterator
Scene::
new_frame(std::list<Frame>::iterator  it)
{
  return frame_list.emplace(it,*this);
}


std::list<Frame>::iterator
Scene::
delete_frame(std::list<Frame>::iterator  it)
{
  return frame_list.erase(it);
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




