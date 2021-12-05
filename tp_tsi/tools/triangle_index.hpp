
#pragma once

#ifndef TRIANGLE_INDEX_HPP
#define TRIANGLE_INDEX_HPP

/** Une structure stockant 3 indices */

struct triangle_index
{
  /** Indice 0 */
  unsigned int u0;
  /** Indice 1 */
  unsigned int u1;
  /** Indice 2 */
  unsigned int u2;

  /** Constructeur indice (0,0,0) */
  triangle_index();
  /** Constructeur indice (u0,u1,u2) */
  triangle_index(unsigned int u0_param,unsigned int u1_param,unsigned int u2_param);
};







#endif
