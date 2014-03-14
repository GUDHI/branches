/*
 * Skeleton_blocker_simple_traits.h
 *
 *  Created on: Feb 11, 2014
 *      Author: dsalinas
 */

#ifndef GUDHI_SKELETON_BLOCKERS_SIMPLE_TRAITS_H_
#define GUDHI_SKELETON_BLOCKERS_SIMPLE_TRAITS_H_

#include <string>
#include <sstream>
#include "Skeleton_blocker_simplex.h"


struct Skeleton_blocker_simple_traits{
	/**
	 * @brief global and local handle similar to <a href="http://www.boost.org/doc/libs/1_38_0/libs/graph/doc/subgraph.html">boost subgraphs</a>.
	 * In gross, vertices are stored in a vector.
	 * For the root simplicial complex, the local and global descriptors are the same.
	 * For a subcomplex L and one of its vertices 'v', the local descriptor of 'v' is its position in
	 * the vertex vector of the subcomplex L whereas its global descriptor is the position of 'v'
	 * in the vertex vector of the root simplicial complex.
	 */
	struct Root_vertex_handle{
		typedef int boost_vertex_handle;
		explicit Root_vertex_handle(boost_vertex_handle val = -1 ):vertex(val){}
		boost_vertex_handle vertex;

		bool operator!=( const Root_vertex_handle& other) const{
			return ! this->vertex == other.vertex;
		}

		bool operator==( const Root_vertex_handle& other) const{
			return this->vertex == other.vertex;
		}

		bool operator<( const Root_vertex_handle& other) const{
			return this->vertex < other.vertex;
		}

		friend std::ostream& operator << (std::ostream& o, const Root_vertex_handle & v)
		{
			o << v.vertex;
			return o;
		}
	};

	struct Vertex_handle{
		typedef int boost_vertex_handle;
		Vertex_handle(boost_vertex_handle val=-1):vertex(val){}

		boost_vertex_handle vertex;

		bool operator==( const Vertex_handle& other) const{
			return this->vertex == other.vertex;
		}

		bool operator!=( const Vertex_handle& other) const{
			return this->vertex != other.vertex;
		}

		bool operator<(const Vertex_handle& other) const{
			return this->vertex < other.vertex;
		}

		friend std::ostream& operator << (std::ostream& o, const Vertex_handle & v)
		{
			o << v.vertex;
			return o;
		}
	};


	class Simple_vertex {
		bool is_active_;
		Root_vertex_handle id_;
	public:
		virtual ~Simple_vertex(){}

		void activate(){is_active_=true;}
		void deactivate(){is_active_=false;}
		bool is_active() const{return is_active_;}
		void set_id(Root_vertex_handle i){id_=i;}
		Root_vertex_handle get_id() const{return id_;}

		virtual std::string to_string() const {
			std::ostringstream res;
			res<< id_;
			return res.str();
		}

		friend std::ostream& operator << (std::ostream& o, const Simple_vertex & v){
			o << v.to_string();
			return o;
		}
	};
	typedef Simple_vertex Graph_vertex;

	class Simple_edge {
		Root_vertex_handle a_;
		Root_vertex_handle b_;
		int index_;
	public:

		Simple_edge():a_(-1),b_(-1),index_(-1){}

		int& index(){return index_;}
		int index() const {return index_;}

		void setId(Root_vertex_handle a,Root_vertex_handle b){
			a_ = a;
			b_ = b;
		}

		Root_vertex_handle first() const {
			return a_;
		}

		Root_vertex_handle second() const {
			return b_;
		}

		friend std::ostream& operator << (std::ostream& o, const Simple_edge & v){
			o << "("<<v.a_<<","<<v.b_<<" - id = "<<v.index();
			return o;
		}
	};

	typedef Simple_edge Graph_edge;

	typedef Skeleton_blocker_simplex<Vertex_handle> Simplex_handle;
	typedef Skeleton_blocker_simplex<Root_vertex_handle> Root_simplex_handle;
};


#endif /* GUDHI_SKELETON_BLOCKERS_SIMPLE_TRAITS_H_ */
