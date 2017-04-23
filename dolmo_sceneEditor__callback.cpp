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
      --current_index;
      --current_frame;

      needed_to_redraw = true;
    }
}


void
SceneEditor::
change_to_next()
{
    if(current_index < ((*target)->size()-1))
    {
      ++current_index;
      ++current_frame;

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

      needed_to_redraw = true;
    }
}


void
SceneEditor::
copy_this()
{
    if(!copy_node)
    {
//      copy_node = new Scene;
    }


//  copy_node->reform(**target);
}


void
SceneEditor::
apply_copy()
{
    if(copy_node)
    {
//      (*target)->reform(*copy_node);
      current_frame->update();

      needed_to_redraw = true;
    }
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


      needed_to_redraw = true;
    }
}


void
SceneEditor::
start_to_animate()
{
  change_mode(Mode::animation);

  animation_frame = (*target)->begin();
}




