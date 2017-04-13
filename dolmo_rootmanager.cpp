#include"dolmo_rootmanager.hpp"
#include"dolmo_model.hpp"
#include"dolmo_screen.hpp"
#include<vector>




RootManager::
RootManager():
z_max(z_max_max),
last_time(0),
copy_node(nullptr),
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


std::vector<Node*>
RootManager::
trash;


Node*
RootManager::
raise_node()
{
  Node*  ptr;

    if(trash.size())
    {
      ptr = trash.back();

      trash.pop_back();
    }

  else
    {
      ptr = new Node(get_model());
    }


  return ptr;
}


void
RootManager::
load(const char*  s)
{
    if(animation_flag)
    {
      return;
    }


  int  d;
  int  n;

    if(sscanf(s," %d , %n",&d,&n) >= 1)
    {
        if(d >= 1)
        {
          s += n;

            for(auto  root: root_list)
            {
              trash.emplace_back(root);
            }


          root_list.clear();

            while(d--)
            {
              auto  root = raise_node();

              s = root->sscan(s);

              root->update();

              root_list.emplace_back(root);
            }


          current_node = nullptr;
          current_root = root_list.begin();

          needed_to_redraw = true;
        }
    }
}


void
RootManager::
render(bool  force)
{
    if(force || needed_to_redraw)
    {
      auto  root = animation_flag? current_frame:current_root;

      screen::clear();

      (*root)->render(z_max);


      screen::put(current_index+1,root_list.size(),0,0);

      screen::update(!animation_flag);

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




