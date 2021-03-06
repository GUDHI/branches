/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Authors:       Francois Godi, small modifications by Pawel Dlotko
 *
 *    Copyright (C) 2015 Inria
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

#include <gudhi/Bottleneck.h>
#include <gudhi/reader_utils.h>
#include <iostream>
#include <vector>
#include <utility>  // for pair
#include <string>
#include <limits>  // for numeric_limits

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cout << "To run this program please provide as an input two files with persistence diagrams. Each file" <<
        " should contain a birth-death pair per line. Third, optional parameter is an error bound on the bottleneck" <<
        " distance (set by default to the smallest positive double value). If you set the error bound to 0, be" <<
        " aware this version is exact but expensive. The program will now terminate \n";
    return -1;
  }
  std::vector<std::pair<double, double>> diag1 = Gudhi::read_persistence_intervals_in_dimension(argv[1]);
  std::vector<std::pair<double, double>> diag2 = Gudhi::read_persistence_intervals_in_dimension(argv[2]);

  double tolerance = std::numeric_limits<double>::min();
  if (argc == 4) {
    tolerance = atof(argv[3]);
  }
  double b = Gudhi::persistence_diagram::bottleneck_distance(diag1, diag2, tolerance);
  std::cout << "The distance between the diagrams is : " << b << ". The tolerance is : " << tolerance << std::endl;

  return 0;
}
