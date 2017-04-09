#include"dolmo_node.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_image.hpp"




namespace{


Node*     root_node = nullptr;
Node*  current_node = nullptr;


void
process_button(const SDL_MouseButtonEvent&  evt)
{
  current_node = screen::get(evt.x,evt.y);
}


void
process_motion(const SDL_MouseMotionEvent&  evt)
{
    if(current_node)
    {
      current_node->change_angle(evt.x,evt.y);
    }
}


void
render()
{
    if(Node::needed_to_redraw)
    {
      screen::clear();

      root_node->render();

      screen::update();

      Node::needed_to_redraw = false;
    }
}


}




int
main(int  argc, char**  argv)
{
  screen::open();
  image::open("dolmo_parts.png");


  root_node = new Node(200,200);


  auto   head = root_node->join(new Node("頭部",0,Rect(   0, 0,80,80),Point(30,80)), 0,  0);
  auto   bust = root_node->join(new Node("胸部",0,Rect(   0,80,80,80),Point(30, 0)), 0,  0);
  auto  waist =      bust->join(new Node("腰部",0,Rect(80*4,60,80,80),Point(50, 0)),10, 70);

  root_node->update();


  static SDL_Event  evt;

    for(;;)
    {
        while(SDL_PollEvent(&evt))
        {
            switch(evt.type)
            {
          case(SDL_DROPFILE):
              SDL_free(evt.drop.file);
              break;
          case(SDL_WINDOWEVENT):
                if(evt.window.event == SDL_WINDOWEVENT_EXPOSED)
                {
                  Node::needed_to_redraw = true;
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
                  root_node->own_radian -= 0.1;
                  root_node->update();
                  break;
              case(SDLK_RIGHT):
                  root_node->own_radian += 0.1;
                  root_node->update();
                  break;
                }
              break;
          case(SDL_QUIT):
              goto EXIT;
              break;
            }
        }


      render();

      SDL_Delay(20);
    }


EXIT:
  screen::close();

  return 0;
}




