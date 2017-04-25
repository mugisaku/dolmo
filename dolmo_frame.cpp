#include"dolmo_frame.hpp"
#include"dolmo_doll.hpp"
#include"dolmo_dollState.hpp"
#include"dolmo_model.hpp"




Frame::
Frame(Scene*  scene_):
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
        if(&**it == &doll)
        {
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
//  dollstate_list.sort([](const DollState&  a, const DollState&  b){return(a->get_z_value() < b->get_z_value());});

    for(auto&  st: dollstate_list)
    {
      st.store();
    }
}


void
Frame::
unraise()
{
    for(auto&  st: dollstate_list)
    {
      st.load();
    }
}




