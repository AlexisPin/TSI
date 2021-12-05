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
#include "mesh.hpp"

/*****************************************************************************\
 * Variables globales
 *
 *
 \*****************************************************************************/

//identifiant du shader
GLuint shader_program_id;
GLuint gui_program_id;

//identifiants pour object 1
GLuint vao_object_1=0;
GLuint vbo_object_1=0;
GLuint vboi_object_1=0;
GLuint texture_id_object_1=0;
int nbr_triangle_object_1;

//identifiants pour object 2
GLuint vao_object_2=0;
GLuint vbo_object_2=0;
GLuint vboi_object_2=0;
GLuint texture_id_object_2=0;
int nbr_triangle_object_2;

//identifiants pour object 3
GLuint vao_object_3=0;
GLuint vbo_object_3=0;
GLuint vboi_object_3=0;
GLuint texture_id_object_3=0;
int nbr_triangle_object_3;


//Matrice de transformation
struct transformation
{
  mat4 rotation;
  vec3 rotation_center;
  vec3 translation;

  transformation():rotation(),rotation_center(),translation(){}
};

//Transformation des modeles
transformation transformation_model_1;
transformation transformation_model_2;
transformation transformation_model_3;

//Transformation de la vue (camera)
transformation transformation_view;

//Matrice de projection
mat4 projection;

//angle de deplacement
float angle_x_model_1 = 0.0f;
float angle_y_model_1 = 0.0f;
float angle_view = 0.0f;

// Text drawing Structure
struct text{
  GLuint vao = 0;              // Vertex array id
  GLuint texture_id = 0;       // Well, texture id...
  std::string value;           // Value of the text to display
  vec2 bottomLeft;
  vec2 topRight;
};
text text_to_draw;


void load_texture(const char* filename,GLuint *texture_id);

void init_model_1();
void init_model_2();
void init_model_3();

void draw_model_1();
void draw_model_2();
void draw_model_3();

void init_text(text *t){
  vec3 p0=vec3( 0.0f, 0.0f, 0.0f);
  vec3 p1=vec3( 0.0f, 1.0f, 0.0f);
  vec3 p2=vec3( 1.0f, 1.0f, 0.0f);
  vec3 p3=vec3( 1.0f, 0.0f, 0.0f);

  vec3 geometrie[4] = {p0, p1, p2, p3}; 
  triangle_index index[2] = { triangle_index(0, 1, 2), triangle_index(0, 2, 3)};

  glGenVertexArrays(1, &(t->vao));                                              CHECK_GL_ERROR();
  glBindVertexArray(t->vao);                                                  CHECK_GL_ERROR();

  GLuint vbo;
  glGenBuffers(1, &vbo);                                                       CHECK_GL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER,vbo);                                          CHECK_GL_ERROR();
  glBufferData(GL_ARRAY_BUFFER,sizeof(geometrie),geometrie,GL_STATIC_DRAW);   CHECK_GL_ERROR();

  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); CHECK_GL_ERROR();

  GLuint vboi;
  glGenBuffers(1,&vboi);                                                      CHECK_GL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi);                                 CHECK_GL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);   CHECK_GL_ERROR();

  load_texture("data/fontB.tga",&t->texture_id);
}

void draw_texts(text *text_t){
   vec2 size = (text_t->topRight - text_t->bottomLeft) / float(text_t->value.size());
  
  //Send uniform parameters
  GLint loc_size = glGetUniformLocation(gui_program_id, "size"); CHECK_GL_ERROR();
  if (loc_size == -1) std::cerr << "Pas de variable uniforme : size" << std::endl;
  glUniform2f(loc_size,size.x, size.y);    CHECK_GL_ERROR();

  glBindVertexArray(text_t->vao);CHECK_GL_ERROR();
  
  for(int i = 0; i < text_t->value.size(); ++i)
  {
    GLint loc_start = glGetUniformLocation(gui_program_id, "start"); CHECK_GL_ERROR();
    if (loc_start == -1) std::cerr << "Pas de variable uniforme : start" << std::endl;
    glUniform2f(loc_start,text_t->bottomLeft.x+i*size.x, text_t->bottomLeft.y);    CHECK_GL_ERROR();

    GLint loc_char = glGetUniformLocation(gui_program_id, "c"); CHECK_GL_ERROR();
    if (loc_char == -1) std::cerr << "Pas de variable uniforme : c" << std::endl;
    glUniform1i(loc_char, (int)text_t->value[i]);    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, text_t->texture_id);                       CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);                    CHECK_GL_ERROR();
  }
}

static void init()
{
  // Chargement du shader
  shader_program_id = glhelper::create_program_from_file(
      "programme_11/src/shader.vert", 
      "programme_11/src/shader.frag"); CHECK_GL_ERROR();
  glUseProgram(shader_program_id);

    //matrice de projection
    projection = matrice_projection(60.0f*M_PI/180.0f,1.0f,0.01f,100.0f);
  GLint loc_projection = glGetUniformLocation(shader_program_id, "projection"); CHECK_GL_ERROR();
  if (loc_projection == -1) std::cerr << "Pas de variable uniforme : projection" << std::endl;
  glUniformMatrix4fv(loc_projection,1,false,pointeur(projection)); CHECK_GL_ERROR();

  //centre de rotation de la 'camera' (les objets sont centres en z=-2)
  transformation_view.rotation_center = vec3(0.0f,0.0f,-2.0f);

  //activation de la gestion de la profondeur
  glEnable(GL_DEPTH_TEST); CHECK_GL_ERROR();

  // Charge modele 1 sur la carte graphique
  init_model_1();
  // Charge modele 2 sur la carte graphique
  init_model_2();
  // Charge modele 3 sur la carte graphique
  init_model_3();

  gui_program_id = glhelper::create_program_from_file("programme_11/src/gui.vert", "programme_11/src/gui.frag"); CHECK_GL_ERROR();

  text_to_draw.value = "CPE";
  text_to_draw.bottomLeft = vec2(-0.2, 0.5);
  text_to_draw.topRight = vec2(0.2, 1);
  init_text(&text_to_draw);

}

//Fonction d'affichage
static void display_callback()
{
  //effacement des couleurs du fond d'ecran
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f); CHECK_GL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERROR();

  glUseProgram(shader_program_id);
  glEnable(GL_DEPTH_TEST);

  // Affecte les parametres uniformes de la vue (identique pour tous les modeles de la scene)
  {
    //envoie de la rotation
    GLint loc_rotation_view = glGetUniformLocation(shader_program_id, "rotation_view"); CHECK_GL_ERROR();
    if (loc_rotation_view == -1) std::cerr << "Pas de variable uniforme : rotation_view" << std::endl;
    glUniformMatrix4fv(loc_rotation_view,1,false,pointeur(transformation_view.rotation)); CHECK_GL_ERROR();

    //envoie du centre de rotation
    vec3 cv = transformation_view.rotation_center;
    GLint loc_rotation_center_view = glGetUniformLocation(shader_program_id, "rotation_center_view"); CHECK_GL_ERROR();
    if (loc_rotation_center_view == -1) std::cerr << "Pas de variable uniforme : rotation_center_view" << std::endl;
    glUniform4f(loc_rotation_center_view , cv.x,cv.y,cv.z , 0.0f); CHECK_GL_ERROR();

    //envoie de la translation
    vec3 tv = transformation_view.translation;
    GLint loc_translation_view = glGetUniformLocation(shader_program_id, "translation_view"); CHECK_GL_ERROR();
    if (loc_translation_view == -1) std::cerr << "Pas de variable uniforme : translation_view" << std::endl;
    glUniform4f(loc_translation_view , tv.x,tv.y,tv.z , 0.0f); CHECK_GL_ERROR();
  }

  // Affiche le modele numero 1 (dinosaure)
  draw_model_1();
  // Affiche le modele numero 2 (sol)
  draw_model_2();
  // Affiche le modele numero 2 (monstre)
  draw_model_3();

  glDisable(GL_DEPTH_TEST);
  glUseProgram(gui_program_id);
  draw_texts(&text_to_draw);

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

    case 'o':
      angle_x_model_1 += d_angle;
      break;
    case 'l':
      angle_x_model_1 -= d_angle;
      break;

    case 'k':
      angle_y_model_1 += d_angle;
      break;
    case 'm':
      angle_y_model_1 -= d_angle;
      break;


    case 's':
      angle_view += d_angle;
      break;
    case 'f':
      angle_view -= d_angle;
      break;


    case 'e':
      transformation_view.translation.z += dz;
      break;
    case 'd':
      transformation_view.translation.z -= dz;
      break;
  }

  transformation_model_1.rotation = matrice_rotation(angle_y_model_1 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_1 , 1.0f,0.0f,0.0f);
  transformation_view.rotation = matrice_rotation(angle_view , 0.0f,1.0f,0.0f);

  {
    //  // Exemple camera fixe :
    //  vec3 pos_camera = vec3(10., 10., 0.);
    //  vec3 target;
    //  transformation_view.rotation_center = vec3();
    //  transformation_view.rotation = matrice_lookat(pos_camera, target, vec3(0., 1., 0.));
    //  transformation_view.translation = extract_translation(transformation_view.rotation);
  }

  {
    //  // Exemple camera fps :
    //  transformation_view.rotation_center = vec3();
    //  vec3 pos_camera = transformation_model_1.translation + vec3(0.,-0.5,-2.) + transformation_model_1.rotation * vec3(0., 0.3, 0.);
    //  vec3 target = pos_camera + transformation_model_1.rotation * vec3(0., 0., 1.)  ;
    //  transformation_view.rotation = matrice_lookat(pos_camera, target, vec3(0., 1., 0.));
    //  transformation_view.translation = extract_translation(transformation_view.rotation);
  }

  {
    //  // Exemple camera troisieme personne :
    //  transformation_view.rotation_center = vec3();
    //  vec3 target = transformation_model_1.translation + vec3(0.,-0.5,-2.);
    //  vec3 pos_camera = target - transformation_model_1.rotation * vec3(0., -3., 5.)  ;
    //  transformation_view.rotation = matrice_lookat(pos_camera, target, vec3(0., 1., 0.));
    //  transformation_view.translation = extract_translation(transformation_view.rotation);
  }
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
      transformation_model_1.translation.y += dL; //rotation avec la touche du haut
      break;
    case GLUT_KEY_DOWN:
      transformation_model_1.translation.y -= dL; //rotation avec la touche du bas
      break;
    case GLUT_KEY_LEFT:
      transformation_model_1.translation.x -= dL; //rotation avec la touche de gauche
      break;
    case GLUT_KEY_RIGHT:
      transformation_model_1.translation.x += dL; //rotation avec la touche de droite
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

void draw_model_1()
{

  //envoie des parametres uniformes
  {
    GLint loc_rotation_model = glGetUniformLocation(shader_program_id, "rotation_model"); CHECK_GL_ERROR();
    if (loc_rotation_model == -1) std::cerr << "Pas de variable uniforme : rotation_model" << std::endl;
    glUniformMatrix4fv(loc_rotation_model,1,false,pointeur(transformation_model_1.rotation));    CHECK_GL_ERROR();

    vec3 c = transformation_model_1.rotation_center;
    GLint loc_rotation_center_model = glGetUniformLocation(shader_program_id, "rotation_center_model"); CHECK_GL_ERROR();
    if (loc_rotation_center_model == -1) std::cerr << "Pas de variable uniforme : rotation_center_model" << std::endl;
    glUniform4f(loc_rotation_center_model , c.x,c.y,c.z , 0.0f);                                 CHECK_GL_ERROR();

    vec3 t = transformation_model_1.translation;
    GLint loc_translation_model = glGetUniformLocation(shader_program_id, "translation_model"); CHECK_GL_ERROR();
    if (loc_translation_model == -1) std::cerr << "Pas de variable uniforme : translation_model" << std::endl;
    glUniform4f(loc_translation_model , t.x,t.y,t.z , 0.0f);                                     CHECK_GL_ERROR();
  }

  glBindVertexArray(vao_object_1);CHECK_GL_ERROR();

  //affichage
  {
    glBindTexture(GL_TEXTURE_2D, texture_id_object_1);                             CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_1, GL_UNSIGNED_INT, 0);     CHECK_GL_ERROR();
  }

}

void draw_model_2()
{

  //envoie des parametres uniformes
  {
    GLint loc_rotation_model = glGetUniformLocation(shader_program_id, "rotation_model"); CHECK_GL_ERROR();
    if (loc_rotation_model == -1) std::cerr << "Pas de variable uniforme : rotation_model" << std::endl;
    glUniformMatrix4fv(loc_rotation_model,1,false,pointeur(transformation_model_2.rotation));    CHECK_GL_ERROR();

    vec3 c = transformation_model_2.rotation_center;
    GLint loc_rotation_center_model = glGetUniformLocation(shader_program_id, "rotation_center_model"); CHECK_GL_ERROR();
    if (loc_rotation_center_model == -1) std::cerr << "Pas de variable uniforme : rotation_center_model" << std::endl;
    glUniform4f(loc_rotation_center_model , c.x,c.y,c.z , 0.0f);                                 CHECK_GL_ERROR();

    vec3 t = transformation_model_2.translation;
    GLint loc_translation_model = glGetUniformLocation(shader_program_id, "translation_model"); CHECK_GL_ERROR();
    if (loc_translation_model == -1) std::cerr << "Pas de variable uniforme : translation_model" << std::endl;
    glUniform4f(loc_translation_model , t.x,t.y,t.z , 0.0f);                                     CHECK_GL_ERROR();
  }

  glBindVertexArray(vao_object_2);CHECK_GL_ERROR();

  //affichage
  {
    glBindTexture(GL_TEXTURE_2D, texture_id_object_2);                             CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_2, GL_UNSIGNED_INT, 0);     CHECK_GL_ERROR();
  }
}

void draw_model_3()
{
  //envoie des parametres uniformes
  {
    GLint loc_rotation_model = glGetUniformLocation(shader_program_id, "rotation_model"); CHECK_GL_ERROR();
    if (loc_rotation_model == -1) std::cerr << "Pas de variable uniforme : rotation_model" << std::endl;
    glUniformMatrix4fv(loc_rotation_model,1,false,pointeur(transformation_model_3.rotation));    CHECK_GL_ERROR();

    vec3 c = transformation_model_3.rotation_center;
    GLint loc_rotation_center_model = glGetUniformLocation(shader_program_id, "rotation_center_model"); CHECK_GL_ERROR();
    if (loc_rotation_center_model == -1) std::cerr << "Pas de variable uniforme : rotation_center_model" << std::endl;
    glUniform4f(loc_rotation_center_model , c.x,c.y,c.z , 0.0f);                                 CHECK_GL_ERROR();

    vec3 t = transformation_model_3.translation;
    GLint loc_translation_model = glGetUniformLocation(shader_program_id, "translation_model"); CHECK_GL_ERROR();
    if (loc_translation_model == -1) std::cerr << "Pas de variable uniforme : translation_model" << std::endl;
    glUniform4f(loc_translation_model , t.x,t.y,t.z , 0.0f);                                     CHECK_GL_ERROR();
  }

  glBindVertexArray(vao_object_3); CHECK_GL_ERROR();

  //affichage
  {
    glBindTexture(GL_TEXTURE_2D, texture_id_object_3);                             CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_3, GL_UNSIGNED_INT, 0);     CHECK_GL_ERROR();
  }
}


void init_model_1()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_obj_file("data/stegosaurus.obj");

  // Affecte une transformation sur les sommets du maillage
  float s = 0.2f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f,-0.9f,
      0.0f, 0.0f,   s ,-2.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  transformation_model_1.rotation_center = vec3(0.0f,-0.5f,-2.0f);

  // Calcul automatique des normales du maillage
  update_normals(&m);
  // Les sommets sont affectes a une couleur blanche
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  //attribution d'une liste d'état (1 indique la création d'une seule liste)
  glGenVertexArrays(1, &vao_object_1);
  glBindVertexArray(vao_object_1);

  //attribution d'un buffer de donnees (1 indique la création d'un buffer)
  glGenBuffers(1,&vbo_object_1); CHECK_GL_ERROR();
  //affectation du buffer courant
  glBindBuffer(GL_ARRAY_BUFFER,vbo_object_1); CHECK_GL_ERROR();
  //copie des donnees des sommets sur la carte graphique
  glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

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
  glGenBuffers(1,&vboi_object_1); CHECK_GL_ERROR();
  //affectation du buffer courant (buffer d'indice)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_1); CHECK_GL_ERROR();
  //copie des indices sur la carte graphique
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

  // Nombre de triangles de l'objet 1
  nbr_triangle_object_1 = m.connectivity.size();

  // Chargement de la texture
  load_texture("data/stegosaurus.tga",&texture_id_object_1);


}

void init_model_2()
{
  //Creation manuelle du model 2

  //coordonnees geometriques des sommets
  vec3 p0=vec3(-25.0f,-0.9f,-25.0f);
  vec3 p1=vec3( 25.0f,-0.9f,-25.0f);
  vec3 p2=vec3( 25.0f,-0.9f, 25.0f);
  vec3 p3=vec3(-25.0f,-0.9f, 25.0f);

  //normales pour chaque sommet
  vec3 n0=vec3(0.0f,1.0f,0.0f);
  vec3 n1=n0;
  vec3 n2=n0;
  vec3 n3=n0;

  //couleur pour chaque sommet
  vec3 c0=vec3(1.0f,1.0f,1.0f);
  vec3 c1=c0;
  vec3 c2=c0;
  vec3 c3=c0;

  //texture du sommet
  vec2 t0=vec2(0.0f,0.0f);
  vec2 t1=vec2(1.0f,0.0f);
  vec2 t2=vec2(1.0f,1.0f);
  vec2 t3=vec2(0.0f,1.0f);

  vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
  vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
  vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
  vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);


  //tableau entrelacant coordonnees-normales
  vertex_opengl geometrie[]={v0,v1,v2,v3};


  //indice des triangles
  triangle_index tri0=triangle_index(0,1,2);
  triangle_index tri1=triangle_index(0,2,3);
  triangle_index index[]={tri0,tri1};
  nbr_triangle_object_2 = 2;


  //attribution d'une liste d'état (1 indique la création d'une seule liste)
  glGenVertexArrays(1, &vao_object_2);
  glBindVertexArray(vao_object_2);
  //attribution d'un buffer de donnees (1 indique la création d'un buffer)
  glGenBuffers(1,&vbo_object_2);                                             CHECK_GL_ERROR();
  //affectation du buffer courant
  glBindBuffer(GL_ARRAY_BUFFER,vbo_object_2);                                CHECK_GL_ERROR();
  //copie des donnees des sommets sur la carte graphique
  glBufferData(GL_ARRAY_BUFFER,sizeof(geometrie),geometrie,GL_STATIC_DRAW);  CHECK_GL_ERROR();

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
  glGenBuffers(1,&vboi_object_2);                                            CHECK_GL_ERROR();
  //affectation du buffer courant (buffer d'indice)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_2);                       CHECK_GL_ERROR();
  //copie des indices sur la carte graphique
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);  CHECK_GL_ERROR();

  // Chargement de la texture
  load_texture("data/grass.tga",&texture_id_object_2);

}


void init_model_3()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_off_file("data/armadillo_light.off");

  // Affecte une transformation sur les sommets du maillage
  float s = 0.01f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 1.0f,
      0.0f,    s, 0.0f,-0.2f,
      0.0f, 0.0f,   s ,-2.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,matrice_rotation(M_PI/2.0f,1.0f,0.0f,0.0f));
  apply_deformation(&m,matrice_rotation(1.5*M_PI/2.0f,0.0f,1.0f,0.0f));
  apply_deformation(&m,transform);

  // Calcul automatique des normales du maillage
  update_normals(&m);
  // Les sommets sont affectes a une couleur blanche
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  //attribution d'une liste d'état (1 indique la création d'une seule liste)
  glGenVertexArrays(1, &vao_object_3);
  glBindVertexArray(vao_object_3);
  //attribution d'un buffer de donnees (1 indique la création d'un buffer)
  glGenBuffers(1,&vbo_object_3);                                 CHECK_GL_ERROR();
  //affectation du buffer courant
  glBindBuffer(GL_ARRAY_BUFFER,vbo_object_3); CHECK_GL_ERROR();
  //copie des donnees des sommets sur la carte graphique
  glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

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
  glGenBuffers(1,&vboi_object_3); CHECK_GL_ERROR();
  //affectation du buffer courant (buffer d'indice)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_3); CHECK_GL_ERROR();
  //copie des indices sur la carte graphique
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

  // Nombre de triangles de l'objet 3
  nbr_triangle_object_3 = m.connectivity.size();

  // Chargement de la texture
  load_texture("data/white.tga",&texture_id_object_3);


}

void load_texture(const char* filename,GLuint *texture_id)
{
  // Chargement d'une texture (seul les textures tga sont supportes)
  Image  *image = image_load_tga(filename);
  if (image) //verification que l'image est bien chargee
  {

    //Creation d'un identifiant pour la texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); CHECK_GL_ERROR();
    glGenTextures(1, texture_id); CHECK_GL_ERROR();

    //Selection de la texture courante a partir de son identifiant
    glBindTexture(GL_TEXTURE_2D, *texture_id); CHECK_GL_ERROR();

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
  glUniform1i (loc_texture,0); CHECK_GL_ERROR();
}
