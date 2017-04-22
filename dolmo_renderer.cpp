#include"dolmo_renderer.hpp"
#include"dolmo_node.hpp"




Renderer::
Renderer(int  w, int  h)
{
  resize(w,h);
}




void
Renderer::
resize(int  w, int  h)
{
  width  = w;
  height = h;

  table.resize(w*h);
}


void
Renderer::
clear()
{
  auto  it = &table[0];

  auto  end = &table[width*height];

    while(it != end)
    {
      it->color_index = 0;
      it->z           = 0;
      it->nodeptr     = nullptr;

      ++it;
    }
}




int  Renderer::get_width() const{return width;}
int  Renderer::get_height() const{return height;}


void
Renderer::
put(int  color_index, Node*  nodeptr, int  x, int  y)
{
    if((x >=     0) &&
       (y >=     0) &&
       (x <  width) &&
       (y < height))
    {
      auto&  cell = table[width*y+x];

      auto  z = nodeptr->z_value;

        if(!cell.color_index || (cell.z < z))
        {
          cell.color_index = (3*z)+color_index;

          cell.z           = z;
          cell.nodeptr     = nodeptr;
        }
    }
}


const Cell&
Renderer::
get_cell(int  x, int  y) const
{
  return table[width*y+x];
}




