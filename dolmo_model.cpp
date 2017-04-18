#include"dolmo_model.hpp"
#include"dolmo_node.hpp"
#include"dolmo_screen.hpp"
#include<cstdlib>




namespace{


Node*
model;


void
make()
{
  auto  root = new Node(screen::width/2,screen::width/2-32);

  constexpr int  u = 64;

  auto   head = root->join(new Node("頭部",2,Rect(  0,0,u,u),Point(30,63)),10, 0,JoiningKind::upward);
  auto   bust = root->join(new Node("胸部",2,Rect(u*1,0,u,u),Point(20, 0)), 0, 0);
  auto  waist = bust->join(new Node("腰部",2,Rect(u*2,0,u,u),Point(20, 0)), 0,54);


  auto  l_thigh =   waist->join(new Node("左大腿",1,Rect(u*6,0,u,u),Point(35,0)),10,48);
  auto  l_shin  = l_thigh->join(new Node(  "左脛",1,Rect(u*7,0,u,u),Point(30,0)), 0,64);
  auto  l_foot  =  l_shin->join(new Node(  "左足",1,Rect(u*8,0,u,u),Point(30,0)), 0,64);

  auto  r_thigh =   waist->join(new Node("右大腿",3,Rect(u*6,0,u,u),Point(35,0)),10,48);
  auto  r_shin  = r_thigh->join(new Node(  "右脛",3,Rect(u*7,0,u,u),Point(30,0)), 0,64);
  auto  r_foot  =  r_shin->join(new Node(  "右足",3,Rect(u*8,0,u,u),Point(30,0)), 0,64);

  l_thigh->own_degree =  18;
  r_thigh->own_degree = -18;


  auto  l_upperarm =       bust->join(new Node("左上腕",0,Rect(u*3,0,u,u),Point(32,0)),10,10);
  auto  l_forearm  = l_upperarm->join(new Node("左前腕",0,Rect(u*4,0,u,u),Point(32,0)), 0,64);
  auto  l_hand     =  l_forearm->join(new Node(  "左手",0,Rect(u*5,0,u,u),Point(32,0)), 0,64);

  auto  r_upperarm =       bust->join(new Node("右上腕",4,Rect(u*3,0,u,u),Point(32,0)),10,10);
  auto  r_forearm  = r_upperarm->join(new Node("右前腕",4,Rect(u*4,0,u,u),Point(32,0)), 0,64);
  auto  r_hand     =  r_forearm->join(new Node(  "右手",4,Rect(u*5,0,u,u),Point(32,0)), 0,64);

  l_upperarm->own_degree =  18;
  r_upperarm->own_degree = -18;

  model = root;
}


}


const Node&
get_model()
{
  static bool  done;

    if(!done)
    {
      make();

      done = true;
    }


  return *model;
}




