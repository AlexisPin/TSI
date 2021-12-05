
#include <cmath>
#include <cstdlib>



#include "mat4.hpp"
#include "vec3.hpp"

mat4::mat4()
{
  M[0]=1.0f;M[4]=0.0f;M[ 8]=0.0f;M[12]=0.0f;
  M[1]=0.0f;M[5]=1.0f;M[ 9]=0.0f;M[13]=0.0f;
  M[2]=0.0f;M[6]=0.0f;M[10]=1.0f;M[14]=0.0f;
  M[3]=0.0f;M[7]=0.0f;M[11]=0.0f;M[15]=1.0f;
}

mat4::mat4(float x00,float x01,float x02,float x03,
    float x10,float x11,float x12,float x13,
    float x20,float x21,float x22,float x23,
    float x30,float x31,float x32,float x33)
{
  M[0]=x00;M[4]=x01;M[ 8]=x02;M[12]=x03;
  M[1]=x10;M[5]=x11;M[ 9]=x12;M[13]=x13;
  M[2]=x20;M[6]=x21;M[10]=x22;M[14]=x23;
  M[3]=x30;M[7]=x31;M[11]=x32;M[15]=x33;
}



float mat4::operator()(int x,int y) const
{
  if(x>=0 && x<4 && y>=0 && y<4)
    return M[x+4*y];

  else
  {
    //gestion d'erreur
    std::cout<<"Indices de matrices incorrects ("<<x<<","<<y<<")"<<std::endl;
    abort();
  }
}


float& mat4::operator()(int x,int y)
{
  if(x>=0 && x<4 && y>=0 && y<4)
    return M[x+4*y];

  else
  {
    //gestion d'erreur
    std::cout<<"Indices de matrices incorrects ("<<x<<","<<y<<")"<<std::endl;
    abort();
  }
}



mat4 operator*(const mat4& m1,const mat4& m2)
{
  mat4 res=matrice_zeros();

  for(int kx=0;kx<4;++kx)
  {
    for(int ky=0;ky<4;++ky)
    {
      for(int kz=0;kz<4;++kz)
        res(kx,ky) += m1(kx,kz)*m2(kz,ky);
    }
  }

  return res;
}


const float *pointeur(const mat4& m)
{
  return m.M;
}


mat4 transpose(const mat4& m)
{
  return mat4(m(0,0),m(1,0),m(2,0),m(3,0),
      m(0,1),m(1,1),m(2,1),m(3,1),
      m(0,2),m(1,2),m(2,2),m(3,2),
      m(0,3),m(1,3),m(2,3),m(3,3));
}

mat4 matrice_rotation(float angle,float axe_x,float axe_y,float axe_z)
{
  const float n=std::sqrt(axe_x*axe_x+axe_y*axe_y+axe_z*axe_z);
  const float epsilon=1e-5f;

  mat4 m;
  if(n<epsilon)
  {
    std::cout<<"Attention, norme de vecteur vaut 0 pour matrice de rotation"<<std::endl;
    return m;
  }

  const float x=axe_x/n;
  const float y=axe_y/n;
  const float z=axe_z/n;

  const float cost=cos(angle);
  const float sint=sin(angle);

  m(0,0)=cost+x*x*(1.0f-cost);    m(0,1)=x*y*(1.0f-cost)-z*sint;   m(0,2)=x*z*(1.0f-cost)+y*sint;  m(0,3)=0.0f;
  m(1,0)=y*x*(1.0f-cost)+z*sint;  m(1,1)=cost+y*y*(1.0f-cost);     m(1,2)=y*z*(1.0f-cost)-x*sint;  m(1,3)=0.0f;
  m(2,0)=z*x*(1.0f-cost)-y*sint;  m(2,1)=z*y*(1.0f-cost)+x*sint;   m(2,2)=cost+z*z*(1.0f-cost);    m(2,3)=0.0f;
  m(3,0)=0.0f;                    m(3,1)=0.0f;                     m(3,2)=0.0f;                    m(3,3)=1.0f;

  return m;
}


mat4 matrice_projection(float fov,float aspect,float d_min,float d_max)
{
  const float f=1.0f/std::tan(fov/2.0f);
  const float fx=f/aspect;

  const float L=d_min-d_max;
  const float C=(d_max+d_min)/L;
  const float D=(2.0f*d_max*d_min)/L;

  return mat4(fx   ,0.0f ,0.0f ,0.0f,
      0.0f ,f    ,0.0f ,0.0f,
      0.0f ,0.0f ,C    ,D   ,
      0.0f ,0.0f ,-1.0f,0.0f);

}

mat4 matrice_lookat(const vec3& eye, const vec3& center, const vec3& up)
{
  vec3 X, Y, Z;
  Z = normalize(eye - center); 
  X = normalize(cross(normalize(up), Z));
  Y = cross(Z, X);

  return mat4(  
      X.x, X.y, X.z, -dot(X, eye),
      Y.x, Y.y, Y.z, -dot(Y, eye), 
      Z.x, Z.y, Z.z, -dot(Z, eye),
      0.,  0.,  0.,  1. 
      );
}

vec3 extract_translation(mat4& m)
{
  vec3 v(m(0,3), m(1,3), m(2,3));
  m(0,3) = m(1,3) = m(2, 3) = 0.;
  return v;
}

mat4 matrice_zeros()
{
  return mat4(0.0f,0.0f,0.0f,0.0f,
      0.0f,0.0f,0.0f,0.0f,
      0.0f,0.0f,0.0f,0.0f,
      0.0f,0.0f,0.0f,0.0f);
}

std::ostream& operator<<(std::ostream& sout,const mat4& m)
{
  sout<<m(0,0)<<","<<m(0,1)<<","<<m(0,2)<<","<<m(0,3)<<std::endl;
  sout<<m(1,0)<<","<<m(1,1)<<","<<m(1,2)<<","<<m(1,3)<<std::endl;
  sout<<m(2,0)<<","<<m(2,1)<<","<<m(2,2)<<","<<m(2,3)<<std::endl;
  sout<<m(3,0)<<","<<m(3,1)<<","<<m(3,2)<<","<<m(3,3)<<std::endl;

  return sout;
}

/** Applique mat4 sur un vec3 */
vec3 operator*(const mat4& m,const vec3& p)
{
  vec3 r(m(0,0)*p.x+m(0,1)*p.y+m(0,2)*p.z+m(0,3),
      m(1,0)*p.x+m(1,1)*p.y+m(1,2)*p.z+m(1,3),
      m(2,0)*p.x+m(2,1)*p.y+m(2,2)*p.z+m(2,3));
  r=r/(m(3,0)*p.x+m(3,1)*p.y+m(3,2)*p.z+m(3,3));

  return r;
}


