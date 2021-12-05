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

/*****************************************************************************\
 * init                                                                      *
 \*****************************************************************************/
static void init()
{

  float sommets[]={0.0f,0.0f,0.0f,
    0.8f,0.0f,0.0f,
    0.0f,0.8f,0.0f};

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


  // Chargement du shader
  shader_program_id = glhelper::create_program_from_file(
      "programme_2/src/shader.vert", 
      "programme_2/src/shader.frag"); CHECK_GL_ERROR();
  glUseProgram(shader_program_id);

  //activation de la gestion de la profondeur
  glEnable(GL_DEPTH_TEST); CHECK_GL_ERROR();
}

//Fonction d'affichage
static void display_callback()
{
  //effacement des couleurs du fond d'ecran
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f); CHECK_GL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERROR();

  float translation_x=0.0f;
  float translation_y=0.0f;
  float translation_z=0.0f;

  GLint loc_translation = glGetUniformLocation(shader_program_id, "translation"); CHECK_GL_ERROR();
  if (loc_translation == -1) std::cerr << "Pas de variable uniforme : translation" << std::endl;
  glUniform4f(loc_translation,translation_x,translation_y,translation_z,0.0f); CHECK_GL_ERROR();

  glDrawArrays(GL_TRIANGLES, 0, 3); CHECK_GL_ERROR();

  //Changement de buffer d'affichage pour eviter un effet de scintillement
  glutSwapBuffers();
}

/*****************************************************************************\
 * keyboard_callback                                                         *
 \*****************************************************************************/
static void keyboard_callback(unsigned char key, int, int)
{
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
