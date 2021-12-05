/*****************************************************************************\
 * TP CPE                                                                    *
 * ------
 *
 * Fonctions d'aides pour OpenGL (affichage d'erreur, chargement de shaders, etc)
 *
 \*****************************************************************************/

#ifndef GL_HELPER_H
#define GL_HELPER_H

#include <string>

#define GLEW_STATIC 1
#include <GL/glew.h>

// based on https://blog.nobel-joergensen.com/2013/01/29/debugging-opengl-using-glgeterror/
void _check_gl_error(const char *file, int line);
#define CHECK_GL_ERROR() _check_gl_error(__FILE__, __LINE__)

namespace glhelper
{
  // Renvoie le contenu d'un fichier
  std::string extract_file_content(const std::string& filename);

  // Creation d'un shader
  // shader_source : code GLSL
  // shader_type : enum représentant le type de shader (GL_VERTEX_SHADER ou GL_FRAGMENT_SHADER)
  // Renvoie l'identifiant du shader
  GLuint compile_shader(const char* shader_source, GLenum shader_type);

  // Creation programme GPU (vertex + fragment)
  // vertex_content : Contenu du vertex shader
  // fragment_content : Contenu du fragment shader
  // Renvoie l'identifiant du programme
  GLuint create_program(const std::string& vertex_content, const std::string& fragment_content);

  // Creation programme GPU à partir de fichiers (vertex + fragment)
  // vertex_file : Nom du fichier contenant le vertex shader
  // fragment_file : Nom du fichier contenant le fragment shader
  // Renvoie l'identifiant du programme
  GLuint create_program_from_file(const std::string& vertex_file, const std::string& fragment_file);

  // Fonction pour faire une capture d'écran du FBO courant
  // filename : Nom de la capture d'écran, par défaut utilise un timestamp
  void print_screen(std::string filename = "");

}// namespace glhelper

#endif
