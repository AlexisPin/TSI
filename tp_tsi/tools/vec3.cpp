
#include "vec3.hpp"
#include <cmath>
#include <cassert>
#include <iostream>


vec3::vec3()
  :x(0.0f),y(0.0f),z(0.0f)
{}

vec3::vec3(float x_param,float y_param,float z_param)
  :x(x_param),y(y_param),z(z_param)
{}

vec3& vec3::operator+=(const vec3& v)
{
  x+=v.x;
  y+=v.y;
  z+=v.z;
  return *this;
}

vec3& vec3::operator-=(const vec3& v)
{
  x-=v.x;
  y-=v.y;
  z-=v.z;
  return *this;
}

vec3& vec3::operator*=(float s)
{
  x*=s; y*=s; z*=s;
  return *this;
}

vec3& vec3::operator/=(float s)
{
  assert(fabs(s)>10e-6);
  x/=s; y/=s; z/=s;
  return *this;
}

std::ostream& operator<<(std::ostream& sout,const vec3& v)
{
  sout<<v.x<<","<<v.y<<","<<v.z;
  return sout;
}

float norm(const vec3& v)
{
  return std::sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

float dot(const vec3& v0,const vec3& v1)
{
  return v0.x*v1.x+v0.y*v1.y+v0.z*v1.z;
}

vec3 cross(const vec3& v0,const vec3& v1)
{
  return vec3(v0.y*v1.z-v0.z*v1.y,
      v0.z*v1.x-v0.x*v1.z,
      v0.x*v1.y-v0.y*v1.x);
}

vec3 normalize(const vec3& v)
{
  vec3 temp=v;
  temp/=norm(v);
  return temp;
}

vec3 operator+(const vec3& v0,const vec3& v1)
{
  vec3 temp=v0;
  temp+=v1;
  return temp;
}

vec3 operator-(const vec3& v0,const vec3& v1)
{
  vec3 temp=v0;
  temp-=v1;
  return temp;
}

vec3 operator*(const vec3& v0,float s)
{
  vec3 temp=v0;
  temp*=s;
  return temp;
}

vec3 operator*(float s,const vec3& v0)
{
  return v0*s;
}

vec3 operator/(const vec3& v0,float s)
{
  vec3 temp=v0;temp/=s;return temp;
}


