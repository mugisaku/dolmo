#include"dolmo_frame.hpp"
#include"dolmo_doll.hpp"
#include"dolmo_dollState.hpp"
#include"dolmo_model.hpp"




Frame::
Frame(Scene&  scene_):
scene(scene_)
{
}


Frame::
~Frame()
{
    for(auto&  it: dollstate_list)
    {
      (**it).delete_state(it);
    }
}




void
Frame::
change_index(uint32_t  i)
{
  index = i;
}


void
Frame::
add(Doll&  doll)
{
  auto  it = doll.new_state(*this);

  dollstate_list.emplace_back(it);
}


void
Frame::
remove(Doll&  doll)
{
  auto   it = dollstate_list.begin();
  auto  end = dollstate_list.end();

    while(it != end)
    {
        if(&***it == &doll)
        {
          doll.delete_state(*it);

          dollstate_list.erase(it);

          break;
        }


      ++it;
    }
}


void
Frame::
raise()
{
    for(auto&  st: dollstate_list)
    {
      st->store();
    }
}


void
Frame::
unraise()
{
    for(auto&  st: dollstate_list)
    {
      st->load();
    }
}




