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

#include <gudhi/Persistence_landscape_on_grid.h>

#include <iostream>
#include <sstream>
#include <limits>
#include <vector>

using Persistence_landscape_on_grid = Gudhi::Persistence_representations::Persistence_landscape_on_grid;

int main(int argc, char** argv) {
  std::cout << "This program creates persistence landscape on grid of diagrams provided as an input.\n";
  std::cout << "The first parameter of a program is an integer, a size of a grid.\n";
  std::cout << "The second and third parameters are min and max of the grid. If you want those numbers to be computed "
               "based on the data, set them both to -1 \n";
  std::cout << "The fourth parameter of the program is the dimension of persistence to be used to construct "
               "persistence landscape on a grid. If your file contains ";
  std::cout << "the information about dimension of birth-death pairs, please provide here the dimension of intervals "
               "you want to use. If your input files consist only ";
  std::cout << "of birth-death pairs, please set the fourth parameter to -1 \n";
  std::cout << "The remaining parameters are the names of files with persistence diagrams. \n";

  if (argc < 5) {
    std::cout << "Wrong parameter list, the program will now terminate \n";
    return 1;
  }

  size_t size_of_grid = (size_t)atoi(argv[1]);
  double min_ = atof(argv[2]);
  double max_ = atof(argv[3]);
  int dim = atoi(argv[4]);
  unsigned dimension = std::numeric_limits<unsigned>::max();
  if (dim >= 0) {
    dimension = (unsigned)dim;
  }

  std::vector<const char*> filenames;
  for (int i = 5; i < argc; ++i) {
    filenames.push_back(argv[i]);
  }

  std::cout << "Creating persistence landscapes...\n";
  for (size_t i = 0; i != filenames.size(); ++i) {
    std::cout << "Creating persistence landscape on a grid based on a file : " << filenames[i] << std::endl;
    Persistence_landscape_on_grid l;
    if ((min_ != -1) || (max_ != -1)) {
      l = Persistence_landscape_on_grid(filenames[i], min_, max_, size_of_grid, dimension);
    } else {
      // (min_ == -1) && (max_ == -1), in this case the program will find min_ and max_ based on the data.
      l = Persistence_landscape_on_grid(filenames[i], size_of_grid, dimension);
    }
    std::stringstream ss;
    ss << filenames[i] << ".g_land";
    l.print_to_file(ss.str().c_str());
  }
  std::cout << "Done \n";
  return 0;
}
