#include"dolmo_scene.hpp"
#include"dolmo_frame.hpp"
#include"dolmo_doll.hpp"
#include"dolmo_dollState.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_model.hpp"




Scene::
Scene()
{
  frame_list.emplace_back(*this);
}


Scene::
Scene(const libjson::Value&  value)
{
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
  auto   it = doll_list.begin();
  auto  end = doll_list.end();

    while(it != end)
    {
        if(&*it == &target)
        {
            for(auto&  frame: frame_list)
            {
              frame.remove(target);
            }


          doll_list.erase(it);

          break;
        }


      ++it;
    }
}




void
Scene::
update_index()
{
  int  i = 0;

    for(auto&  frame: frame_list)
    {
      frame.change_index(i++);
    }
}


std::list<Frame>::iterator
Scene::
new_frame(std::list<Frame>::iterator  it)
{
  auto  res = frame_list.emplace(it,*this);

    for(auto&  doll: doll_list)
    {
      res->add(doll);
    }


  update_index();

  return res;
}


std::list<Frame>::iterator
Scene::
delete_frame(std::list<Frame>::iterator  it)
{
  auto  res = frame_list.erase(it);

  update_index();

  return res;
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




libjson::Value
Scene::
to_json() const
{
  libjson::Object  obj;

  obj.emplace_back(std::string("doll list"),libjson::Value(libjson::Array()));


  libjson::Array  arr;

/*
  auto   it = frame_list.cbegin();
  auto  end = frame_list.cend();

    if(it != end)
    {
//      arr.emplace_back(it++->to_json());

        while(it != end)
        {
//          arr.emplace_back(it++->to_json());
        }
    }
*/


  obj.emplace_back(std::string("frame list"),libjson::Value(std::move(arr)));


  return libjson::Value(std::move(obj));
}


void
Scene::
scan(const libjson::Value&  val)
{
    if(val == libjson::ValueKind::object)
    {
       doll_list.clear();
      frame_list.clear();

      const libjson::Array*  doll_list_arr = nullptr;

        for(auto&  member: val->object)
        {
             if(member.name == "doll number")
             {
               auto  n = member.value->number;

                 while(n--)
                 {
//                   doll_list.emplace_back(*this);
                 }
             }
        }
    }
}




