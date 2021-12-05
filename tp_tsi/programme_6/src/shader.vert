#version 330 core

layout (location = 0) in vec3 position;

uniform vec4 translation;
uniform mat4 rotation;
uniform mat4 projection;

out vec3 coordonnee_3d;

//Un Vertex Shader minimaliste
void main (void)
{
  //Les coordonnees 3D du sommet
  coordonnee_3d = position;

  //Coordonnees du sommet
  vec4 p = rotation*vec4(position, 1.0)+translation;

  //Projection du sommet
  p = projection*p;

  //position dans l'espace ecran
  gl_Position = p;
}
