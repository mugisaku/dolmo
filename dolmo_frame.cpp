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
copy(const Frame&  src) const
{
    if(&scene != &src.scene)
    {
      printf("シーンが違います\n");

      report;

      throw;
    }


  auto  n = src.dollstate_list.size();

    if(n != dollstate_list.size())
    {
      printf("状態数が一致しません\n");

      report;

      throw;
    }


  auto  dst_it =     dollstate_list.cbegin();
  auto  src_it = src.dollstate_list.cbegin();

    while(n--)
    {
            DollState&  dst_st = **dst_it++;
      const DollState&  src_st = **src_it++;

        if(&*dst_st != &*src_st)
        {
          printf("ターゲットが一致しません\n");

          report;

          throw;
        }


      dst_st.copy(src_st);
    }
}


void
Frame::
raise() const
{
    for(auto&  st: dollstate_list)
    {
      st->store();
    }
}


void
Frame::
unraise() const
{
    for(auto&  st: dollstate_list)
    {
      st->load();
    }
}




