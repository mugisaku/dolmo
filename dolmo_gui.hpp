#ifndef DOLMO_GUI_HPP_INCLUDED
#define DOLMO_GUI_HPP_INCLUDED


#include<cstdint>
#include"dolmo_sceneEditor.hpp"


using Callback = void(SceneEditor::*)();


namespace gui{


void  initialize();


void   make_button(int  x, int  y, const char16_t*  text, SceneEditor&  mgr, Callback  cb);
bool  touch_button(int  x, int  y, bool  press=false);

void  add_radio_button(const char16_t*  text, SceneEditor&  mgr, Callback  cb);
void  fix_radio_buttons(int  x, int  y);

void  render_buttons();


}




#endif




