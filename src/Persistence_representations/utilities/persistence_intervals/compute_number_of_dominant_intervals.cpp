/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Pawel Dlotko
 *
 *    Copyright (C) 2016 Inria
 *
 *    Modification(s):
 *      - YYYY/MM Author: Description of the modification
 */

#include <gudhi/Persistence_intervals.h>

#include <iostream>
#include <limits>
#include <vector>
#include <utility>

using Persistence_intervals = Gudhi::Persistence_representations::Persistence_intervals;

int main(int argc, char** argv) {
  std::cout << "This program compute the dominant intervals. A number of intervals to be displayed is a parameter of "
               "this program. \n";
  if (argc != 4) {
    std::cout << "To run this program, please provide the name of a file with persistence diagram, dimension of "
                 "intervals that should be taken into account (if your file contains only persistence pairs in a "
                 "single dimension, set it up to -1) and number of dominant intervals you would like to get \n";
    return 1;
  }
  int dim = atoi(argv[2]);
  unsigned dimension = std::numeric_limits<unsigned>::max();
  if (dim >= 0) {
    dimension = (unsigned)dim;
  }
  Persistence_intervals p(argv[1], dimension);
  std::vector<std::pair<double, double> > dominant_intervals = p.dominant_intervals(atoi(argv[3]));
  std::cout << "Here are the dominant intervals : " << std::endl;
  for (size_t i = 0; i != dominant_intervals.size(); ++i) {
    std::cout << " " << dominant_intervals[i].first << "," << dominant_intervals[i].second << " " << std::endl;
  }

  return 0;
}
