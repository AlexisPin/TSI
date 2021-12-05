#ifndef MESH_HPP
#define MESH_HPP

#include "vertex_opengl.hpp"
#include "triangle_index.hpp"
#include <vector>
#include <string>

struct mat4;
struct vec3;

/** une structure de maillage */
struct mesh
{
  /** les informations (coord,couleur,normale,texture) par sommets */
  std::vector<vertex_opengl> vertex;

  /** la connectivite des triangles */
  std::vector<triangle_index> connectivity;
};

/** chargement d'un fichier off */
mesh load_off_file(const std::string& filename);
/** chargement d'un fichier obj (gere potentiellement la texture) */
mesh load_obj_file(const std::string& filename);

/** calcule les normales du maillage passe en parametre */
void update_normals(mesh* m);
/** donne une couleur uniforme au maillage passe en parametre */
void fill_color(mesh* m,const vec3& color);
/** chaque sommet du maillage recoit une couleur correspondante a sa normale */
void fill_color_normal(mesh* m);

/** applique la matrice passee en parametre a l'ensemble des sommets du maillage */
void apply_deformation(mesh* m,const mat4 T);
/** inverse le sens de toutes les normales du maillage */
void invert_normals(mesh* m);

#endif
