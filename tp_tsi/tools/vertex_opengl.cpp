#include "vertex_opengl.hpp"


vertex_opengl::vertex_opengl()
  :position(),normal(vec3(0,0,1)),color(),texture()
{
}

vertex_opengl::vertex_opengl(const vec3& position_param,
    const vec3& normal_param,
    const vec3& color_param,
    const vec2& texture_param)
  :position(position_param),normal(normal_param),color(color_param),texture(texture_param)
{
}
