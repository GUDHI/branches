/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Pawel Dlotko
 *
 *    Copyright (C) 2015  INRIA (France)
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

#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

//concretizations
#include <gudhi/persistence_vectors.h>
#include <gudhi/Persistence_landscape.h>
#include <gudhi/Persistence_landscape_on_grid.h>
#include <gudhi/Persistence_heat_maps.h>

#ifdef GUDHI_USE_TBB
#include <tbb/parallel_sort.h>
#include <tbb/task_scheduler_init.h>
#endif

#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

namespace Gudhi
{
namespace Gudhi_stat
{

template < typename TopologicalObject > 
class difference_of_objects
{
public:
	TopologicalObject operator()( const TopologicalObject& first, const TopologicalObject& second )const
	{
		return first-second;
	}
};

template < typename TopologicalObject > 
class norm_of_objects
{
public:
    norm_of_objects():power(1){}
    norm_of_objects( double power_ ):power(power_){}
	double operator()( const TopologicalObject& obj )const
	{
		TopologicalObject empty;
		double dist = empty.distance( obj , power );   	
		//std::cerr << "dist : " << dist << std::endl;getchar();			
		return dist;
	}
private:
	double power;
};



/**
* This is a generic function to perform multiplicative bootstrap.
**/
template < typename TopologicalObject , typename OperationOnObjects , typename NormOnObjects >
double multiplicative_bootstrap( const std::vector< TopologicalObject* >& topological_objects , size_t number_of_bootstrap_operations , const OperationOnObjects& oper , const NormOnObjects& norm ,  double quantile = 0.95 , size_t maximal_number_of_threads_in_TBB = std::numeric_limits<size_t>::max() )
{
	bool dbg = false;
	
	#ifdef GUDHI_USE_TBB
	tbb::task_scheduler_init init(maximal_number_of_threads_in_TBB == std::numeric_limits<size_t>::max() ? tbb::task_scheduler_init::automatic : maximal_number_of_threads_in_TBB);
	#endif
	
	//initialization of a random number generator:
    std::random_device rd;
    std::mt19937 generator( time(NULL) );
	std::normal_distribution<> norm_distribution(0.,1.);
	
	
	//first compute an average of topological_objects
	TopologicalObject average;
	average.compute_average( topological_objects );

	std::vector< double > vector_of_intermediate_characteristics( number_of_bootstrap_operations , 0 );


	#ifdef GUDHI_USE_TBB
    tbb::parallel_for ( tbb::blocked_range<size_t>(0, number_of_bootstrap_operations), [&](const tbb::blocked_range<size_t>& range) 
    {
    for  ( size_t it_no = range.begin() ;  it_no != range.end() ; ++it_no )
	#else
	for ( size_t it_no = 0 ;  it_no < number_of_bootstrap_operations ; ++it_no )
	#endif	
	{
		if ( dbg )
		{
			std::cout << "Still : " << number_of_bootstrap_operations-it_no << " tests to go. \n The subsampled vector consist of points number : ";
		}				
		
		
		//and compute the intermediate characteristic:		
		TopologicalObject result;
		for ( size_t i = 0 ; i != topological_objects.size() ; ++i )
		{				
			double rand_variable = norm_distribution( generator );
			result = result + rand_variable*oper(*(topological_objects[i]) , average);
		}
		if ( dbg )
		{
			std::cerr << "Result 1 : " << result << std::endl;
			getchar();
		}
		//HERE THE NORM SEEMS TO BE MISSING!!
		result = result.abs();
		if ( dbg )
		{
			std::cerr << "Result 2 : " << result << std::endl;
			getchar();
		}
		result = result*(1.0/sqrt( topological_objects.size() ));											
		if ( dbg )
		{
			std::cerr << "Result 3 : " << result << std::endl;
			getchar();
		}
		//NEED TO TAKE MAX
		if ( dbg )
		{
			std::cerr << "Result 4 : " << norm(result) << std::endl;
			getchar();
		}
		vector_of_intermediate_characteristics[it_no] = norm(result);		
	}
	#ifdef GUDHI_USE_TBB
    }
    );
	#endif
	
		
		
	size_t position_of_quantile = floor(quantile*vector_of_intermediate_characteristics.size());
	if ( position_of_quantile ) --position_of_quantile;
	if ( dbg )
	{
		std::cout << "position_of_quantile : " << position_of_quantile << ", and here is the array : " << std::endl;
		for ( size_t i = 0 ; i != vector_of_intermediate_characteristics.size() ; ++i )
		{
			std::cout << vector_of_intermediate_characteristics[i] << std::endl;
		}
		std::cout << std::endl;
	}
	  
	//now we need to sort the vector_of_distances and find the quantile:
	std::nth_element (vector_of_intermediate_characteristics.begin(), vector_of_intermediate_characteristics.begin()+position_of_quantile, vector_of_intermediate_characteristics.end());	
	double result = vector_of_intermediate_characteristics[ position_of_quantile ]/(sqrt( topological_objects.size() ));	
	if ( dbg )std::cout << "Result : " << result << std::endl;
	
	return result;
	
}//multiplicative_bootstrap

}//namespace Gudhi_stat
}//namespace Gudhi

#endif
