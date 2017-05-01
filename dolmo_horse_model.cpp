#include"dolmo_model.hpp"
#include"dolmo_node.hpp"
#include"dolmo_screen.hpp"
#include<cstdlib>




namespace{


Node*
model;


constexpr int  w = 64;
constexpr int  h = 96;


Node*
make_foreleg(Node*  abdomen, int  z)
{
  auto  p0 = abdomen->join(new Node("",z,1,Rect(w*5,0,w,h),Point( 0,0)),-72,-16);
  auto  p1 =      p0->join(new Node("",z,1,Rect(w*6,0,w,h),Point(32,8)),32,40);
  auto  p2 =      p1->join(new Node("",z,1,Rect(w*7,0,w,h),Point(32,8)), 0,56);
  auto  p3 =      p2->join(new Node("",z,1,Rect(w*8,0,w,h),Point(32,8)), 0,44);

  return p0;
}


Node*
make_hindleg(Node*  abdomen, int  z)
{
  auto  p0 = abdomen->join(new Node("",z,1,Rect(w* 9,0,w,h),Point(56,8)),64,-16);
  auto  p1 =      p0->join(new Node("",z,1,Rect(w*10,0,w,h),Point( 8,8)),-40,32);
  auto  p2 =      p1->join(new Node("",z,1,Rect(w*11,0,w,h),Point(32,8)),32,40);
  auto  p3 =      p2->join(new Node("",z,1,Rect(w*12,0,w,h),Point(32,8)), 8,64);

  return p0;
}


void
make()
{
  auto  root = new Node(screen::width/2,screen::width/2-32);

  auto  abdomen = root->join(new Node("胴",2,1,Rect(160,0,160,h),Point(240-160,48)),0,0);

  auto  neck = abdomen->join(new Node("頚",2,1,Rect(w*1,0,96,h),Point(138-64,58)),-44,-24,JoiningKind::to_left);
  auto  head =    neck->join(new Node("頭",2,1,Rect(w*0,0,w,h),Point(56,32)),-72,-32,JoiningKind::to_left);

  make_foreleg(abdomen,4);
  make_foreleg(abdomen,0);

  make_hindleg(abdomen,3);
  make_hindleg(abdomen,1);

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




