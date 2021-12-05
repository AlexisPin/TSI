/*****************************************************************************\
 * TP CPE, 4ETI, TP synthese d'images
 * --------------
 *
 * Programme principal des appels OpenGL
 \*****************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#define GLEW_STATIC 1
#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#define MACOSX_COMPATIBILITY GLUT_3_2_CORE_PROFILE
#else
#include <GL/glut.h>
#define MACOSX_COMPATIBILITY 0
#endif

#include "glhelper.hpp"
#include "mat4.hpp"
#include "vec3.hpp"
#include "vec2.hpp"
#include "triangle_index.hpp"
#include "vertex_opengl.hpp"
#include "image.hpp"

/*****************************************************************************\
 * Variables globales
 *
 *
 \*****************************************************************************/

//identifiant du shader
GLuint shader_program_id;

//l'identifiant de l'objet 3D
GLuint vao=0;
GLuint vbo=0;
GLuint vboi=0;
GLuint texture_id=0;

//les parametres de translations
float translation_x=0.0f;
float translation_y=0.0f;
float translation_z=-3.0f;

float angle_x=0.0f;
float angle_y=0.0f;

//Matrice de rotation
mat4 rotation;

//Matrice de projection
mat4 projection;

/*****************************************************************************\
 * init                                                                      *
 \*****************************************************************************/
static void init()
{

  //coordonnees geometriques des sommets
  vec3 p0=vec3(0.0f,0.0f,0.0f);
  vec3 p1=vec3(1.0f,0.0f,0.0f);
  vec3 p2=vec3(0.0f,1.0f,0.0f);
  vec3 p3=vec3(0.8f,0.8f,0.5f);

  //normales pour chaque sommet
  vec3 n0=vec3(0.0f,0.0f,1.0f);
  vec3 n1=vec3(-0.25f,-0.25f,0.8535f);
  vec3 n2=vec3(-0.25f,-0.25f,0.8535f);
  vec3 n3=vec3(-0.5f,-0.5f,0.707);

  //couleur pour chaque sommet
  vec3 c0=vec3(0.0f,0.0f,0.0f);
  vec3 c1=vec3(1.0f,0.0f,0.0f);
  vec3 c2=vec3(0.0f,1.0f,0.0f);
  vec3 c3=vec3(1.0f,1.0f,0.0f);

  //texture du sommet
  vec2 t0=vec2(0.0f,0.0f);
  vec2 t1=vec2(1.0f,0.0f);
  vec2 t2=vec2(0.0f,1.0f);
  vec2 t3=vec2(1.0f,1.0f);

  vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
  vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
  vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
  vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);


  //tableau entrelacant coordonnees-normales-couleurs-textures
  vertex_opengl geometrie[]={v0,v1,v2,v3};


  //indice des triangles
  triangle_index tri0=triangle_index(0,1,2);
  triangle_index tri1=triangle_index(1,3,2);
  triangle_index index[]={tri0,tri1};

  //attribution d'une liste d'état (1 indique la création d'une seule liste)
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //attribution d'un buffer de donnees (1 indique la création d'un seul buffer)
  glGenBuffers(1,&vbo); CHECK_GL_ERROR();
  //affectation du buffer courant
  glBindBuffer(GL_ARRAY_BUFFER,vbo); CHECK_GL_ERROR();
  //copie des donnees des sommets sur la carte graphique
  glBufferData(GL_ARRAY_BUFFER,sizeof(geometrie),geometrie,GL_STATIC_DRAW); CHECK_GL_ERROR();

  // Active l'utilisation des données de positions (le 0 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les positions des sommets
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), 0); CHECK_GL_ERROR();

  // Active l'utilisation des données de normales (le 1 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(1); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les normales des sommets
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vertex_opengl), (void*)sizeof(vec3)); CHECK_GL_ERROR();

  // Active l'utilisation des données de couleurs (le 2 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(2); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les couleurs des sommets
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(2*sizeof(vec3))); CHECK_GL_ERROR();

  // Active l'utilisation des données de textures (le 3 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(3); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les textures des sommets
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(3*sizeof(vec3))); CHECK_GL_ERROR();

  //attribution d'un autre buffer de donnees
  glGenBuffers(1,&vboi); CHECK_GL_ERROR();
  //affectation du buffer courant (buffer d'indice)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi); CHECK_GL_ERROR();
  //copie des indices sur la carte graphique
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW); CHECK_GL_ERROR();

  // Chargement du shader
  shader_program_id = glhelper::create_program_from_file(
      "programme_8/src/shader.vert", 
      "programme_8/src/shader.frag"); CHECK_GL_ERROR();
  glUseProgram(shader_program_id);

    //matrice de projection
    projection=matrice_projection(50.0f*M_PI/180.0f,1.0f,0.5f,10.0f);
  GLint loc_projection = glGetUniformLocation(shader_program_id, "projection"); CHECK_GL_ERROR();
  if (loc_projection == -1) std::cerr << "Pas de variable uniforme : projection" << std::endl;
  glUniformMatrix4fv(loc_projection,1,false,pointeur(projection)); CHECK_GL_ERROR();


  // Chargement d'une texture (seul les textures tga sont supportes)
  Image  *image = image_load_tga("data/unicorn.tga");
  if (image) //verification que l'image est bien chargee
  {

    //Creation d'un identifiant pour la texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); CHECK_GL_ERROR();
    glGenTextures(1, &texture_id); CHECK_GL_ERROR();

    //Selection de la texture courante a partir de son identifiant
    glBindTexture(GL_TEXTURE_2D, texture_id); CHECK_GL_ERROR();

    //Parametres de la texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERROR();

    //Envoie de l'image en memoire video
    if(image->type==IMAGE_TYPE_RGB){ //image RGB
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data); CHECK_GL_ERROR();}
    else if(image->type==IMAGE_TYPE_RGBA){ //image RGBA
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data); CHECK_GL_ERROR();}
    else{
      std::cout<<"Image type not handled"<<std::endl;}

    delete image;
  }
  else
  {
    std::cerr<<"Erreur chargement de l'image, etes-vous dans le bon repertoire?"<<std::endl;
    abort();
  }
GLint loc_texture = glGetUniformLocation(shader_program_id, "texture"); CHECK_GL_ERROR();
  if (loc_texture == -1) std::cerr << "Pas de variable uniforme : texture" << std::endl;
  glUniform1i (loc_texture, 0); CHECK_GL_ERROR();

  //activation de la gestion de la profondeur
  glEnable(GL_DEPTH_TEST); CHECK_GL_ERROR();
}

//Fonction d'affichage
static void display_callback()
{
  //effacement des couleurs du fond d'ecran
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f); CHECK_GL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERROR();

  //envoie des parametres sur la carte graphique
  GLint loc_rotation = glGetUniformLocation(shader_program_id, "rotation"); CHECK_GL_ERROR();
  if (loc_rotation == -1) std::cerr << "Pas de variable uniforme : rotation" << std::endl;
  glUniformMatrix4fv(loc_rotation,1,false,pointeur(rotation)); CHECK_GL_ERROR();
  GLint loc_translation = glGetUniformLocation(shader_program_id, "translation"); CHECK_GL_ERROR();
  if (loc_translation == -1) std::cerr << "Pas de variable uniforme : translation" << std::endl;
  glUniform4f(loc_translation,translation_x,translation_y,translation_z,0.0f); CHECK_GL_ERROR();

  glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, 0); CHECK_GL_ERROR();

  //Changement de buffer d'affichage pour eviter un effet de scintillement
  glutSwapBuffers();
}

/*****************************************************************************\
 * keyboard_callback                                                         *
 \*****************************************************************************/
static void keyboard_callback(unsigned char key, int, int)
{
  float d_angle=0.1f;
  float dz=0.5f;

  //quitte le programme si on appuie sur les touches 'q', 'Q', ou 'echap'
  //enregistre l'image si on appuie sur la touche 'p'
  switch (key)
  {
    case 'p':
      glhelper::print_screen();
      break;
    case 'q':
    case 'Q':
    case 27:
      exit(0);
      break;

    case 'i':
      angle_x+=d_angle;
      break;
    case 'k':
      angle_x-=d_angle;
      break;
    case 'j':
      angle_y+=d_angle;
      break;
    case 'l':
      angle_y-=d_angle;
      break;

    case 'y':
      translation_z+=dz;
      break;
    case 'h':
      translation_z-=dz;
      break;
  }

  mat4 rotation_x=matrice_rotation(angle_x,1.0f,0.0f,0.0f);
  mat4 rotation_y=matrice_rotation(angle_y,0.0f,1.0f,0.0f);
  rotation=rotation_x*rotation_y;
}

/*****************************************************************************\
 * special_callback                                                          *
 \*****************************************************************************/
static void special_callback(int key, int,int)
{
  float dL=0.03f;
  switch (key)
  {
    case GLUT_KEY_UP:
      translation_y+=dL; //rotation avec la touche du haut
      break;
    case GLUT_KEY_DOWN:
      translation_y-=dL; //rotation avec la touche du bas
      break;
    case GLUT_KEY_LEFT:
      translation_x-=dL; //rotation avec la touche de gauche
      break;
    case GLUT_KEY_RIGHT:
      translation_x+=dL; //rotation avec la touche de droite
      break;
  }
}


/*****************************************************************************\
 * timer_callback                                                            *
 \*****************************************************************************/
static void timer_callback(int)
{
  //demande de rappel de cette fonction dans 25ms
  glutTimerFunc(25, timer_callback, 0);

  //reactualisation de l'affichage
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  //**********************************************//
  //Lancement des fonctions principales de GLUT
  //**********************************************//

  //initialisation
  glutInit(&argc, argv);

  //Mode d'affichage (couleur, gestion de profondeur, ...)
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);

  //Taille de la fenetre a l'ouverture
  glutInitWindowSize(600, 600);

  //Titre de la fenetre
  glutCreateWindow("OpenGL");

  //Fonction de la boucle d'affichage
  glutDisplayFunc(display_callback);

  //Fonction de gestion du clavier
  glutKeyboardFunc(keyboard_callback);

  //Fonction des touches speciales du clavier (fleches directionnelles)
  glutSpecialFunc(special_callback);

  //Fonction d'appel d'affichage en chaine
  glutTimerFunc(25, timer_callback, 0);

  //Option de compatibilité
  glewExperimental = true;

  //Initialisation des fonctions OpenGL
  glewInit();

  //Affiche la version openGL utilisée
  std::cout << "OpenGL: " << (GLchar *)(glGetString(GL_VERSION)) << std::endl;

  //Notre fonction d'initialisation des donnees et chargement des shaders
  init();

  //Lancement de la boucle (infinie) d'affichage de la fenetre
  glutMainLoop();

  //Plus rien n'est execute apres cela
  return 0;
}
