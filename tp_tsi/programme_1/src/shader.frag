#version 330 core

// Variable de sortie (sera utilisé comme couleur)
out vec4 color;

//Un Fragment Shader minimaliste

void main (void)
{
float x=gl_FragCoord.x/600.0-0.5;
float y=gl_FragCoord.y/600.0-0.5;
float r = 0.0;
float g=1.0;
if(pow(pow(x,2) + pow(y,2),0.5) <= 0.25)
{
r=1.0;
g=0.0;
}
else
{
r = 0.0;
g = 1.0;
}
  color = vec4(r,g,0.0,0.0);
}