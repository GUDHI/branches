/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Siargey Kachanovich
 *
 *    Copyright (C) 2015  INRIA Sophia Antipolis-Méditerranée (France)
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

#ifndef GUDHI_WITNESS_COMPLEX_H_
#define GUDHI_WITNESS_COMPLEX_H_

#include <boost/iterator/transform_iterator.hpp>
#include <algorithm>
#include <utility>
#include "gudhi/reader_utils.h"
#include "gudhi/distance_functions.h"
#include "gudhi/Simplex_tree.h"
#include <vector>
#include <forward_list>
#include <math.h>

namespace Gudhi {

  /*
template<typename FiltrationValue = double,
         typename SimplexKey      = int,
         typename VertexHandle    = int>
class Simplex_tree;
  */  

      
template<typename FiltrationValue = double,
         typename SimplexKey      = int,
         typename VertexHandle    = int>
    class Witness_complex: public Simplex_tree<> {
    //class Witness_complex: public Simplex_tree<FiltrationValue, SimplexKey, VertexHandle> {
    //class Witness_complex {
public:

//Simplex_tree<> st;
// typedef int                      Simplex_handle; //index in vector complex_
 
// typedef typename std::vector< Simplex_handle >::iterator    Boundary_simplex_iterator;
// typedef boost::iterator_range<Boundary_simplex_iterator>    Boundary_simplex_range;  
 
// typedef typename std::vector< Simplex_handle >::iterator    Skeleton_simplex_iterator;
// typedef boost::iterator_range< Skeleton_simplex_iterator >  Skeleton_simplex_range;

//  typedef IndexingTag Indexing_tag;
  /** \brief Type for the value of the filtration function.
   *
   * Must be comparable with <. */
//  typedef FiltrationValue Filtration_value;
  /** \brief Key associated to each simplex.
   *
   * Must be a signed integer type. */
//  typedef SimplexKey Simplex_key;
  /** \brief Type for the vertex handle.
   *
   * Must be a signed integer type. It admits a total order <. */
//  typedef VertexHandle Vertex_handle;

  /* Type of node in the simplex tree. */
//  typedef Simplex_tree_node_explicit_storage<Simplex_tree> Node;
  /* Type of dictionary Vertex_handle -> Node for traversing the simplex tree. */
//  typedef typename boost::container::flat_map<Vertex_handle, Node> Dictionary;

/*
  friend class Simplex_tree_node_explicit_storage< Simplex_tree<FiltrationValue, SimplexKey, VertexHandle> >;
  friend class Simplex_tree_siblings< Simplex_tree<FiltrationValue, SimplexKey, VertexHandle>, Dictionary>;
  friend class Simplex_tree_simplex_vertex_iterator< Simplex_tree<FiltrationValue, SimplexKey, VertexHandle> >;
  friend class Simplex_tree_boundary_simplex_iterator< Simplex_tree<FiltrationValue, SimplexKey, VertexHandle> >;
  friend class Simplex_tree_complex_simplex_iterator< Simplex_tree<FiltrationValue, SimplexKey, VertexHandle> >;
  friend class Simplex_tree_skeleton_simplex_iterator< Simplex_tree<FiltrationValue, SimplexKey, VertexHandle> >;
*/

  /* \brief Set of nodes sharing a same parent in the simplex tree. */
  /* \brief Set of nodes sharing a same parent in the simplex tree. */
// typedef Simplex_tree_siblings<Simplex_tree, Dictionary> Siblings;


typedef std::vector< double > Point_t;
typedef std::vector< Point_t > Point_Vector;

typedef std::vector< Vertex_handle > typeVectorVertex;
typedef std::pair<typeVectorVertex, Filtration_value> typeSimplex;
typedef std::pair< Simplex_tree<>::Simplex_handle, bool > typePairSimplexBool;
/*
Witness_complex(int number_of_landmarks, std::string file_name)
{
}
*/

/**
 * /brief Iterative construction of the witness complex basing on a matrix of k nearest neighbours of the form {witnesses}x{landmarks}.
 * Landmarks are supposed to be in [0,nbL-1]
 */

template< typename KNearestNeighbours >
void witness_complex(KNearestNeighbours & knn)
{
    int k=1; /* current dimension in iterative construction */
    //Construction of the active witness list
    int nbW = knn.size();
    int nbL = knn.at(0).size();
    VertexHandle vh;
    typeVectorVertex vv;
    typeSimplex simplex;
    typePairSimplexBool returnValue;
    /* The list of still useful witnesses
     * it will diminuish in the course of iterations
     */
    std::forward_list<int> active_w = new std::forward_list<int>();
    for (int i=0; i != nbW; ++i) {
        // initial fill of active witnesses list
        active_w.push_front(i);
    }
    for (int i=0; i != nbL; ++i) {
        // initial fill of 0-dimensional simplices
        // by doing it we don't assume that landmarks are necessarily witnesses themselves anymore
        vh = (Vertex_handle)i;
        vv = {i};
        /* TODO Filtration */
        simplex = std::make_pair(vv,Filtration_value(0.0));
        returnValue = this.insert_simplex(simplex.first, simplex.second);
        /* TODO Error if not inserted : normally no need here though*/
    }
    while (!active_w.empty() && k+1 < nbL ) {
        std::forward_list<int>::iterator it = active_w.begin();
        while (it != active_w.end()) {
            typeVectorVertex simplex_vector;
            for (int i=0; i != k+1; ++i) {
                // create a (k+1) element array for the given active landmark
                /*
                typeVectorVertex::iterator itSV = simplex_vector.begin();
                while (itSV != simplex_vector.end() && *itSV < knn[*it][i]) {
                    itSV++;
                }
                simplex_vector.insert(itSV,knn[*it][i]);
                */
                simplex_vector.push_back(knn[*it][i]);
            }
            /* THE INSERTION: Checking if all the subfaces are in the simplex tree is mandatory */
            bool ok = all_faces_in(simplex_vecter);
            returnValue = this.insert_simplex(simplex_vector,0.0);
        }
    } 
}

private:

bool all_faces_in(typeVectorVertex v)
{
return true;
}

/**
 * \brief Permutes the vector in such a way that the landmarks appear first
 */

void furthestPoints(Point_Vector &W, int nbP, std::string file_land, int dim, int nbL, Point_Vector &L) {
  //std::cout << "Enter furthestPoints "<< endl;
  //Point_Vector *L = new Point_Vector();
  double density = 5.;
  int current_number_of_landmarks=0;
  double curr_max_dist;
  double curr_dist;
  double mindist = 10005.;
  int curr_max_w=0;
  int curr_w=0;
  srand(354698);
  int rand_int = rand()% nbP;
  //std::cout << rand_int << endl;
  L.push_back(W[rand_int]);// first landmark is random
  current_number_of_landmarks++;
  while (1) {
    curr_w = 0;
    curr_max_dist = -1;
    for(Point_Vector::iterator itW = W.begin(); itW != W.end(); itW++) {
      //compute distance from w and L
      mindist = 100000.;
      for(Point_Vector::iterator itL = L.begin(); itL != L.end(); itL++) {
          //curr_dist = distPoints(*itW,*itL);
          curr_dist = euclidean_distance(*itW,*itL);
        if(curr_dist < mindist) {
          mindist = curr_dist;
        }
      }
      if(mindist > curr_max_dist) {
        curr_max_w = curr_w; //???
        curr_max_dist = mindist;
      }
      curr_w++;
    }
    L.push_back(W[curr_max_w]);
    current_number_of_landmarks++;
    density = sqrt(curr_max_dist);
    //std::cout << "[" << current_number_of_landmarks << ":" << density <<"] ";
    if(L.size() == nbL) break;
  }
  //std::cout << endl;
  return L;
}
  
}; //class Witness_complex

  
} // namespace Guhdi

#endif