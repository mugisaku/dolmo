#include"dolmo_sceneEditor.hpp"
#include"dolmo_doll.hpp"
#include"dolmo_model.hpp"
#include"dolmo_screen.hpp"




void
SceneEditor::
increase_z_max()
{
    if(z_max < z_max_max)
    {
      ++z_max;

      needed_to_redraw = true;
    }
}


void
SceneEditor::
decrease_z_max()
{
    if(z_max)
    {
      --z_max;

      needed_to_redraw = true;
    }
}


void
SceneEditor::
change_to_previous()
{
    if(current_index)
    {
      current_frame->unraise();

      --current_index;
      --current_frame;

      current_frame->raise();

      needed_to_redraw = true;
    }
}


void
SceneEditor::
change_to_next()
{
    if(current_index < ((*target)->size()-1))
    {
      current_frame->unraise();

      ++current_index;
      ++current_frame;

      current_frame->raise();

      needed_to_redraw = true;
    }
}


void
SceneEditor::
insert_new_to_previous()
{
    if((*target)->size() < 98)
    {
      target->new_frame(current_frame);

      ++current_index;

      target->update_index();

      needed_to_redraw = true;
    }
}


void
SceneEditor::
insert_new_to_next()
{
    if((*target)->size() < 98)
    {
      auto  it = current_frame;

      target->new_frame(++it);

      target->update_index();

      needed_to_redraw = true;
    }
}


void
SceneEditor::
copy_this()
{
//  copy_frame = *current_frame;
}


void
SceneEditor::
apply_copy()
{
//  copy_frame.raise();

  needed_to_redraw = true;
}


void
SceneEditor::
erase_this()
{
    if((*target)->size() > 1)
    {
      current_frame = target->delete_frame(current_frame);

        if(current_frame == (*target)->end())
        {
          --current_index;
          --current_frame;
        }


      target->update_index();

      current_frame->raise();

      needed_to_redraw = true;
    }
}




void
SceneEditor::
change_to_move_angle()
{
  change_mode(Mode::move_angle);

}


void
SceneEditor::
change_to_move_position()
{
  change_mode(Mode::move_position);

}


void
SceneEditor::
change_to_switch_reversing()
{
  change_mode(Mode::switch_reversing);
}


void
SceneEditor::
change_to_remove_doll()
{
  change_mode(Mode::remove_doll);
}


void
SceneEditor::
change_to_allocate_doll()
{
  change_mode(Mode::allocate_doll);
}


void
SceneEditor::
change_to_animate()
{
  current_frame->unraise();

  change_mode(Mode::animation);

  animation_frame = (*target)->begin();

  animation_frame->raise();
}




