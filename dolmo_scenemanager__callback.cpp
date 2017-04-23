#include"dolmo_scenemanager.hpp"
#include"dolmo_model.hpp"
#include"dolmo_screen.hpp"




void
SceneManager::
increase_z_max()
{
    if(z_max < z_max_max)
    {
      ++z_max;

      needed_to_redraw = true;
    }
}


void
SceneManager::
decrease_z_max()
{
    if(z_max)
    {
      --z_max;

      needed_to_redraw = true;
    }
}


void
SceneManager::
change_to_previous()
{
    if(current_index)
    {
      --current_index;
      --current_scene;

      needed_to_redraw = true;
    }
}


void
SceneManager::
change_to_next()
{
    if(current_index < (scene_list.size()-1))
    {
      ++current_index;
      ++current_scene;

      needed_to_redraw = true;
    }
}


void
SceneManager::
insert_new_to_previous()
{
    if(scene_list.size() < 98)
    {
      scene_list.emplace(current_scene,Scene());

      ++current_index;

      needed_to_redraw = true;
    }
}


void
SceneManager::
insert_new_to_next()
{
    if(scene_list.size() < 98)
    {
      auto  it = current_scene;

      scene_list.emplace(++it,Scene());

      needed_to_redraw = true;
    }
}


void
SceneManager::
copy_this()
{
    if(!copy_node)
    {
//      copy_node = new Scene;
    }


//  copy_node->reform(**current_scene);
}


void
SceneManager::
apply_copy()
{
    if(copy_node)
    {
//      (*current_scene)->reform(*copy_node);
      current_scene->update();

      needed_to_redraw = true;
    }
}


void
SceneManager::
erase_this()
{
    if(scene_list.size() > 1)
    {
      current_scene = scene_list.erase(current_scene);

        if(current_scene == scene_list.end())
        {
          --current_index;
          --current_scene;
        }


      needed_to_redraw = true;
    }
}


void
SceneManager::
start_to_animate()
{
  change_mode(Mode::animation);

  current_frame = scene_list.cbegin();
}




