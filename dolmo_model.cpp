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
  auto  thigh = waist->join(new Node("大腿",z,Rect(u*7,0,u,u),Point(32,8)),0,6);
  auto   shin = thigh->join(new Node(  "脛",z,Rect(u*8,0,u,u),Point(32,8)),0,64); 
  auto   foot =  shin->join(new Node(  "足",z,Rect(u*9,0,u,u),Point(32,8)),0,64);

  return thigh;
}


void
make()
{
  auto  root = new Node(screen::width/2,screen::width/2-32);

  auto     head =    root->join(new Node("頭",2,Rect(u*0,0,u,u),Point(32,56)),0, 0);
  auto     bust =    head->join(new Node("胸",2,Rect(u*1,0,u,u),Point(32, 8)),0, 8);
  auto  abdomen =    bust->join(new Node("腹",2,Rect(u*2,0,u,u),Point(32,32)),0,56);
  auto    waist = abdomen->join(new Node("腰",2,Rect(u*3,0,u,u),Point(32,32)),0,32);

  waist->fix_angle();

  make_arm(bust,4)->change_degree(18);
  make_arm(bust,0)->change_degree(-18);

  make_leg(waist,3)->change_degree( 18);
  make_leg(waist,1)->change_degree(-18);


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




