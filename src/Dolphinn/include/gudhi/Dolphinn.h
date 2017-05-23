#ifndef DOLPHINN_H
#define DOLPHINN_H

#include <gudhi/Hypercube.h>
#include <thread>
//#include "IO.h"


namespace Gudhi {
namespace dolphinn {

/**
 * \class Dolphinn Dolphinn.h gudhi/Dolphinn.h
 * \brief Method for approximate neighbour computing.
 * 
 * \ingroup dolphinn
 * 
 * \details
 * This class was created to interface with the user. 
 *
 * Dolphinn projects all the data on the vertices of an hypercube aiming to send close points to 
 * close vertices (w.r.t. the hamming distance). It proposes two queries: the k-nearest neigbourg 
 * search and the range query.
 * 
 * \remark When the class is built, the hypercube is immediately built.
 * 
 */

	template <typename T, typename bitT>
  class Dolphinn
  {
  	typedef typename std::vector<T> Point;
  
  	// See constructor
  	int N;
  	const int D,K;
		double hashing_method;
		std::vector<Point>& pointset;
		Hypercube<Point, T, bitT> hypercube;

		public:
		Hypercube<Point, T, bitT> get_hypercube(){
			return hypercube;
		}

  	/** \brief Constructor of the class and fills the hypercube.
      *
      * @param pointset    		- Set of points
      * @param N           		- number of points
      * @param D           		- dimension of the points
      * @param K           		- dimension of Hypercube (and of the mapped points)
      * @param hashing_method - if positive, the parameter of Stable Distribution, if nul, the LSH method used is the hyperplanes.
      *                      Neighbor Search, to adapt to the average distance of the NN, 'r' is the hashing window.
   */
  	
  	Dolphinn(std::vector<Point>& pointset, int N, const int D, const int K, const double hashing_method) : N(pointset.size()), D(D), K(K), hashing_method(hashing_method), pointset(pointset), hypercube(pointset, N, D, K, 1, hashing_method) 
  		{}
  	
  	/** \brief Radius query the Hamming cube.
      *
      * @param query               - vector of queries
      * @param Q                   - number of queries
      * @param radius              - find a point within r with query
      * @param max_pnts_to_search  - threshold
      * @param results_idxs        - indices of Q points, where Eucl(point[i], query[i]) <= r
      * @param threads_no          - number of threads to be created. Default value is 'std::thread::hardware_concurrency()'.
    */
  	void radius_query(const std::vector<Point>& query, const int Q, const float radius, const int max_pnts_to_search, std::vector<int>& results_idxs, const int threads_no = 1/* std::thread::hardware_concurrency()*/) {
  		if(max_pnts_to_search>N){
  			hypercube.radius_query(query, Q, radius, N, results_idxs, threads_no);
  		} else {
  			hypercube.radius_query(query, Q, radius, max_pnts_to_search, results_idxs, threads_no);
  		}
  	}
  	
  	/** \brief Nearest Neighbor query in the Hamming cube.
      *
      * @param query               - vector of queries
      * @param Q                   - number of queries
      * @param m                   - number of neighbours to search 
      * @param max_pnts_to_search  - threshold
      * @param results_idxs_dists  - indices and distances of Q points, where the (Approximate) Nearest Neighbors are stored.
      * @param threads_no          - number of threads to be created. Default value is 'std::thread::hardware_concurrency()'.
    	*/
  	void m_nearest_neighbors_query(const std::vector<Point>& query, const int Q, const int m, const int max_pnts_to_search, std::vector<std::vector<std::pair<int, float>>>& results_idxs_dists, const int threads_no = 1 /*std::thread::hardware_concurrency()*/) {
  		if(max_pnts_to_search>N){
  			hypercube.m_nearest_neighbors_query(query, Q, m, N, results_idxs_dists, threads_no);
  		} else {
  			hypercube.m_nearest_neighbors_query(query, Q, m, max_pnts_to_search, results_idxs_dists, threads_no);
  		}
  		
  	}
  	
  	
  	
  };
}
}

#endif /* DOLPHINN_H*/