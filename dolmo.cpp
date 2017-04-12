#include"dolmo_node.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_image.hpp"
#include<list>
#include<vector>
#include<cstdlib>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


Node*  current_node = nullptr;


Point  previous_point;
Point   current_point;


int
current_index;


std::list<Node*>
root_list;


std::list<Node*>::iterator
current_root;


std::list<Node*>::const_iterator
current_frame;


std::vector<Node*>
stock;


Node*
model;


bool
animation_flag;


bool
needed_to_redraw = true;


void
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
insert_new_to_previous()
{
    if(root_list.size() < 98)
    {
      auto  root = new Node(*model);

      root->update();

      root_list.emplace(current_root,root);

      ++current_index;

      needed_to_redraw = true;
    }
}


void
insert_new_to_next()
{
    if(root_list.size() < 98)
    {
      auto  root = new Node(*model);

      auto  it = current_root;

      root->update();

      root_list.emplace(++it,root);

      needed_to_redraw = true;
    }
}


void
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
print()
{
}


void
start_to_animate()
{
  animation_flag = true;

  current_frame = root_list.cbegin();

  needed_to_redraw = true;
}


void
process_button(const SDL_MouseButtonEvent&  evt)
{
    if(animation_flag)
    {
      animation_flag = false;

      needed_to_redraw = true;
    }

  else
    {
        if(!screen::push_button(evt.x,evt.y))
        {
          current_node = screen::get(evt.x,evt.y);

                           current_point.assign(evt.x,evt.y);
          previous_point = current_point                    ;
        }
    }
}


void
process_motion(const SDL_MouseMotionEvent&  evt)
{
  current_point.assign(evt.x,evt.y);
}


void
render()
{
    if(needed_to_redraw)
    {
      auto  root = animation_flag? current_frame:current_root;

      screen::clear();

      (*root)->render();


      screen::put(current_index+1,root_list.size(),screen::width-(16*5),0);

      screen::update();

      needed_to_redraw = false;
    }
}


void
load(char*  path)
{
  auto  f = fopen(path,"rb");

    if(f)
    {
      static char  buf[4096];

      char*  p = buf;

        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f) || ferror(f))
            {
              break;
            }


          *p++ = c;
        }


      fclose(f);

        if((*current_root)->sscan(buf))
        {
          (*current_root)->update();
        }
    }


  SDL_free(path);
}


Node*
create_model()
{
  auto  root = new Node(screen::width/2,screen::width/2);

  auto   head = root->join(new Node("頭部",0,Rect(   0, 0,80,80),Point(30,80)), 0,  0,JoiningKind::upward);
  auto   bust = root->join(new Node("胸部",0,Rect(   0,80,80,80),Point(30, 0)), 0,  0);
  auto  waist = bust->join(new Node("腰部",0,Rect(80*4,60,80,80),Point(50, 0)),10, 70);


  auto  l_thigh =   waist->join(new Node("左大腿",-1,Rect(80*2,0,80,100),Point(40,10)),-10, 60);
  auto  l_shin  = l_thigh->join(new Node(  "左脛",-1,Rect(80*3,0,80,100),Point(40,10)),-10, 70);
  auto  l_foot  =  l_shin->join(new Node(  "左足",-1,Rect(80*4,0,80, 40),Point(30, 0)),  0, 80);

  auto  r_thigh =   waist->join(new Node("右大腿",1,Rect(80*2,0,80,100),Point(40,10)),-10, 60);
  auto  r_shin  = r_thigh->join(new Node(  "右脛",1,Rect(80*3,0,80,100),Point(40,10)),-10, 70);
  auto  r_foot  =  r_shin->join(new Node(  "右足",1,Rect(80*4,0,80, 40),Point(30, 0)),  0, 80);

  l_thigh->own_radian =  15*pi/180;
  r_thigh->own_radian = -15*pi/180;


  auto  l_upperarm =       bust->join(new Node("左上腕",-1,Rect(80*1   ,  0,40,100),Point(20,10)),0, 10);
  auto  l_forearm  = l_upperarm->join(new Node("左前腕",-1,Rect(80*1+40,  0,40,100),Point(20,10)),0, 80);
  auto  l_hand     =  l_forearm->join(new Node(  "左手",-1,Rect(80*1+40,100,80, 60),Point(20, 0)),0, 80);

  auto  r_upperarm =       bust->join(new Node("右上腕",1,Rect(80*1   ,  0,40,100),Point(20,10)),0, 10);
  auto  r_forearm  = r_upperarm->join(new Node("右前腕",1,Rect(80*1+40,  0,40,100),Point(20,10)),0, 80);
  auto  r_hand     =  r_forearm->join(new Node(  "右手",1,Rect(80*1+40,100,80, 60),Point(20, 0)),0, 80);

  l_upperarm->own_radian =  15*pi/180;
  r_upperarm->own_radian = -15*pi/180;


  return root;
}


void
main_loop()
{
  static SDL_Event  evt;

    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
      case(SDL_DROPFILE):
          load(evt.drop.file);
          break;
      case(SDL_WINDOWEVENT):
            if(evt.window.event == SDL_WINDOWEVENT_EXPOSED)
            {
              needed_to_redraw = true;
            }
          break;
      case(SDL_MOUSEBUTTONDOWN):
          process_button(evt.button);
          break;
      case(SDL_MOUSEBUTTONUP):
          current_node = nullptr;
          break;
      case(SDL_MOUSEMOTION):
          process_motion(evt.motion);
          break;
      case(SDL_KEYDOWN):
            switch(evt.key.keysym.sym)
            {
          case(SDLK_LEFT):
              break;
          case(SDLK_RIGHT):
              ++current_index;
              needed_to_redraw = true;
              break;
          case(SDLK_SPACE):
              (*current_root)->fprint(stdout);
              fputc('\n',stdout);
              fflush(stdout);
              break;
            }
          break;
      case(SDL_QUIT):
          screen::close();
          std::exit(0);
          break;
        }
    }


    if(animation_flag)
    {
      static uint32_t  last;

      auto  now = SDL_GetTicks();

        if(now >= (last+200))
        {
          last = now;

            if(++current_frame == root_list.cend())
            {
              current_frame = root_list.cbegin();
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


  render();
}


}




int
main(int  argc, char**  argv)
{
  screen::open();
  image::open("dolmo_parts.png");

  screen::make_button(0,  0,"change to previous",change_to_previous);
  screen::make_button(0, 20,"change to next",change_to_next);
  screen::make_button(0, 40,"insert new to previous",insert_new_to_previous);
  screen::make_button(0, 60,"insert new to next",insert_new_to_next);
  screen::make_button(0, 80,"erase this",erase_this);
  screen::make_button(0,100,"animate",start_to_animate);
  screen::make_button(0,120,"print",print);

  model = create_model();

  root_list.emplace_back(new Node(*model));

  current_root = root_list.begin();

  (*current_root)->update();


#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,-1,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }
#endif


  return 0;
}




