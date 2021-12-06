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
GLuint vboi = 0;

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
float sommets[] = { 0.0f,0.0f,0.0f,
1.0f,0.0f,0.0f,
0.0f,1.0f,0.0f,
0.0f,0.0f,1.0f
};

unsigned int index[] = { 0,1,2,
0,1,3 };
  //attribution d'une liste d'état (1 indique la création d'une seule liste)
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //attribution d'un buffer de donnees (1 indique la création d'un seul buffer)
  glGenBuffers(1,&vbo); CHECK_GL_ERROR();
  //affectation du buffer courant
  glBindBuffer(GL_ARRAY_BUFFER,vbo); CHECK_GL_ERROR();
  //copie des donnees des sommets sur la carte graphique
  glBufferData(GL_ARRAY_BUFFER,sizeof(sommets),sommets,GL_STATIC_DRAW); CHECK_GL_ERROR();

  // Active l'utilisation des données de positions (le 0 correspond à la location dans le vertex shader)
  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  // Indique comment le buffer courant (dernier vbo "bindé") est utilisé pour les positions des sommets
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); CHECK_GL_ERROR();

  //attribution d’un autre buffer de donnees
  glGenBuffers(1, &vboi);
  //affectation du buffer courant (buffer d’indice)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboi);
  //copie des indices sur la carte graphique
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index),
      index, GL_STATIC_DRAW);

  // Chargement du shader
  shader_program_id = glhelper::create_program_from_file(
      "programme_5/src/shader.vert", 
      "programme_5/src/shader.frag"); CHECK_GL_ERROR();
  glUseProgram(shader_program_id);

  //matrice de projection
  projection=matrice_projection(50.0f*M_PI/180.0f,1.0f,0.5f,10.0f);
  GLint loc_projection = glGetUniformLocation(shader_program_id, "projection"); CHECK_GL_ERROR();
  if (loc_projection == -1) std::cerr << "Pas de variable uniforme : projection" << std::endl;
  glUniformMatrix4fv(loc_projection,1,false,pointeur(projection)); CHECK_GL_ERROR();

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

  //glDrawArrays(GL_TRIANGLES, 0, 6); CHECK_GL_ERROR();
  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, 0);

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
