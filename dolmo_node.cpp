#include"dolmo_node.hpp"
#include"dolmo_image.hpp"
#include"dolmo_screen.hpp"
#include<algorithm>




int
reporting_count;


bool
Node::
needed_to_redraw;




Node::
Node(Rect&&  rect, Point&&  offset_, Point&&  center_):
parent(nullptr),
image_rect(rect),
raw_offset(offset_),
raw_center(center_),
raw_radian(0.0)
{}




void
Node::
join(Node*  child, int  x, int  y)
{
  child->parent = this;

  children.emplace_back(child);

  child->raw_offset.x = x;
  child->raw_offset.y = y;

  child->update();
}


void
Node::
change_angle(int  x, int  y)
{
  y = -y+(transformed_center.y);
  x =  x-(transformed_center.x);

    if(x || y)
    {
      raw_radian = -std::atan2(y,x);

        if(raw_radian < 0)
        {
//          raw_radian += (pi*2);
        }


//      raw_radian += (pi/2);


      update();
    }
}


void
Node::
update()
{
  needed_to_redraw = true;

  transformed_offset.x = raw_offset.x;
  transformed_offset.y = raw_offset.y;
  transformed_radian   = raw_radian;

    if(parent)
    {
      transformed_offset.x += parent->transformed_offset.x;
      transformed_offset.y += parent->transformed_offset.y;
      transformed_radian   += parent->transformed_radian;
    }

  else
    {
    }


  transformed_center.x = transformed_offset.x+raw_center.x;
  transformed_center.y = transformed_offset.y+raw_center.y;

//  transformed_offset.transform(transformed_radian,transformed_center);

//    center_result.transform(radian_result,center_result);

    for(auto  child: children)
    {
      child->update();
    }
}


bool
Node::
scan(int  x, int  y)
{
/*
    if((x >= (base_result.x-circle_radius)) &&
       (y >= (base_result.y-circle_radius)) &&
       (x <  (base_result.x+circle_radius)) &&
       (y <  (base_result.y+circle_radius)))
    {
      needed_to_redraw = true;

      current_node = this;

      return true;
    }

  else
    if((x >= (tail_result.x-(square_size/2))) &&
       (y >= (tail_result.y-(square_size/2))) &&
       (x <  (tail_result.x+(square_size/2))) &&
       (y <  (tail_result.y+(square_size/2))))
    {
      needed_to_redraw = true;

      current_node = this;

      return true;
    }
*/

    for(auto  child: children)
    {
        if(child->scan(x,y))
        {
          return true;
        }
    }


  return false;
}


void
Node::
render_center()
{
    for(int  y = 0;  y < circle_radius*2;  ++y)
    {
        for(int  x = 0;  x < circle_radius*2;  ++x)
        {
          auto  i = image::get( 90+x,
                               110+y);

            if(i)
            {
              screen::put(transformed_center.x-circle_radius+x,
                          transformed_center.y-circle_radius+y,0,i,this);
            }
        }
    }
}


void
Node::
render_image()
{
  const int      image_size = std::max(image_rect.w,image_rect.h);
  const int  rendering_size = image_size*2;

    for(int  y = -image_size;  y < rendering_size;  ++y)
    {
        for(int  x = -image_size;  x < rendering_size;  ++x)
        {
          Point  pt(x,y);

          pt.transform(-transformed_radian,raw_center);

            if((pt.x >=            0) &&
               (pt.y >=            0) &&
               (pt.x <  image_rect.w) &&
               (pt.y <  image_rect.h))
            {
              auto  i = image::get(image_rect.x+pt.x,
                                   image_rect.y+pt.y);

                if(i)
                {
                  screen::put(transformed_offset.x+x,
                              transformed_offset.y+y,0,i,this);
                }
            }
        }
    }
}


void
Node::
render()
{
  render_center();
  render_image();

    for(auto  child: children)
    {
      child->render();
    }
}





