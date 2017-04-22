#include"dolmo_scenemanager.hpp"
#include"dolmo_model.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_renderer.hpp"
#include<vector>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif




SceneManager::
SceneManager():
z_max(z_max_max),
current_index(0),
last_time(0),
copy_node(nullptr),
needed_to_redraw(true)
{
  scene_list.emplace_back(new Scene);

  current_scene = scene_list.begin();

  (*current_scene)->update();
}




std::pair<int,int>
SceneManager::
get_numbers() const
{
  return std::make_pair(current_index,scene_list.size());
}


bool
SceneManager::
test_animation_flag() const
{
  return animation_flag;
}


void
SceneManager::
unset_animation_flag()
{
  animation_flag = false;

  needed_to_redraw = true;
}


void
SceneManager::
move_pointer(int  x, int  y)
{
  current_point.assign(x,y);
}


void
SceneManager::
press(Renderer&  renderer, int  x, int  y)
{
    if(!animation_flag)
    {
      current_node = renderer.get_cell(screen::width-1-x,y).nodeptr;

                       current_point.assign(x,y);
      previous_point = current_point            ;
    }
}


void
SceneManager::
unpress()
{
  current_node = nullptr;
}


void
SceneManager::
fprint(FILE*  f) const
{
  fprintf(f,"%d,\n",scene_list.size());

    for(auto  scene: scene_list)
    {
      scene->fprint(f);

      fprintf(f,"\n");
    }


  fprintf(f,"\n");
}


const char*
SceneManager::
sscan(const char*  s)
{
    if(animation_flag)
    {
      return nullptr;
    }


  int  d;
  int  n;

    if(sscanf(s," %d , %n",&d,&n) >= 1)
    {
        if(d >= 1)
        {
          s += n;


          current_node = nullptr;
          current_scene = scene_list.begin();

          current_index = 0;

          needed_to_redraw = true;
        }
    }


  return nullptr;
}


bool
SceneManager::
render(Renderer&  dst, bool  force)
{
    if(needed_to_redraw || force)
    {
      dst.clear();

      (*current_scene)->render(dst,z_max);


      needed_to_redraw = false;

      return true;
    }


  return false;
}


void
SceneManager::
step()
{
    if(animation_flag)
    {
      auto  now = SDL_GetTicks();

        if(now >= (last_time+200))
        {
          last_time = now;

            if(++current_frame == scene_list.cend())
            {
              current_frame = scene_list.cbegin();
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




