
#include "mesh.hpp"

#include "mat4.hpp"

#include "format/mesh_io_obj.hpp"
#include "format/mesh_io_off.hpp"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <cmath>
#include <algorithm>



mesh load_off_file(const std::string& filename)
{
  return cpe::load_mesh_file_off(filename);
}


mesh load_obj_file(const std::string& filename)
{
  return cpe::load_mesh_file_obj(filename);
}



void update_normals(mesh* m)
{
  const std::vector<triangle_index>& c=m->connectivity;

  //compute star
  std::map<int,std::list<int> > star;
  for(unsigned int k=0,N=c.size();k<N;++k)
  {
    const triangle_index& t=c[k];
    star[t.u0].push_back(k);
    star[t.u1].push_back(k);
    star[t.u2].push_back(k);
  }

  //compute per polygon normal
  std::vector<vec3> normal_polygon;
  normal_polygon.resize(m->connectivity.size());
  for(unsigned int k=0,N=m->connectivity.size();k<N;++k)
  {
    const triangle_index& t=m->connectivity[k];
    const vec3& p0=m->vertex[t.u0].position;
    const vec3& p1=m->vertex[t.u1].position;
    const vec3& p2=m->vertex[t.u2].position;

    const vec3 u0=normalize(p1-p0);
    const vec3 u1=normalize(p2-p0);
    const vec3 n=normalize(cross(u0,u1));

    normal_polygon[k]=n;
  }

  //compute per vertex normal
  for(unsigned int k=0,N=m->vertex.size();k<N;++k)
  {
    vec3 temp_normal;
    for(std::list<int>::const_iterator it=star[k].begin(),it_end=star[k].end();
        it!=it_end;++it)
      temp_normal += normal_polygon[*it];

    m->vertex[k].normal=normalize(temp_normal);
  }

}

void fill_color(mesh* m,const vec3& color)
{
  for(unsigned int k=0,N=m->vertex.size();k<N;++k)
    m->vertex[k].color=color;
}
void fill_color_normal(mesh* m)
{
  for(unsigned int k=0,N=m->vertex.size();k<N;++k)
  {
    const vec3& n=m->vertex[k].normal;
    m->vertex[k].color=vec3(fabs(n.x),fabs(n.y),fabs(n.z));
  }
}

void apply_deformation(mesh* m,const mat4 T)
{
  for(unsigned int k=0,N=m->vertex.size();k<N;++k)
  {
    vec3& v=m->vertex[k].position;
    v=T*v;
  }
}

void invert_normals(mesh* m)
{
  for(unsigned int k=0,N=m->vertex.size();k<N;++k)
    m->vertex[k].normal*=-1.0f;
}
