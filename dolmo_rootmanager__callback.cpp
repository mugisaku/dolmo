#include"dolmo_rootmanager.hpp"
#include"dolmo_model.hpp"
#include"dolmo_screen.hpp"
#include<vector>




namespace{


std::vector<Node*>
stock;


}


void
RootManager::
increase_z_max()
{
    if(z_max < z_max_max)
    {
      ++z_max;

      needed_to_redraw = true;
    }
}


void
RootManager::
decrease_z_max()
{
    if(z_max)
    {
      --z_max;

      needed_to_redraw = true;
    }
}


void
RootManager::
change_to_previous()
{
    if(current_index)
    {
      --current_index;

      --current_root;

      needed_to_redraw = true;
    }
}


void
RootManager::
change_to_next()
{
    if(current_index < (root_list.size()-1))
    {
      ++current_index;

      ++current_root;

      needed_to_redraw = true;
    }
}


void
RootManager::
insert_new_to_previous()
{
    if(root_list.size() < 98)
    {
      auto  root = new Node(get_model());

      root->update();

      root_list.emplace(current_root,root);

      ++current_index;

      needed_to_redraw = true;
    }
}


void
RootManager::
insert_new_to_next()
{
    if(root_list.size() < 98)
    {
      auto  root = new Node(get_model());

      auto  it = current_root;

      root->update();

      root_list.emplace(++it,root);

      needed_to_redraw = true;
    }
}


void
RootManager::
erase_this()
{
    if(root_list.size() > 1)
    {
      current_root = root_list.erase(current_root);

        if(current_root == root_list.end())
        {
          --current_index;
          --current_root;
        }


      needed_to_redraw = true;
    }
}


void
RootManager::
print()
{
}


void
RootManager::
start_to_animate()
{
  animation_flag = true;

  current_frame = root_list.cbegin();

  needed_to_redraw = true;
}




