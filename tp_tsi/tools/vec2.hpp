
#pragma once

#ifndef VEC2_HPP
#define VEC2_HPP

#include <iostream>


/** Une structure de vecteur 2D */

struct vec2
{
  /** Coordonnee x */
  float x;
  /** Coordonnee y */
  float y;

  /** Constructeur vecteur (0,0) */
  vec2();
  /** Constructeur vecteur (x,y) */
  vec2(float x_param,float y_param);

  /** Somme vectorielle */
  vec2& operator+=(const vec2& v);
  /** Difference vectorielle */
  vec2& operator-=(const vec2& v);
  /** Multiplication par un scalaire */
  vec2& operator*=(float s);
  /** Division par un scalaire */
  vec2& operator/=(float s);
};


/** Norme d'un vecteur */
float norm(const vec2& v);
/** Produit scalaire */
float dot(const vec2& v0,const vec2& v1);

/** Renvoie un vecteur de meme direction de norme 1 */
vec2 normalize(const vec2& v);

/** Affichage d'un vecteur sur la ligne de commande */
std::ostream& operator<<(std::ostream& sout,const vec2& m);

/** Somme vectorielle */
vec2 operator+(const vec2& v0,const vec2& v1);
/** Difference vectorielle */
vec2 operator-(const vec2& v0,const vec2& v1);
/** Multiplication par un scalaire */
vec2 operator*(const vec2& v0,float s);
/** Multiplication par un scalaire */
vec2 operator*(float s,const vec2& v0);
/** Division par un scalaire */
vec2 operator/(const vec2& v0,float s);




#endif
