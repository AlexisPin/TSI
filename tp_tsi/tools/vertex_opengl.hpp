#ifndef VERTEX_OPENGL_HPP
#define VERTEX_OPENGL_HPP

#include "vec3.hpp"
#include "vec2.hpp"

/** Un vertex de triangle affichage par OpenGL */
struct vertex_opengl
{
  /** Coordonnees 3D du sommet */
  vec3 position;
  /** Normale du sommet */
  vec3 normal;
  /** Couleur du sommet */
  vec3 color;
  /** Coordonnees de texture du sommet */
  vec2 texture;

  /** Constructeur avec vecteurs par defauts */
  vertex_opengl();
  /** Constructeur avec valeur specifiques */
  vertex_opengl(const vec3& position,
      const vec3& normal,
      const vec3& color,
      const vec2& texture);
};

#endif
