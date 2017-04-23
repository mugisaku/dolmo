#include"dolmo_scenemanager.hpp"
#include"dolmo_frame.hpp"
#include"dolmo_doll.hpp"
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
mode(Mode::move_position),
needed_to_redraw(true)
{
  scene_list.emplace_back();

  current_scene = scene_list.begin();

  current_scene->update();

  edition_frame = (*current_scene)->begin();
}




std::pair<int,int>
SceneManager::
get_numbers() const
{
  return std::make_pair(current_index,scene_list.size());
}


const Doll*
SceneManager::
get_current_doll() const
{
  return current_node? current_node->get_doll():nullptr;
}


Mode
SceneManager::
get_mode() const
{
  return mode;
}


void
SceneManager::
change_mode(Mode  m)
{
  mode = m;

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
    if(mode != Mode::animation)
    {
      current_node = renderer.get_cell(x,y).nodeptr;

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

    for(auto&  scene: scene_list)
    {
      scene.fprint(f);

      fprintf(f,"\n");
    }


  fprintf(f,"\n");
}


const char*
SceneManager::
sscan(const char*  s)
{
    if(mode == Mode::animation)
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


      auto  frame = (mode == Mode::animation)? animation_frame
                                             :   edition_frame;

      frame->render(dst,z_max);


      needed_to_redraw = false;

      return true;
    }


  return false;
}


void
SceneManager::
step()
{
    if(mode == Mode::animation)
    {
      auto  now = SDL_GetTicks();

        if(now >= (last_time+200))
        {
          last_time = now;

            if(++animation_frame == (*current_scene)->end())
            {
              animation_frame = (*current_scene)->begin();
            }


          needed_to_redraw = true;
        }
    }

  else
    if(mode == Mode::move_position)
    {
        if(current_node)
        {
            if((current_point.x != previous_point.x) ||
               (current_point.y != previous_point.y))
            {
              auto  doll = current_node->get_doll();

              auto  pt = current_point-current_node->get_graph_center();

              doll->add_to_position(pt);

              doll->update();

              previous_point = current_point;

              needed_to_redraw = true;
            }
        }
    }

  else
    if(mode == Mode::change_angle)
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




