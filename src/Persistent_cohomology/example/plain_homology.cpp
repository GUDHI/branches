/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Marc Glisse
 *
 *    Copyright (C) 2015  INRIA Saclay - Ile-de-France (France)
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gudhi/Simplex_tree.h>
#include <gudhi/Persistent_cohomology.h>

#include <iostream>

using namespace Gudhi;

/* We could perfectly well use the default Simplex_tree<> (which uses
 * Simplex_tree_options_full_featured), the following simply demonstrates
 * how to save on storage by not storing a filtration value.  */

struct MyOptions : Simplex_tree_options_full_featured {
  // Implicitly use 0 as filtration value for all simplices
  static const bool store_filtration = false;
  // The persistence algorithm needs this
  static const bool store_key = true;
  // I have few vertices
  typedef short Vertex_handle;
};
typedef Simplex_tree<MyOptions> ST;

int main() {
  ST st;

  /* Complex to build. */
  /*    1   3          */
  /*    o---o          */
  /*   /X\ /           */
  /*  o---o   o        */
  /*  2   0   4        */

  const short triangle012[] = {0, 1, 2};
  const short edge03[] = {0, 3};
  const short edge13[] = {1, 3};
  const short vertex4[] = {4};
  st.insert_simplex_and_subfaces(triangle012);
  st.insert_simplex_and_subfaces(edge03);
  st.insert_simplex(edge13);
  st.insert_simplex(vertex4);
  // FIXME: Remove this line
  st.set_dimension(2);

  // Sort the simplices in the order of the filtration
  st.initialize_filtration();

  // Class for homology computation
  persistent_cohomology::Persistent_cohomology<ST, persistent_cohomology::Field_Zp> pcoh(st);

  // Initialize the coefficient field Z/2Z for homology
  pcoh.init_coefficients(2);

  // Compute the persistence diagram of the complex
  pcoh.compute_persistent_cohomology();

  // Print the result. The format is, on each line: 2 dim 0 inf
  // where 2 represents the field, dim the dimension of the feature.
  // 2  0 0 inf 
  // 2  0 0 inf 
  // 2  1 0 inf 
  // means that in Z/2Z-homology, the Betti numbers are b0=2 and b1=1.
  pcoh.output_diagram();
}
