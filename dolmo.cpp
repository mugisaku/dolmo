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


void
load(char*  path)
{
  auto  f = fopen(path,"rb");

    if(f)
    {
      static char  buf[2048];

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

        if(root_node->sscan(buf))
        {
          root_node->update();
        }
    }


  SDL_free(path);
}


}




int
main(int  argc, char**  argv)
{
  screen::open();
  image::open("dolmo_parts.png");


  root_node = new Node(screen::width/2,screen::width/2);


  auto   head = root_node->join(new Node("頭部",0,Rect(   0, 0,80,80),Point(30,80)), 0,  0,JoiningKind::upward);
  auto   bust = root_node->join(new Node("胸部",0,Rect(   0,80,80,80),Point(30, 0)), 0,  0);
  auto  waist =      bust->join(new Node("腰部",0,Rect(80*4,60,80,80),Point(50, 0)),10, 70);


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


  root_node->update();


  static SDL_Event  evt;

    for(;;)
    {
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
              case(SDLK_SPACE):
                  root_node->fprint(stdout);
                  fputc('\n',stdout);
                  fflush(stdout);
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




