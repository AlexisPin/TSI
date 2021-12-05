#version 330 core

// Variable de sortie (sera utilisé comme couleur)
out vec4 color;

in vec3 coordonnee_3d;

//Un Fragment Shader minimaliste
void main (void)
{
  float x = abs(coordonnee_3d.x);
  float y = abs(coordonnee_3d.y);
  float z = abs(coordonnee_3d.z);


  color = vec4(x,y,z,1.0);
}
