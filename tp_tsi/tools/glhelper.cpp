/*****************************************************************************\
 * TP CPE                                                                    *
 * ------
 *
 * Fonctions d'aides pour OpenGL (affichage d'erreur, chargement de shaders, etc)
 *
 \*****************************************************************************/

#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>

#include "glhelper.hpp" 

/*****************************************************************************\
 * print_opengl_error                                                        *
 \*****************************************************************************/
void _check_gl_error(const char *file, int line) 
{
  GLenum err = glGetError(); 
  while(err != GL_NO_ERROR) {
    std::string error;
    switch(err) {
      case GL_INVALID_ENUM:                   error="INVALID_ENUM";                   break;
      case GL_INVALID_VALUE:                  error="INVALID_VALUE";                  break;
      case GL_INVALID_OPERATION:              error="INVALID_OPERATION";              break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
      case GL_OUT_OF_MEMORY:                  error="OUT_OF_MEMORY";                  break;
      case GL_STACK_UNDERFLOW:                error="STACK_UNDERFLOW";             break;
      case GL_STACK_OVERFLOW:                 error="STACK_OVERFLOW";              break;
    }
    std::cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<< std::endl;
    err=glGetError(); 
  }
}

namespace glhelper
{
  /*****************************************************************************\
   * Extract file content
   \*****************************************************************************/
  std::string extract_file_content(const std::string& filename)
  {
    std::ifstream ifs(filename.c_str());
    if(!ifs)
    {
      std::cerr << "-------------------------\n";
      std::cerr << "Error reading file: " << filename << std::endl;
      std::cerr << "Executez vous le programme dans le bon repertoire ?"<<std::endl;
      std::cerr << "-------------------------\n";
    }
    return std::string((std::istreambuf_iterator<char>(ifs)),
        (std::istreambuf_iterator<char>()));
  }

  /*****************************************************************************\
   * Compile a shader and check compilation with log
   \*****************************************************************************/
  GLuint compile_shader(const char* shader_content, GLenum shader_type)
  {
    GLuint shader_id = glCreateShader(shader_type); CHECK_GL_ERROR();
    glShaderSource(shader_id, 1, &shader_content, nullptr); CHECK_GL_ERROR();
    glCompileShader(shader_id); CHECK_GL_ERROR();

    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success); CHECK_GL_ERROR();
    if(!success)
    {
      int log_length;
      glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length); CHECK_GL_ERROR();
      if(log_length>1)
      {
        char* log = new char[log_length];
        glGetShaderInfoLog(shader_id, log_length, nullptr, log); CHECK_GL_ERROR();
        std::cerr << "-------------------------\n";
        std::cerr << "Error compiling shader: \n";
        std::cerr << shader_content << "\n";
        std::cerr << "------\n";
        std::cerr << log << "\n";
        std::cerr << "-------------------------" << std::endl;
        delete[] log;
      }
    }
    return shader_id;
  } 

  /*****************************************************************************\
   * Create a program and check validity with log 
   \*****************************************************************************/
  GLuint create_program(const std::string& vs_content, const std::string& fs_content)
  {
    GLuint vs_id = compile_shader(vs_content.c_str(),GL_VERTEX_SHADER);
    GLuint fs_id = compile_shader(fs_content.c_str(),GL_FRAGMENT_SHADER);

    GLuint program_id = glCreateProgram(); CHECK_GL_ERROR();
    glAttachShader(program_id, vs_id); CHECK_GL_ERROR();
    glAttachShader(program_id, fs_id); CHECK_GL_ERROR();
    glLinkProgram(program_id); CHECK_GL_ERROR();

    int success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success); CHECK_GL_ERROR();
    if(!success)
    {
      int log_length;
      glGetShaderiv(program_id, GL_INFO_LOG_LENGTH, &log_length); CHECK_GL_ERROR();
      if(log_length>1)
      {
        char* log = new char[log_length];
        glGetShaderInfoLog(program_id, log_length, nullptr, log); CHECK_GL_ERROR();
        std::cerr << "-------------------------\n";
        std::cerr << "Error linking program: \n" << log << "\n";
        std::cerr << "-------------------------" << std::endl;
        delete[] log;
      }
    }

    glDeleteShader(vs_id); CHECK_GL_ERROR();
    glDeleteShader(fs_id); CHECK_GL_ERROR();

    return program_id;
  }

  /*****************************************************************************\
   * Create a program from files and check validity with log 
   \*****************************************************************************/
  GLuint create_program_from_file(
      const std::string& vs_file,
      const std::string& fs_file)
  {
    return create_program(extract_file_content(vs_file), extract_file_content(fs_file));
  }

  /*****************************************************************************\
   * Create an image from the current FBO 
   \*****************************************************************************/
  void print_screen(std::string filename)
  {
    if(filename.empty())
    {
      std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      filename.resize(30);
      auto size = std::strftime(&filename[0], filename.size(), "%Y%m%d_%H%M%S", std::localtime(&now));
      filename.resize(size);
    }
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int w(viewport[2]), h(viewport[3]);
    unsigned char* pixels = new unsigned char[3 * w * h];
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels); CHECK_GL_ERROR();

    if(filename.substr(filename.find_last_of(".")+1) != "ppm")
      filename += ".ppm";

    std::ofstream ofs(filename);
    if(ofs)
    {
      ofs << "P3\n" << w << " " << h << "\n255\n";
      for(auto y = 0; y < h; ++y)
      {
        for(auto x = 0; x < w; ++x)
        {
          ofs << (int) pixels[3*(x+(h-y-1)*w)+0] << " ";
          ofs << (int) pixels[3*(x+(h-y-1)*w)+1] << " ";
          ofs << (int) pixels[3*(x+(h-y-1)*w)+2] << " ";
        }
        ofs << "\n";
      }
    }

    delete [] pixels;
  }
}
