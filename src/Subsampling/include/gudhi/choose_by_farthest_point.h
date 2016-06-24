/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Siargey Kachanovich
 *
 *    Copyright (C) 2016 INRIA
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

#ifndef CHOOSE_BY_FARTHEST_POINT_H_
#define CHOOSE_BY_FARTHEST_POINT_H_

#include <boost/range.hpp>

#include <gudhi/Spatial_tree_data_structure.h>

#include <gudhi/Clock.h>

#include <CGAL/Search_traits.h>
#include <CGAL/Search_traits_adapter.h>
#include <CGAL/Fuzzy_sphere.h>

#include <iterator>
#include <algorithm>  // for sort
#include <vector>
#include <random>

namespace Gudhi {

namespace subsampling {
  /** 
   *  \ingroup subsampling
   *  \brief Subsample by a greedy strategy of iteratively adding the farthest point from the
   *  current chosen point set to the subsampling. 
   *  The iteration starts with the landmark `starting point`.
   *  \details It chooses `final_size` points from a random access range `points` and
   *  outputs it in the output iterator `output_it`.
   *  
   */

  template < typename Kernel,
             typename Point_container,
             typename OutputIterator>
  void choose_by_farthest_point( Kernel& k,
                                 Point_container const &points,
                                 int final_size,
                                 int starting_point,
                                 OutputIterator output_it)
  {
    typename Kernel::Squared_distance_d sqdist = k.squared_distance_d_object();
    
    int nb_points = boost::size(points);
    assert(nb_points >= final_size);

    int current_number_of_landmarks = 0;  // counter for landmarks
    double curr_max_dist = 0;  // used for defining the furhest point from L
    const double infty = std::numeric_limits<double>::infinity();  // infinity (see next entry)
    std::vector< double > dist_to_L(nb_points, infty);  // vector of current distances to L from points

    int curr_max_w = starting_point;

    for (current_number_of_landmarks = 0; current_number_of_landmarks != final_size; current_number_of_landmarks++) {
      // curr_max_w at this point is the next landmark
      *output_it++ = points[curr_max_w];
      // std::cout << curr_max_w << "\n";
      unsigned i = 0;
      for (auto& p : points) {
        double curr_dist = sqdist(p, *(std::begin(points) + curr_max_w));
        if (curr_dist < dist_to_L[i])
          dist_to_L[i] = curr_dist;
        ++i;
      }
      // choose the next curr_max_w
      curr_max_dist = 0;
      for (i = 0; i < dist_to_L.size(); i++)
        if (dist_to_L[i] > curr_max_dist) {
          curr_max_dist = dist_to_L[i];
          curr_max_w = i;
        }
    }
  }
  
  /** 
   *  \ingroup subsampling
   *  \brief Subsample by a greedy strategy of iteratively adding the farthest point from the
   *  current chosen point set to the subsampling. 
   *  The iteration starts with a random landmark.
   *  \details It chooses `final_size` points from a random access range `points` and
   *  outputs it in the output iterator `output_it`.
   *  
   */
  template < typename Kernel,
             typename Point_container,
             typename OutputIterator>
  void choose_by_farthest_point( Kernel& k,
                                 Point_container const &points,
                                 int final_size,
                                 OutputIterator output_it)
  {
    // Choose randomly the first landmark 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 6);
    int starting_point = dis(gen);
    choose_by_farthest_point(k, points, final_size, starting_point, output_it);
  }
  
} // namespace subsampling
  
}  // namespace Gudhi

#endif  // CHOOSE_BY_FARTHEST_POINT_H_
