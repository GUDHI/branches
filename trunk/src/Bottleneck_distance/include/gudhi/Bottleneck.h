/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author:       Francois Godi
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

#ifndef BOTTLENECK_H_
#define BOTTLENECK_H_

#include <gudhi/Graph_matching.h>
#include <cmath>

namespace Gudhi {

namespace persistence_diagram {

double bottleneck_distance_approx(Persistence_graph& g, double e) {
  double b_lower_bound = 0.;
  double b_upper_bound = g.diameter_bound();
  const double alpha = std::pow(g.size(), 1. / 5.);
  Graph_matching m(g);
  Graph_matching biggest_unperfect(g);
  while (b_upper_bound - b_lower_bound > 2 * e) {
    double step = b_lower_bound + (b_upper_bound - b_lower_bound) / alpha;
    if (step <= b_lower_bound || step >= b_upper_bound)  // Avoid precision problem
      break;
    m.set_r(step);
    while (m.multi_augment());  // compute a maximum matching (in the graph corresponding to the current r)
    if (m.perfect()) {
      m = biggest_unperfect;
      b_upper_bound = step;
    } else {
      biggest_unperfect = m;
      b_lower_bound = step;
    }
  }
  return (b_lower_bound + b_upper_bound) / 2.;
}

double bottleneck_distance_exact(Persistence_graph& g) {
  std::vector<double> sd = g.sorted_distances();
  long lower_bound_i = 0;
  long upper_bound_i = sd.size() - 1;
  const double alpha = std::pow(g.size(), 1. / 5.);
  Graph_matching m(g);
  Graph_matching biggest_unperfect(g);
  while (lower_bound_i != upper_bound_i) {
    long step = lower_bound_i + static_cast<long> ((upper_bound_i - lower_bound_i - 1) / alpha);
    m.set_r(sd.at(step));
    while (m.multi_augment());  // compute a maximum matching (in the graph corresponding to the current r)
    if (m.perfect()) {
      m = biggest_unperfect;
      upper_bound_i = step;
    } else {
      biggest_unperfect = m;
      lower_bound_i = step + 1;
    }
  }
  return sd.at(lower_bound_i);
}

/** \brief Function to use in order to compute the Bottleneck distance between two persistence diagrams (see concepts).
 * If the last parameter e is not 0, you get an additive e-approximation, which is a lot faster to compute whatever is
 * e.
 * Thus, by default, e is a very small positive double, actually the smallest double possible such that the
 * floating-point inaccuracies don't lead to a failure of the algorithm.
 *
 * \ingroup bottleneck_distance
 */
template<typename Persistence_diagram1, typename Persistence_diagram2>
double bottleneck_distance(const Persistence_diagram1 &diag1, const Persistence_diagram2 &diag2,
                           double e = std::numeric_limits<double>::min()) {
  Persistence_graph g(diag1, diag2, e);
  if (g.bottleneck_alive() == std::numeric_limits<double>::infinity())
    return std::numeric_limits<double>::infinity();
  return std::max(g.bottleneck_alive(), e == 0. ? bottleneck_distance_exact(g) : bottleneck_distance_approx(g, e));
}

}  // namespace persistence_diagram

}  // namespace Gudhi

#endif  // BOTTLENECK_H_
