#include <gudhi/choose_n_farthest_points.h>
#include <gudhi/pick_n_random_points.h>
#include "tbb/tbb.h"

/** Given a set of points p_1, ... , p_n ordered by their insertion order in the 
  * filtration, 
  * computes the edge-filtration corresponding to the oscillating Rips zigzag 
  * filtration of the set of points, i.e.,
  * ... <- R({p_0, ... , p_{i}}, nu * eps_i) ->
  *                   R({p_1, ... , p_i, p_{i+1}}, mu * eps_i) <- 
  *                              R({p_1, ... , p_i, p_{i+1}}, nu * eps_{i+1}) -> ...
  * where 0 < nu <= mu, and eps_i is defined as the sparsity of the point cloud 
  * {p_1, ... , p_i}, i.e., the shortest distance between two points in the set. 
  * This is a decreasing sequence of numbers.
  *
  * The function computes the eps_i in filtration_value[i] = eps_i, with eps_0 = 
  * infinity. A simplex appearing in the inclusion  
  * R({p_0, ... , p_{i}}, nu * eps_i) -> R({p_1, ... , p_i, p_{i+1}}, mu * eps_i)
  * is given filtration value eps_i.
  *
  * filtration_values must be empty, receives the filtration values.
  * edge_filtration must be empty, receives the edge filtration.
  */
template<typename Point_container,
         typename Distance, //furnish()
         typename Edge_t >
void points_to_edge_filtration(Point_container const &points,
                               Distance distance,
                               double nu,
                               double mu,
                               std::vector<double> &filtration_values,
                               std::vector<Edge_t> &edge_filtration )
{
  //computes the eps_i naively, in parallel
  size_t n = points.size();
  filtration_values.resize(n);
  filtration_values[0] = std::numeric_limits<double>::infinity();//eps_0
  tbb::parallel_for(size_t i = 1; i != n; ++i) {
    double dist = std::numeric_limits<double>::infinity();
    tbb::parallel_for(size_t j = 0; j != i; ++j) {
      //find distance from p_j to p_i
      auto curr_dist = distance(points[i],points[j]); 
      if(dist > curr_dist) { dist = curr_dist; } //maintain shortest distance
    }
  }
  //turn filtration_value[i] into sparsity of {p_0, ... , p_i}
  for(size_t i = 1; i != n; ++i) {
    if(filtration_values[i] > filtration_values[i-1]) //make decreasing
    {  filtration_values[i] = filtration_values[i-1];  }
  }
  

}