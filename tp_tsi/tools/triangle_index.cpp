
#include "triangle_index.hpp"
#include <cmath>
#include <cassert>
#include <iostream>


triangle_index::triangle_index()
  :u0(0),u1(0),u2(0)
{}

triangle_index::triangle_index(unsigned int u0_param,unsigned int u1_param,unsigned int u2_param)
  :u0(u0_param),u1(u1_param),u2(u2_param)
{}


