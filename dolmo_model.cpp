#include"dolmo_model.hpp"
#include"dolmo_node.hpp"
#include"dolmo_screen.hpp"
#include<cstdlib>




namespace{


Node*
model;


constexpr int  u = 64;


Node*
make_arm(Node*  bust, int  z)
{
  auto  upperarm =     bust->join(new Node("上腕",z,Rect(u*4,0,u,u),Point(32,8)),4,10);
  auto   forearm = upperarm->join(new Node("前腕",z,Rect(u*5,0,u,u),Point(32,8)),0,64);
  auto      hand =  forearm->join(new Node(  "手",z,Rect(u*6,0,u,u),Point(32,8)),0,64);

  return upperarm;
}


Node*
make_leg(Node*  waist, int  z)
{
  auto  thigh = waist->join(new Node("大腿",z,Rect(u*7,0,u,u),Point(32, 8)),-12,10);
  auto   shin = thigh->join(new Node(  "脛",z,Rect(u*8,0,u,u),Point(30, 8)), 0,64); 
  auto   foot =  shin->join(new Node(  "足",z,Rect(u*9,0,u,u),Point(30, 8)), 0,64);

  return thigh;
}


void
make()
{
  auto  root = new Node(screen::width/2,screen::width/2-64);

  auto     head =    root->join(new Node("頭部",2,Rect(u*0,0,u,u),Point(32,64)), 0, 0,JoiningKind::upward);
  auto     bust =    root->join(new Node("胸部",3,Rect(u*1,0,u,u),Point(32, 8)), 0, 0);
  auto  abdomen =    bust->join(new Node("腹部",2,Rect(u*2,0,u,u),Point(32,32)), 0,54);
  auto    waist = abdomen->join(new Node("腰部",3,Rect(u*3,0,u,u),Point(48,32)),16,20);

  make_arm(bust,5)->own_degree =  18;
  make_arm(bust,0)->own_degree = -18;

  make_leg(waist,4)->own_degree =  18;
  make_leg(waist,1)->own_degree = -18;



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




