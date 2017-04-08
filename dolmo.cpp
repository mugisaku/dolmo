#include"dolmo_node.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_image.hpp"




namespace{


Node*     root_node = nullptr;
Node*  current_node = nullptr;


void
process_motion(const SDL_MouseMotionEvent&  evt)
{
  current_node->change_angle(evt.x,evt.y);
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
  image::open("parts.png");


  root_node = new Node(Rect(0,0,80,80),Point(100,100),Point(30,80));

  current_node = root_node;

/*
  auto  child = new Node(Rect(48,0,48,48),Point(0,0),Point(0,0));

  root_node->join(child,48,48);
*/

  root_node->update();


  static SDL_Event  evt;

    for(;;)
    {
        while(SDL_PollEvent(&evt))
        {
            switch(evt.type)
            {
          case(SDL_MOUSEMOTION):
              process_motion(evt.motion);
              break;
          case(SDL_KEYDOWN):
                switch(evt.key.keysym.sym)
                {
              case(SDLK_LEFT):
                  root_node->update();
                  break;
              case(SDLK_RIGHT):
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




