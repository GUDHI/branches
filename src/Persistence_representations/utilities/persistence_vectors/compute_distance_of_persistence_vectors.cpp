/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Pawel Dlotko
 *
 *    Copyright (C) 2016 Inria
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

#include <gudhi/Persistence_vectors.h>

#include <iostream>
#include <sstream>
#include <limits>
#include <vector>

using Euclidean_distance = Gudhi::Euclidean_distance;
using Vector_distances_in_diagram = Gudhi::Persistence_representations::Vector_distances_in_diagram<Euclidean_distance>;

int main(int argc, char** argv) {
  std::cout << "This program compute distance of persistence vectors stored in a file (the file needs to be created "
               "beforehand). \n";
  std::cout << "The first parameter of a program is an integer p. The program compute l^p distance of the vectors. For "
               "l^infty distance choose p = -1. \n";
  std::cout << "The remaining parameters of this programs are names of files with persistence vectors.\n";

  if (argc < 3) {
    std::cout << "Wrong number of parameters, the program will now terminate \n";
    return 1;
  }

  int pp = atoi(argv[1]);
  double p = std::numeric_limits<double>::max();
  if (pp != -1) {
    p = pp;
  }

  std::vector<const char*> filenames;
  for (int i = 2; i < argc; ++i) {
    filenames.push_back(argv[i]);
  }
  std::vector<Vector_distances_in_diagram> vectors;
  vectors.reserve(filenames.size());
  for (size_t file_no = 0; file_no != filenames.size(); ++file_no) {
    Vector_distances_in_diagram l;
    l.load_from_file(filenames[file_no]);
    vectors.push_back(l);
  }

  // and now we will compute the scalar product of landscapes.

  // first we prepare an array:
  std::vector<std::vector<double> > distance(filenames.size());
  for (size_t i = 0; i != filenames.size(); ++i) {
    std::vector<double> v(filenames.size(), 0);
    distance[i] = v;
  }

  // and now we can compute the distances:
  for (size_t i = 0; i != vectors.size(); ++i) {
    for (size_t j = i + 1; j != vectors.size(); ++j) {
      distance[i][j] = distance[j][i] = vectors[i].distance(vectors[j], p);
    }
  }

  // and now output the result to the screen and a file:
  std::ofstream out;
  out.open("distance.vect");
  for (size_t i = 0; i != distance.size(); ++i) {
    for (size_t j = 0; j != distance.size(); ++j) {
      std::cout << distance[i][j] << " ";
      out << distance[i][j] << " ";
    }
    std::cout << std::endl;
    out << std::endl;
  }
  out.close();

  std::cout << "Distance can be found in 'distance.vect' file\n";
  return 0;
}
