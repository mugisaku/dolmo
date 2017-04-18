#include"dolmo_node.hpp"
#include"dolmo_image.hpp"
#include"dolmo_screen.hpp"
#include<algorithm>




int
reporting_count;




Node::
Node(int  x, int  y):
name("root"),
joining_kind(JoiningKind::none),
z_value(0),
parent(nullptr),
base_offset(x,y),
own_radian(0.0)
{}


Node::
Node(const char*  name_, int  z, Rect&&  img_rect, Point&&  img_center_):
name(name_),
joining_kind(JoiningKind::none),
z_value(z),
parent(nullptr),
image_rect(img_rect),
image_center(img_center_),
own_radian(0.0)
{}


Node::
Node(const Node&  rhs) noexcept:
name(rhs.name),
joining_kind(rhs.joining_kind),
z_value(rhs.z_value),
parent(rhs.parent),
image_rect(rhs.image_rect),
image_center(rhs.image_center),
own_radian(rhs.own_radian),
base_offset(rhs.base_offset)
{
    for(auto  child: rhs.children)
    {
      join(new Node(*child));
    }
}




void
Node::
reform(const Node&  rhs)
{
  base_offset = rhs.base_offset;
  own_radian  = rhs.own_radian;

    if(children.size() == rhs.children.size())
    {
      auto  dst =     children.begin();
      auto  src = rhs.children.cbegin();
      auto  end = rhs.children.cend();

        while(src != end)
        {
          (*dst++)->reform(**src++);
        }
    }
}


Node*
Node::
join(Node*  child)
{
  child->parent = this;

  children.emplace_back(child);

  child->update();


  return child;
}


Node*
Node::
join(Node*  child, int  x, int  y, JoiningKind  jk)
{
  child->parent = this;

  children.emplace_back(child);

  child->joining_kind = jk;

  child->base_offset.assign(x,y);

  child->update();


  return child;
}




void
Node::
change_angle(const Point&  pt)
{
  double  y = -pt.y+(graph_center.y);
  double  x =  pt.x-(graph_center.x);

    if((x != 0.0) &&
       (y != 0.0))
    {
      auto  r = -std::atan2(y,x);

        switch(joining_kind)
        {
      case(JoiningKind::upward):
          r += (pi*2)+((pi/2)*1);
          break;
      case(JoiningKind::downward):
          r += (pi*2)+((pi/2)*3);
          break;
      default:;
        }


      own_radian = r;

        if(parent)
        {
          own_radian -= parent->total_radian;
        }


      update();
    }
}


void
Node::
update()
{
    if(parent)
    {
      const auto&  r = parent->total_radian;
      const auto&  c = parent->graph_center;

      total_radian = r+own_radian;

      graph_center = (c+base_offset).transform(r,c);
    }

  else
    {
      total_radian =  own_radian;
      graph_center = base_offset;
    }


    for(auto  child: children)
    {
      child->update();
    }
}


void
Node::
render_center(Renderer&  dst)
{
  constexpr int  circle_radius = 8;

    for(int  y = 0;  y < circle_radius*2;  ++y)
    {
        for(int  x = 0;  x < circle_radius*2;  ++x)
        {
          dst.put(2,this,graph_center.x-circle_radius+x,
                         graph_center.y-circle_radius+y);
        }
    }
}


void
Node::
render_image(Renderer&  dst)
{
  const int      image_size = std::max(image_rect.w,image_rect.h);
  const int  rendering_size = image_size*2;

  const Point  rendering_base = (graph_center-image_center);

    for(int  y = -image_size;  y < rendering_size;  ++y)
    {
        for(int  x = -image_size;  x < rendering_size;  ++x)
        {
          const int  dst_x = (rendering_base.x+x);
          const int  dst_y = (rendering_base.y+y);

            if((dst_x >=              0) &&
               (dst_y >=              0) &&
               (dst_x <  screen::width ) &&
               (dst_y <  screen::height))
            {
              Point  pt(x,y);

              pt = pt.transform(-total_radian,image_center);

                if((pt.x >=            0) &&
                   (pt.y >=            0) &&
                   (pt.x <  image_rect.w) &&
                   (pt.y <  image_rect.h))
                {
                  auto  i = image::get(image_rect.x+pt.x,
                                       image_rect.y+pt.y);

                    if(i)
                    {
                      dst.put(i,this,dst_x,
                                     dst_y);
                    }
                }
            }
        }
    }
}


void
Node::
render(Renderer&  dst, int  z_max)
{
    if(z_value <= z_max)
    {
      render_image(dst);
    }


/*
    if(parent)
    {
      render_center(dst);
    }
*/

    for(auto  child: children)
    {
      child->render(dst,z_max);
    }
}




const char*
Node::
sscan(const char*  s)
{
  int  d;
  int  sz;
  int  n;

    if(sscanf(s," %d , %d , %n",&d,&sz,&n) >= 2)
    {
        if(sz != children.size())
        {
          return nullptr;
        }


      own_radian = d*pi/180;

      s += n;

        for(auto  child: children)
        {
          s = child->sscan(s);

            if(!s)
            {
              break;
            }
        }


      return s;
    }


  return nullptr;
}


void
Node::
fprint(FILE*  f) const
{
  fprintf(f,"%d,%d,",static_cast<int>(own_radian*180/pi),children.size());

    for(auto  child: children)
    {
      child->fprint(f);
    }
}


void
Node::
print() const
{
  printf("%s{\n",name);

  base_offset.print( " base offset");
  image_center.print("image center");
  graph_center.print("graph center");

  printf("  own radian %f\n",own_radian);
  printf("total radian %f\n",total_radian);

  printf("}\n");
}




