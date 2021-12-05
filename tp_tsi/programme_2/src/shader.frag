#version 330 core

// Variable de sortie (sera utilisé comme couleur)
out vec4 color;

//Un Fragment Shader minimaliste
void main (void)
{
  float r=abs(gl_FragCoord.x/600.0);
  float g=abs(gl_FragCoord.y/600.0);

  //couleur du fragment
  color = vec4(r,g,1.0,1.0);
}
