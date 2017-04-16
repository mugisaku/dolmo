#include"dolmo_rootmanager.hpp"
#include"dolmo_model.hpp"
#include"dolmo_screen.hpp"




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
      auto  root = raise_node();

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
      auto  root = raise_node();

      auto  it = current_root;

      root->update();

      root_list.emplace(++it,root);

      needed_to_redraw = true;
    }
}


void
RootManager::
copy_this()
{
    if(!copy_node)
    {
      copy_node = raise_node();
    }


  copy_node->reform(**current_root);
}


void
RootManager::
apply_copy()
{
    if(copy_node)
    {
      (*current_root)->reform(*copy_node);
      (*current_root)->update();

      needed_to_redraw = true;
    }
}


void
RootManager::
erase_this()
{
    if(root_list.size() > 1)
    {
      trash.emplace_back(*current_root);

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
  printf("%d,",root_list.size());

    for(auto  root: root_list)
    {
      root->fprint(stdout);
    }


  printf("\n");
}


void
RootManager::
start_to_animate()
{
  animation_flag = true;

  current_frame = root_list.cbegin();

  needed_to_redraw = true;
}




