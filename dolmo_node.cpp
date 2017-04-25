#include"dolmo_node.hpp"
#include"dolmo_image.hpp"
#include"dolmo_screen.hpp"
#include"dolmo_renderer.hpp"
#include<algorithm>




int
reporting_count;


Node::
Node(int  x, int  y):
name("root"),
joining_kind(JoiningKind::none),
z_value(0),
parent(nullptr),
doll(nullptr),
base_offset(x,y),
own_degree(0),
angle_fixed(false)
{}


Node::
Node(const char*  name_, int  z, Rect&&  img_rect, Point&&  img_center_):
name(name_),
joining_kind(JoiningKind::none),
z_value(z),
parent(nullptr),
doll(nullptr),
image_rect(img_rect),
image_center(img_center_),
own_degree(0),
angle_fixed(false)
{}


Node::
Node(const Node&  rhs) noexcept:
parent(nullptr)
{
  *this = rhs;
}


Node::
~Node()
{
  clear();
}




Node&
Node::
operator=(const Node&  rhs) noexcept
{
  clear();

  name         = rhs.name;
  joining_kind = rhs.joining_kind;
  z_value      = rhs.z_value;
  image_rect   = rhs.image_rect;
  image_center = rhs.image_center;
  own_degree   = rhs.own_degree;
  base_offset  = rhs.base_offset;
  angle_fixed  = rhs.angle_fixed;

    for(auto  child: rhs.children)
    {
      join(new Node(*child));
    }


  return *this;
}




void
Node::
clear()
{
    for(auto  child: children)
    {
      delete child;
    }


  children.clear();
}




int
Node::
get_z_value() const
{
  return z_value;
}


Doll*
Node::
get_doll() const
{
  return doll;
}


void
Node::
fix_angle()
{
  angle_fixed = true;
}


void
Node::
change_doll(Doll&  doll_)
{
  doll = &doll_;

    for(auto  child: children)
    {
      child->change_doll(doll_);
    }
}


void
Node::
change_base_offset(const Point&  pt)
{
  base_offset = pt;
}


void
Node::
add_to_base_offset(const Point&  pt)
{
  base_offset += pt;
}


const Point&
Node::
get_base_offset() const
{
  return base_offset;
}


const Point&
Node::
get_graph_center() const
{
  return graph_center;
}


void
Node::
change_degree(int  v)
{
  own_degree = v;
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
    if(angle_fixed)
    {
        if(parent)
        {
          parent->change_angle(pt);
        }
    }

  else
    {
      double  y = -pt.y+graph_center.y;
      double  x =  pt.x-graph_center.x;

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


          own_degree = r*180/pi;

            if(parent)
            {
              own_degree -= parent->total_degree;
            }


          update();
        }
    }
}


void
Node::
update(bool  reversing)
{
    if(parent)
    {
      total_degree = parent->total_degree+own_degree;
    }

  else
    {
      total_degree =  own_degree;
      graph_center = base_offset;
    }


    sin_value = sin_value_table[(total_degree%360)/10+36];
    cos_value = cos_value_table[(total_degree%360)/10+36];

    reversed_sin_value = sin_value_table[(-total_degree%360)/10+36];
    reversed_cos_value = cos_value_table[(-total_degree%360)/10+36];

      if(parent)
      {
        const auto&  c = parent->graph_center;

        auto  pt = base_offset;

          if(reversing)
          {
            pt.x *= -1;
          }


        graph_center = (c+pt).transform(parent->sin_value,
                                        parent->cos_value,c);
      }


    for(auto  child: children)
    {
      child->update(reversing);
    }
}


void
Node::
render_image(Renderer&  dst, bool  reversing)
{
  const auto  dst_w = dst.get_width();
  const auto  dst_h = dst.get_height();

  const int      image_size = std::max(image_rect.w,image_rect.h);
  const int  rendering_size = image_size*2;

  const Point  rendering_base = (graph_center-image_center);

    for(int  y = -image_size;  y < rendering_size;  ++y)
    {
        for(int  x = -image_size;  x < rendering_size;  ++x)
        {
          const int  dst_x = (rendering_base.x+x);
          const int  dst_y = (rendering_base.y+y);

            if((dst_x >=     0) &&
               (dst_y >=     0) &&
               (dst_x <  dst_w) &&
               (dst_y <  dst_h))
            {
              Point  pt(x,y);

              pt = pt.transform(reversed_sin_value,reversed_cos_value,image_center);

                if((pt.x >=            0) &&
                   (pt.y >=            0) &&
                   (pt.x <  image_rect.w) &&
                   (pt.y <  image_rect.h))
                {
                  int  i;

                    if(reversing)
                    {
                      i = image::get(image_rect.x+image_rect.w-1-pt.x,
                                     image_rect.y+pt.y);
                    }

                  else
                    {
                      i = image::get(image_rect.x+pt.x,
                                     image_rect.y+pt.y);
                    }


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
render(Renderer&  dst, bool  reversing, int  z_max)
{
    if(z_value <= z_max)
    {
      render_image(dst,reversing);
    }


    for(auto  child: children)
    {
      child->render(dst,reversing,z_max);
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


      own_degree = d;

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
  fprintf(f,"%d,%d,",own_degree,children.size());

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

  printf("  own degree %d\n",own_degree);
  printf("total degree %d\n",total_degree);

  printf("}\n");
}




void
Node::
read(const int*&  it)
{
  own_degree = *it++;

  auto  n = *it++;

    if(n != children.size())
    {
      printf("子要素数が一致しません\n");

      throw;
    }


    for(auto  child: children)
    {
      child->read(it);
    }
}


size_t
Node::
write(int*&  it, const int*  end) const
{
  size_t  count = 0;

  *it++ = own_degree;

  ++count;

    if(it != end)
    {
      *it++ = children.size();

      ++count;

        if(it != end)
        {
            for(auto  child: children)
            {
              count += child->write(it,end);
            }
        }
    }


  return count;
}




