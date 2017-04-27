#include"dolmo_model.hpp"
#include"dolmo_node.hpp"
#include"dolmo_screen.hpp"
#include<cstdlib>




namespace{


Node*
model;


constexpr int  u = 64;


Node*
make_foreleg(Node*  bust, int  z)
{
  auto  p0 = bust->join(new Node("",z,1,Rect(u*5,0,u,u),Point( 0,0)),-56,0);
  auto  p1 =   p0->join(new Node("",z,1,Rect(u*6,0,u,u),Point(32,8)),32,32);
  auto  p2 =   p1->join(new Node("",z,1,Rect(u*7,0,u,u),Point(32,8)), 0,40);
  auto  p3 =   p2->join(new Node("",z,1,Rect(u*8,0,u,u),Point(32,8)), 0,40);

  p0->fix_angle();

  return p0;
}


Node*
make_hindleg(Node*  waist, int  z)
{
  auto  p0 = waist->join(new Node("",z,1,Rect(u* 9,0,u,u),Point( 0,0)),-8,0);
  auto  p1 =    p0->join(new Node("",z,1,Rect(u*10,0,u,u),Point(32,8)),16,48);
  auto  p2 =    p1->join(new Node("",z,1,Rect(u*11,0,u,u),Point(32,8)), 0,56);
  auto  p3 =    p2->join(new Node("",z,1,Rect(u*12,0,u,u),Point(32,8)), 0,32);

  p0->fix_angle();

  return p0;
}


void
make()
{
  auto  root = new Node(screen::width/2,screen::width/2-32);

  auto  abdomen =    root->join(new Node("腹",2,1,Rect(u*3,0,u,u),Point(32,32)),0,0);
  auto    waist = abdomen->join(new Node("腰",2,1,Rect(u*4,0,u,u),Point( 0,32)),32,0,JoiningKind::to_right);

  auto  bust = abdomen->join(new Node("胸",2,1,Rect(u*2,0,u,u),Point(64,32)),-32,0,JoiningKind::to_left);
  auto  neck =    bust->join(new Node("頚",2,1,Rect(u*1,0,u,u),Point(64,32)),-32,-16,JoiningKind::to_left);
  auto  head =    neck->join(new Node("胸",2,1,Rect(u*0,0,u,u),Point(64,32)),-56,0,JoiningKind::to_left);

  bust->fix_angle();
  waist->fix_angle();

  make_foreleg(bust,4)->change_degree(18);
  make_foreleg(bust,0)->change_degree(-18);

  make_hindleg(waist,3)->change_degree( 18);
  make_hindleg(waist,1)->change_degree(-18);

  model = root;
}


}


const Node&
get_horse_model()
{
  static bool  done;

    if(!done)
    {
      make();

      done = true;
    }


  return *model;
}




