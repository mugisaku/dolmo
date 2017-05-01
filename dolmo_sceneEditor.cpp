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
mode(Mode::allocate_human_doll),
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


void
SceneEditor::
change_mode(Mode  m)
{
    if(mode == Mode::animation)
    {
      current_frame->raise();
    }


  mode = m;

  current_node = nullptr;

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
    if(mode == Mode::allocate_human_doll)
    {
      target->allocate_doll(get_human_model(),x,y);

      needed_to_redraw = true;
    }

  else
    if(mode == Mode::allocate_horse_doll)
    {
      target->allocate_doll(get_horse_model(),x,y);

      needed_to_redraw = true;
    }

  else
    if(mode == Mode::remove_doll)
    {
      current_node = renderer.get_cell(x,y).nodeptr;

        if(current_node)
        {
          auto&  doll = *current_node->get_doll();

          target->deallocate_doll(doll);

          needed_to_redraw = true;
        }
    }

  else
    if(mode == Mode::increase_scale_level)
    {
      current_node = renderer.get_cell(x,y).nodeptr;

        if(current_node)
        {
          auto  doll = current_node->get_doll();

          doll->increase_scale_level();
          doll->update();

          needed_to_redraw = true;
        }
    }

  else
    if(mode == Mode::decrease_scale_level)
    {
      current_node = renderer.get_cell(x,y).nodeptr;

        if(current_node)
        {
          auto  doll = current_node->get_doll();

          doll->decrease_scale_level();
          doll->update();

          needed_to_redraw = true;
        }
    }

  else
    if(mode == Mode::switch_reversing)
    {
      current_node = renderer.get_cell(x,y).nodeptr;

        if(current_node)
        {
          auto  doll = current_node->get_doll();

          doll->switch_reverse_flag();
          doll->update();

          needed_to_redraw = true;
        }
    }

  else
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

      target->render(dst,z_max);

      needed_to_redraw = false;

      return true;
    }


  return false;
}


void
SceneEditor::
step()
{
    if((mode == Mode::animation) && ((**target).size() > 1))
    {
      auto  now = SDL_GetTicks();

        if(now >= (last_time+200))
        {
          last_time = now;

            if(++animation_frame == (*target)->end())
            {
              animation_frame = (*target)->begin();
            }


          animation_frame->raise();

          needed_to_redraw = true;
        }
    }

  else
    if(mode == Mode::move_angle)
    {
        if(current_node)
        {
            if((current_point.x != previous_point.x) ||
               (current_point.y != previous_point.y))
            {
              auto  doll = current_node->get_doll();

              current_node->change_angle(current_point,doll->get_scale_level());

              doll->update();

              previous_point = current_point;

              needed_to_redraw = true;
            }
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
}




