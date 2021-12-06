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

  if((x-0.295)*(x-0.295)+(y-0.295)*(y-0.295)<=0.295*0.295){
	color = vec4(y+0.4,x+0.3,1.0,1.0);}

  if((x-0.295)*(x-0.295)+(y-0.295)*(y-0.295)<=0.1475*0.1475)
    discard;
}
