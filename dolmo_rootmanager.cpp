#include"dolmo_rootmanager.hpp"
#include"dolmo_model.hpp"
#include"dolmo_screen.hpp"
#include<vector>




RootManager::
RootManager():
z_max(z_max_max),
last_time(0),
needed_to_redraw(true)
{
  root_list.emplace_back(new Node(get_model()));

  current_root = root_list.begin();

  (*current_root)->update();
}




void
RootManager::
move_pointer(int  x, int  y)
{
    if(screen::touch_button(x,y,false))
    {
      needed_to_redraw = true;
    }


  current_point.assign(x,y);
}


void
RootManager::
press(int  x, int  y)
{
    if(animation_flag)
    {
      animation_flag = false;

      needed_to_redraw = true;
    }

  else
    {
        if(!screen::touch_button(x,y,true))
        {
          current_node = screen::get(x,y);

                           current_point.assign(x,y);
          previous_point = current_point            ;
        }
    }
}


void
RootManager::
unpress()
{
  current_node = nullptr;
}




void
RootManager::
load(const char*  s)
{
    if((*current_root)->sscan(s))
    {
      (*current_root)->update();
    }
}




void
RootManager::
render()
{
    if(needed_to_redraw)
    {
      auto  root = animation_flag? current_frame:current_root;

      screen::clear();

      (*root)->render(z_max);


      screen::put(current_index+1,root_list.size(),0,0);

      screen::update();

      needed_to_redraw = false;
    }
}


void
RootManager::
step()
{
    if(animation_flag)
    {
      auto  now = SDL_GetTicks();

        if(now >= (last_time+200))
        {
          last_time = now;

            if(++current_frame == root_list.cend())
            {
              current_frame = root_list.cbegin();
            }


          needed_to_redraw = true;
        }
    }

  else
    {
        if(current_node)
        {
            if((current_point.x != previous_point.x) ||
               (current_point.y != previous_point.y))
            {
              current_node->change_angle(current_point);

              previous_point = current_point;

              needed_to_redraw = true;
            }
        }
    }
}




