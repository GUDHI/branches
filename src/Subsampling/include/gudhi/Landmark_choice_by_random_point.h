/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Siargey Kachanovich
 *
 *    Copyright (C) 2016  INRIA Sophia Antipolis-Méditerranée (France)
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

#ifndef LANDMARK_CHOICE_BY_RANDOM_POINT_H_
#define LANDMARK_CHOICE_BY_RANDOM_POINT_H_

#include <boost/range/size.hpp>

#include <random>     // random_device, mt19937
#include <algorithm>  // shuffle
#include <numeric>    // iota
#include <iterator>
#include <gudhi/Clock.h>


namespace Gudhi {

  /**
   *  \ingroup witness_complex
   * \brief Landmark choice strategy by taking random vertices for landmarks.
   *
   *  \details It chooses nbL distinct landmarks from a random access range `points`
   *  and outputs them to an output iterator.
   *  Point_container::iterator should be ValueSwappable and RandomAccessIterator.
   */

  template <typename Point_container,
            typename OutputIterator>
  void landmark_choice_by_random_point(Point_container const &points,
                                       unsigned nbL,
                                       OutputIterator output_it) {
#ifdef GUDHI_LM_PROFILING
    Gudhi::Clock t;
#endif

    unsigned nbP = boost::size(points);
    assert(nbP >= nbL);
    std::vector<int> landmarks(nbP);
    std::iota(landmarks.begin(), landmarks.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());
 
    std::shuffle(landmarks.begin(), landmarks.end(), g);
    landmarks.resize(nbL);

    for (int l: landmarks)
      *output_it++ = points[l];
    
#ifdef GUDHI_LM_PROFILING
      t.end();
      std::cerr << "Random landmark choice took " << t.num_seconds()
        << " seconds." << std::endl;
#endif

    
  }

}  // namespace Gudhi

#endif  // LANDMARK_CHOICE_BY_RANDOM_POINT_H_
