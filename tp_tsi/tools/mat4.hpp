
#ifndef MAT4_HPP
#define MAT4_HPP

#include <iostream>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

struct vec3;

/** Une structure de matrice de taille 4x4 */

struct mat4
{
  /** Initialise la matrice a l'identitee */
  mat4();
  /** Initialisation par valeur */
  mat4(float x00,float x01,float x02,float x03,
      float x10,float x11,float x12,float x13,
      float x20,float x21,float x22,float x23,
      float x30,float x31,float x32,float x33);

  /** Obtention des valeurs de la matrice sous la forme m(x,y) */
  float operator()(int x,int y) const;

  /** Modification des valeurs de la matrice sous la forme m(x,y)=... */
  float& operator()(int x,int y);


  /** Donnees de la matrice sous forme d'un tableau */
  float M[4*4];
};

/** Produit de matrice */
mat4 operator*(const mat4& m1,const mat4& m2);

/** Applique mat4 sur un vec3 */
vec3 operator*(const mat4& m1,const vec3& m2);

/** Recupere un pointeur sur les donnees de la matrice */
const float *pointeur(const mat4& m);

/** Calcule la transposee d'une matrice */
mat4 transpose(const mat4& m);

/** Construit une matrice de rotation ayant pour axe: (axe_x,axe_y,axe_z) et l'angle donne */
mat4 matrice_rotation(float angle,float axe_x,float axe_y,float axe_z);

/** Construit une matrice de projection */
//mat4 matrice_projection(float L,float H,float d_min,float d_max);
mat4 matrice_projection(float fov,float aspect,float d_min,float d_max);

/** Construit une matrice de rotation pour une camera de position eye, regardant dans la direction center
 * et ayant pour position haute up 
 * WARNING : Extraire la partie translation à l'aide de la fonction extract_translation */
mat4 matrice_lookat(const vec3& eye, const vec3& center, const vec3& up);

/** Extrait et renvoie la translation d'un matrice de transformation */
vec3 extract_translation(mat4& m);

/** Construit une matrice n'ayant que des zeros */
mat4 matrice_zeros();

/** Affichage d'une matrice sur la ligne de commande */
std::ostream& operator<<(std::ostream& sout,const mat4& m);

#endif
