/*
 * Skeleton_blockers_iterators_out.h
 *
 *  Created on: Aug 25, 2014
 *      Author: dsalinas
 */

#ifndef GUDHI_SKELETON_BLOCKERS_VERTICES_ITERATORS_H_
#define GUDHI_SKELETON_BLOCKERS_VERTICES_ITERATORS_H_

#include "boost/iterator/iterator_facade.hpp"


/**
 *@brief Iterator on the vertices of a simplicial complex
 *@remark The increment ++ operators go to the next active vertex.
 */
template<typename SkeletonBlockerComplex>
class Complex_vertex_iterator : public boost::iterator_facade
< Complex_vertex_iterator <SkeletonBlockerComplex>
  , typename SkeletonBlockerComplex::Vertex_handle const
  , boost::forward_traversal_tag
  , typename SkeletonBlockerComplex::Vertex_handle const
  >
{
	friend class boost::iterator_core_access;

	typedef typename SkeletonBlockerComplex::boost_vertex_iterator boost_vertex_iterator;
	typedef typename SkeletonBlockerComplex::Vertex_handle Vertex_handle;
private:
	const SkeletonBlockerComplex* complex;
	std::pair<boost_vertex_iterator,boost_vertex_iterator> vertexIterator;


public:
	Complex_vertex_iterator():complex(NULL){
	}

	Complex_vertex_iterator(const SkeletonBlockerComplex* complex_):
		complex(complex_),
		vertexIterator(vertices(complex_->skeleton)){
		if(!finished() && !is_active()) {
			goto_next_valid();
		}
	}

	/**
	 * return an iterator to the end.
	 */
	Complex_vertex_iterator(const SkeletonBlockerComplex* complex_,int end):
		complex(complex_),vertexIterator(vertices(complex_->skeleton)){
		vertexIterator.first = vertexIterator.second ;
	}

public:
	void increment () {goto_next_valid();}
	Vertex_handle  dereference() const	{
		return(Vertex_handle(*(vertexIterator.first)));
	}

	bool equal(const Complex_vertex_iterator& other) const{
		return vertexIterator == other.vertexIterator &&  complex == other.complex;
	}

private:
	bool finished() const{
		return vertexIterator.first == vertexIterator.second;
	}

	void goto_next_valid(){
		++vertexIterator.first;
		if(!finished() && !is_active()){
			goto_next_valid();
		}
	}

	bool is_active() const{
		return ((*complex)[Vertex_handle(*vertexIterator.first)]).is_active();
	}

};




template<typename SkeletonBlockerComplex>
class Complex_neighbors_vertices_iterator
: public boost::iterator_facade < Complex_neighbors_vertices_iterator<SkeletonBlockerComplex>
  , typename SkeletonBlockerComplex::Vertex_handle const
  , boost::forward_traversal_tag
  , typename SkeletonBlockerComplex::Vertex_handle const
  >
{
	friend class boost::iterator_core_access;

	typedef SkeletonBlockerComplex Complex;
	typedef typename Complex::boost_adjacency_iterator boost_adjacency_iterator;
	typedef typename Complex::Vertex_handle Vertex_handle;
	typedef typename Complex::Edge_handle Edge_handle;

private:

	const Complex* complex;
	Vertex_handle v;

	boost_adjacency_iterator current_;
	boost_adjacency_iterator end_;

public:
	//	boost_adjacency_iterator ai, ai_end;
	//	for (tie(ai, ai_end) = adjacent_vertices(v.vertex, skeleton); ai != ai_end; ++ai){

	Complex_neighbors_vertices_iterator():complex(NULL){
	}

	Complex_neighbors_vertices_iterator(const Complex* complex_,Vertex_handle v_):
		complex(complex_),
		v(v_){
		tie(current_,end_) = adjacent_vertices(v.vertex, complex->skeleton);
	}

	/**
	 * returns an iterator to the end
	 */
	Complex_neighbors_vertices_iterator(const Complex* complex_,Vertex_handle v_,int end):
		complex(complex_),
		v(v_){
		tie(current_,end_) = adjacent_vertices(v.vertex, complex->skeleton);
		set_end();
	}


	void increment () {
		if(current_ != end_)
			++current_;
	}

	Vertex_handle  dereference() const	{
		return(Vertex_handle(*current_));
	}

	bool equal(const Complex_neighbors_vertices_iterator& other) const{
		return (complex== other.complex) && (v == other.v) && (current_ == other.current_) && (end_ == other.end_);
	}

private:
	//todo remove this ugly hack
	void set_end(){
		current_ = end_;
	}
};




#endif /* GUDHI_SKELETON_BLOCKERS_VERTICES_ITERATORS_H_ */


