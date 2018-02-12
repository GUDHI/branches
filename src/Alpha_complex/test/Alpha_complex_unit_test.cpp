/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Vincent Rouvreau
 *
 *    Copyright (C) 2015  INRIA
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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "alpha_complex"
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <CGAL/Delaunay_triangulation.h>
#include <CGAL/Epick_d.h>

#include <cmath>  // float comparison
#include <limits>
#include <string>
#include <vector>

#include <gudhi/Alpha_complex.h>
// to construct a simplex_tree from Delaunay_triangulation
#include <gudhi/graph_simplicial_complex.h>
#include <gudhi/Simplex_tree.h>
#include <gudhi/Unitary_tests_utils.h>

// Use dynamic_dimension_tag for the user to be able to set dimension
typedef CGAL::Epick_d< CGAL::Dynamic_dimension_tag > Kernel_d;
// Use static dimension_tag for the user not to be able to set dimension
typedef CGAL::Epick_d< CGAL::Dimension_tag<2> > Kernel_s;
// The triangulation uses the default instantiation of the TriangulationDataStructure template parameter

typedef boost::mpl::list<Kernel_d, Kernel_s> list_of_kernel_variants;

BOOST_AUTO_TEST_CASE_TEMPLATE(Alpha_complex_from_OFF_file, TestedKernel, list_of_kernel_variants) {
  // ----------------------------------------------------------------------------
  //
  // Init of an alpha-complex from a OFF file
  //
  // ----------------------------------------------------------------------------
  std::string off_file_name("alphacomplexdoc.off");
  double max_alpha_square_value = 60.0;
  std::cout << "========== OFF FILE NAME = " << off_file_name << " - alpha²=" <<
      max_alpha_square_value << "==========" << std::endl;

  Gudhi::alpha_complex::Alpha_complex<TestedKernel> alpha_complex_from_file(off_file_name);

  std::cout << "alpha_complex_from_points.number_of_vertices()=" << alpha_complex_from_file.number_of_vertices()
      << std::endl;
  BOOST_CHECK(alpha_complex_from_file.number_of_vertices() == 7);

  Gudhi::Simplex_tree<> simplex_tree_60;
  BOOST_CHECK(alpha_complex_from_file.create_complex(simplex_tree_60, max_alpha_square_value));

  std::cout << "simplex_tree_60.dimension()=" << simplex_tree_60.dimension() << std::endl;
  BOOST_CHECK(simplex_tree_60.dimension() == 2);

  std::cout << "alpha_complex_from_points.number_of_vertices()=" << alpha_complex_from_file.number_of_vertices()
      << std::endl;
  BOOST_CHECK(alpha_complex_from_file.number_of_vertices() == 7);

  std::cout << "simplex_tree_60.num_vertices()=" << simplex_tree_60.num_vertices() << std::endl;
  BOOST_CHECK(simplex_tree_60.num_vertices() == 7);

  std::cout << "simplex_tree_60.num_simplices()=" << simplex_tree_60.num_simplices() << std::endl;
  BOOST_CHECK(simplex_tree_60.num_simplices() == 25);

  max_alpha_square_value = 59.0;
  std::cout << "========== OFF FILE NAME = " << off_file_name << " - alpha²=" <<
      max_alpha_square_value << "==========" << std::endl;

  Gudhi::Simplex_tree<> simplex_tree_59;
  BOOST_CHECK(alpha_complex_from_file.create_complex(simplex_tree_59, max_alpha_square_value));
  
  std::cout << "simplex_tree_59.dimension()=" << simplex_tree_59.dimension() << std::endl;
  BOOST_CHECK(simplex_tree_59.dimension() == 2);

  std::cout << "simplex_tree_59.num_vertices()=" << simplex_tree_59.num_vertices() << std::endl;
  BOOST_CHECK(simplex_tree_59.num_vertices() == 7);

  std::cout << "simplex_tree_59.num_simplices()=" << simplex_tree_59.num_simplices() << std::endl;
  BOOST_CHECK(simplex_tree_59.num_simplices() == 23);
}

// Use static dimension_tag for the user not to be able to set dimension
typedef CGAL::Epick_d< CGAL::Dimension_tag<4> > Kernel_4;
typedef Kernel_4::Point_d Point_4;
typedef std::vector<Point_4> Vector_4_Points;

bool is_point_in_list(Vector_4_Points points_list, Point_4 point) {
  for (auto& point_in_list : points_list) {
    if (point_in_list == point) {
      return true;  // point found
    }
  }
  return false;  // point not found
}

BOOST_AUTO_TEST_CASE(Alpha_complex_from_points) {
  // ----------------------------------------------------------------------------
  // Init of a list of points
  // ----------------------------------------------------------------------------
  Vector_4_Points points;
  std::vector<double> coords = { 0.0, 0.0, 0.0, 1.0 };
  points.push_back(Point_4(coords.begin(), coords.end()));
  coords = { 0.0, 0.0, 1.0, 0.0 };
  points.push_back(Point_4(coords.begin(), coords.end()));
  coords = { 0.0, 1.0, 0.0, 0.0 };
  points.push_back(Point_4(coords.begin(), coords.end()));
  coords = { 1.0, 0.0, 0.0, 0.0 };
  points.push_back(Point_4(coords.begin(), coords.end()));

  // ----------------------------------------------------------------------------
  // Init of an alpha complex from the list of points
  // ----------------------------------------------------------------------------
  Gudhi::alpha_complex::Alpha_complex<Kernel_4> alpha_complex_from_points(points);

  std::cout << "========== Alpha_complex_from_points ==========" << std::endl;

  Gudhi::Simplex_tree<> simplex_tree;
  BOOST_CHECK(alpha_complex_from_points.create_complex(simplex_tree));
  
  std::cout << "alpha_complex_from_points.number_of_vertices()=" << alpha_complex_from_points.number_of_vertices()
      << std::endl;
  BOOST_CHECK(alpha_complex_from_points.number_of_vertices() == points.size());

  // Another way to check num_simplices
  std::cout << "Iterator on alpha complex simplices in the filtration order, with [filtration value]:" << std::endl;
  int num_simplices = 0;
  for (auto f_simplex : simplex_tree.filtration_simplex_range()) {
    num_simplices++;
    std::cout << "   ( ";
    for (auto vertex : simplex_tree.simplex_vertex_range(f_simplex)) {
      std::cout << vertex << " ";
    }
    std::cout << ") -> " << "[" << simplex_tree.filtration(f_simplex) << "] ";
    std::cout << std::endl;
  }
  BOOST_CHECK(num_simplices == 15);
  std::cout << "simplex_tree.num_simplices()=" << simplex_tree.num_simplices() << std::endl;
  BOOST_CHECK(simplex_tree.num_simplices() == 15);

  std::cout << "simplex_tree.dimension()=" << simplex_tree.dimension() << std::endl;
  BOOST_CHECK(simplex_tree.dimension() == 3);
  std::cout << "simplex_tree.num_vertices()=" << simplex_tree.num_vertices() << std::endl;
  BOOST_CHECK(simplex_tree.num_vertices() == 4);

  for (auto f_simplex : simplex_tree.filtration_simplex_range()) {
    switch (simplex_tree.dimension(f_simplex)) {
      case 0:
        GUDHI_TEST_FLOAT_EQUALITY_CHECK(simplex_tree.filtration(f_simplex), 0.0);
        break;
      case 1:
        GUDHI_TEST_FLOAT_EQUALITY_CHECK(simplex_tree.filtration(f_simplex), 1.0/2.0);
        break;
      case 2:
        GUDHI_TEST_FLOAT_EQUALITY_CHECK(simplex_tree.filtration(f_simplex), 2.0/3.0);
        break;
      case 3:
        GUDHI_TEST_FLOAT_EQUALITY_CHECK(simplex_tree.filtration(f_simplex), 3.0/4.0);
        break;
      default:
        BOOST_CHECK(false);  // Shall not happen
        break;
    }
  }

  Point_4 p0 = alpha_complex_from_points.get_point(0);
  std::cout << "alpha_complex_from_points.get_point(0)=" << p0 << std::endl;
  BOOST_CHECK(4 == p0.dimension());
  BOOST_CHECK(is_point_in_list(points, p0));

  Point_4 p1 = alpha_complex_from_points.get_point(1);
  std::cout << "alpha_complex_from_points.get_point(1)=" << p1 << std::endl;
  BOOST_CHECK(4 == p1.dimension());
  BOOST_CHECK(is_point_in_list(points, p1));

  Point_4 p2 = alpha_complex_from_points.get_point(2);
  std::cout << "alpha_complex_from_points.get_point(2)=" << p2 << std::endl;
  BOOST_CHECK(4 == p2.dimension());
  BOOST_CHECK(is_point_in_list(points, p2));

  Point_4 p3 = alpha_complex_from_points.get_point(3);
  std::cout << "alpha_complex_from_points.get_point(3)=" << p3 << std::endl;
  BOOST_CHECK(4 == p3.dimension());
  BOOST_CHECK(is_point_in_list(points, p3));

  // Test to the limit
  BOOST_CHECK_THROW (alpha_complex_from_points.get_point(4), std::out_of_range);
  BOOST_CHECK_THROW (alpha_complex_from_points.get_point(-1), std::out_of_range);
  BOOST_CHECK_THROW (alpha_complex_from_points.get_point(1234), std::out_of_range);
  
  // Test after prune_above_filtration
  bool modified = simplex_tree.prune_above_filtration(0.6);
  if (modified) {
    simplex_tree.initialize_filtration();
  }
  BOOST_CHECK(modified);
  
  // Another way to check num_simplices
  std::cout << "Iterator on alpha complex simplices in the filtration order, with [filtration value]:" << std::endl;
  num_simplices = 0;
  for (auto f_simplex : simplex_tree.filtration_simplex_range()) {
    num_simplices++;
    std::cout << "   ( ";
    for (auto vertex : simplex_tree.simplex_vertex_range(f_simplex)) {
      std::cout << vertex << " ";
    }
    std::cout << ") -> " << "[" << simplex_tree.filtration(f_simplex) << "] ";
    std::cout << std::endl;
  }
  BOOST_CHECK(num_simplices == 10);
  std::cout << "simplex_tree.num_simplices()=" << simplex_tree.num_simplices() << std::endl;
  BOOST_CHECK(simplex_tree.num_simplices() == 10);

  std::cout << "simplex_tree.dimension()=" << simplex_tree.dimension() << std::endl;
  BOOST_CHECK(simplex_tree.dimension() == 1);
  std::cout << "simplex_tree.num_vertices()=" << simplex_tree.num_vertices() << std::endl;
  BOOST_CHECK(simplex_tree.num_vertices() == 4);

  for (auto f_simplex : simplex_tree.filtration_simplex_range()) {
    switch (simplex_tree.dimension(f_simplex)) {
      case 0:
        GUDHI_TEST_FLOAT_EQUALITY_CHECK(simplex_tree.filtration(f_simplex), 0.0);
        break;
      case 1:
        GUDHI_TEST_FLOAT_EQUALITY_CHECK(simplex_tree.filtration(f_simplex), 1.0/2.0);
        break;
      default:
        BOOST_CHECK(false);  // Shall not happen
        break;
    }
  }

}

BOOST_AUTO_TEST_CASE_TEMPLATE(Alpha_complex_from_empty_points, TestedKernel, list_of_kernel_variants) {
  std::cout << "========== Alpha_complex_from_empty_points ==========" << std::endl;

  // ----------------------------------------------------------------------------
  // Init of an empty list of points
  // ----------------------------------------------------------------------------
  std::vector<typename TestedKernel::Point_d> points;

  // ----------------------------------------------------------------------------
  // Init of an alpha complex from the list of points
  // ----------------------------------------------------------------------------
  Gudhi::alpha_complex::Alpha_complex<TestedKernel> alpha_complex_from_points(points);

  // Test to the limit
  BOOST_CHECK_THROW (alpha_complex_from_points.get_point(0), std::out_of_range);

  Gudhi::Simplex_tree<> simplex_tree;
  BOOST_CHECK(!alpha_complex_from_points.create_complex(simplex_tree));
  
  std::cout << "alpha_complex_from_points.number_of_vertices()=" << alpha_complex_from_points.number_of_vertices()
      << std::endl;
  BOOST_CHECK(alpha_complex_from_points.number_of_vertices() == points.size());

  std::cout << "simplex_tree.num_simplices()=" << simplex_tree.num_simplices() << std::endl;
  BOOST_CHECK(simplex_tree.num_simplices() == 0);

  std::cout << "simplex_tree.dimension()=" << simplex_tree.dimension() << std::endl;
  BOOST_CHECK(simplex_tree.dimension() == -1);
  
  std::cout << "simplex_tree.num_vertices()=" << simplex_tree.num_vertices() << std::endl;
  BOOST_CHECK(simplex_tree.num_vertices() == 0);
}
