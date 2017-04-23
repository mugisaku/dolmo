#include"dolmo_sceneEditor.hpp"
#include"dolmo_frame.hpp"
#include"dolmo_doll.hpp"
#include"dolmo_model.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_renderer.hpp"
#include<vector>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif




SceneEditor::
SceneEditor():
z_max(z_max_max),
current_index(0),
last_time(0),
copy_node(nullptr),
mode(Mode::move_position),
needed_to_redraw(true)
{
}




void
SceneEditor::
open(Scene&  new_target)
{
  target = &new_target;

  current_frame = (*target)->begin();
  current_index = 0;
}


std::pair<int,int>
SceneEditor::
get_numbers() const
{
  return std::make_pair(current_index,(*target)->size());
}


const Doll*
SceneEditor::
get_current_doll() const
{
  return current_node? current_node->get_doll():nullptr;
}


Mode
SceneEditor::
get_mode() const
{
  return mode;
}


void
SceneEditor::
change_mode(Mode  m)
{
  mode = m;

  needed_to_redraw = true;
}


void
SceneEditor::
move_pointer(int  x, int  y)
{
  current_point.assign(x,y);
}


void
SceneEditor::
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
SceneEditor::
unpress()
{
  current_node = nullptr;
}


bool
SceneEditor::
render(Renderer&  dst, bool  force)
{
    if(needed_to_redraw || force)
    {
      dst.clear();


      auto  frame = (mode == Mode::animation)? animation_frame
                                             :   current_frame;

      frame->render(dst,z_max);


      needed_to_redraw = false;

      return true;
    }


  return false;
}


void
SceneEditor::
step()
{
    if(mode == Mode::animation)
    {
      auto  now = SDL_GetTicks();

        if(now >= (last_time+200))
        {
          last_time = now;

            if(++animation_frame == (*target)->end())
            {
              animation_frame = (*target)->begin();
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




