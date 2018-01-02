/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Pawel Dlotko
 *
 *    Copyright (C) 2016  INRIA (France)
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

#include <gudhi/reader_utils.h>
#include <gudhi/Persistence_intervals.h>

#include <iostream>
#include <vector>
#include <limits>
#include <utility>

using Persistence_intervals = Gudhi::Persistence_representations::Persistence_intervals;

int main(int argc, char** argv) {
  std::cout << "This program compute a plot of persistence Betti numbers. The input parameter is a file with "
               "persistence intervals. \n";
  std::cout << "The second optional parameter of a program is the dimension of the persistence that is to be used. If "
               "your file contains only birth-death pairs, you can skip this parameter\n";
  if (argc < 2) {
    std::cout << "To run this program, please provide the name of a file with persistence diagram and number of "
                 "dominant intervals you would like to get \n";
    return 1;
  }
  unsigned dimension = std::numeric_limits<unsigned>::max();
  int dim = -1;
  if (argc > 2) {
    dim = atoi(argv[2]);
  }
  if (dim >= 0) {
    dimension = (unsigned)dim;
  }

  std::stringstream gnuplot_script;
  gnuplot_script << argv[1] << "_Gnuplot_script";
  std::ofstream out;
  out.open(gnuplot_script.str().c_str());

  Persistence_intervals p(argv[1], dimension);
  std::vector<std::pair<double, size_t> > pbns = p.compute_persistent_betti_numbers();

  // set up the ranges so that we see the image well.
  double xRangeBegin = pbns[0].first;
  double xRangeEnd = pbns[pbns.size() - 1].first;
  double yRangeBegin = 0;
  double yRangeEnd = 0;
  for (size_t i = 0; i != pbns.size(); ++i) {
    if (pbns[i].second > yRangeEnd) yRangeEnd = pbns[i].second;
  }
  xRangeBegin -= (xRangeEnd - xRangeBegin) / 100.0;
  xRangeEnd += (xRangeEnd - xRangeBegin) / 100.0;
  yRangeEnd += yRangeEnd / 100;

  out << "set xrange [" << xRangeBegin << " : " << xRangeEnd << "]" << std::endl;
  out << "set yrange [" << yRangeBegin << " : " << yRangeEnd << "]" << std::endl;
  out << "plot '-' using 1:2 notitle with lp " << std::endl;
  double previous_y = 0;
  for (size_t i = 0; i != pbns.size(); ++i) {
    out << pbns[i].first << " " << previous_y << std::endl;
    out << pbns[i].first << " " << pbns[i].second << std::endl;
    previous_y = pbns[i].second;
  }
  out << std::endl;
  out.close();

  std::cout << "To visualize, open gnuplot and type: load \'" << gnuplot_script.str().c_str() << "\'" << std::endl;

  return 0;
}
