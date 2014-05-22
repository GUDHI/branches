#ifndef GUDHI_SKELETON_BLOCKER_SIMPLEX_H
#define GUDHI_SKELETON_BLOCKER_SIMPLEX_H

#include<cassert>
#include<iostream>
#include<set>
#include<vector>


/**
 *@brief Abstract simplex used in skeleton blockers data-structure.
 *
 * An abstract simplex is represented as an ordered set of T elements,
 * each element representing a vertex.
 * 
 * The element representing a vertex can be SkeletonBlockerDS::Vertex_handle or SkeletonBlockerDS::Root_vertex_handle.
 *
 *
 */
template <typename T>

class Skeleton_blocker_simplex {

private :
	std::set<T> simplex_set;

public:
	typedef typename T::boost_vertex_handle boost_vertex_handle;

	typedef T Vertex_handle;


	typedef typename std::set<T>::const_iterator Simplex_vertex_const_iterator;
	typedef typename std::set<T>::iterator Simplex_vertex_iterator;

	/** @name Constructors / Destructors / Initialization
	 */
	//@{

	/**
	 * Constructs the empty simplex {}
	 */
	Skeleton_blocker_simplex():simplex_set() {}

	/**
	 * Clear the simplex
	 */
	inline void clear() {
		simplex_set.clear();
	}

	/**
	 * Constructs the singleton {a}
	 */
	Skeleton_blocker_simplex(T a)
	{
		add_vertex(a);
	}

	/**
	 * Constructs the edge {a,b}
	 */
	Skeleton_blocker_simplex(T a, T b)
	{
		add_vertex(a); add_vertex(b);
	}

	/**
	 * Constructs the triangle {a,b,c}
	 */
	Skeleton_blocker_simplex(T a, T b, T c)
	{
		add_vertex(a); add_vertex(b); add_vertex(c);
	}

	/**
	 * Constructs the tetrahedron {a,b,c,d}
	 */
	Skeleton_blocker_simplex(T a, T b, T c, T d)
	{
		add_vertex(a); add_vertex(b); add_vertex(c); add_vertex(d);
	}

	/**
	 * Initialize a simplex with a string such as {0,1,2}
	 */
	Skeleton_blocker_simplex(std::string token){
		clear();
		if ((token[0] == '{')  && (token[token.size()-1] == '}' ) )
		{
			token.erase(0,1);
			token.erase(token.size()-1,1);
			while(token.size()!=0 ){
				int coma_position=token.find_first_of(',');
				//cout << "coma_position:"<<coma_position<<endl;
				std::string n = token.substr(0,coma_position);
				//cout << "token:"<<token<<endl;
				token.erase(0,n.size()+1);
				add_vertex((T)(atoi(n.c_str())));
			}
		}
	}

	//@}

	/** @name Simplex manipulation
	 */
	//@{

	/**
	 * Add the vertex v to the simplex:
	 * Note that adding two times the same vertex is
	 * the same that adding it once.
	 * \f$ (*this) \leftarrow (*this) \cup \{ v \} \f$
	 */
	void inline add_vertex(T v)
	{
		simplex_set.insert(v);
	}

	/**
	 * Remove the vertex v from the simplex:
	 * \f$ (*this) \leftarrow (*this) \setminus \{ v \} \f$
	 */
	void inline remove_vertex(T v)
	{
		simplex_set.erase(v);
	}

	/**
	 * Intersects the simplex with the simplex a:
	 * \f$ (*this) \leftarrow (*this) \cap a \f$
	 */
	void intersection(const Skeleton_blocker_simplex & a){
		std::vector<T> v;
		v.reserve(std::min(simplex_set.size(), a.simplex_set.size()));

		set_intersection(simplex_set.begin(),simplex_set.end(),
				a.simplex_set.begin(),a.simplex_set.end(),
				std::back_inserter(v));
		clear();
		for (auto i:v)
			simplex_set.insert(i);
	}

	/**
	 * Substracts a from the simplex:
	 * \f$ (*this) \leftarrow (*this) \setminus a \f$
	 */
	void difference(const Skeleton_blocker_simplex & a){
		std::vector<T> v;
		v.reserve(simplex_set.size());

		set_difference(simplex_set.begin(),simplex_set.end(),
				a.simplex_set.begin(),a.simplex_set.end(),
				std::back_inserter(v));

		clear();
		for (auto i:v)
			simplex_set.insert(i);
	}

	/**
	 * Add vertices of a to the simplex:
	 * \f$ (*this) \leftarrow (*this) \cup a \f$
	 */
	void union_vertices(const Skeleton_blocker_simplex & a){
		std::vector<T> v;
		v.reserve(simplex_set.size() + a.simplex_set.size());

		set_union(simplex_set.begin(),simplex_set.end(),
				a.simplex_set.begin(),a.simplex_set.end(),
				std::back_inserter(v));
		clear();
		simplex_set.insert(v.begin(),v.end());
	}

	typename std::set<T>::const_iterator begin() const{
		return simplex_set.cbegin();
	}

	typename std::set<T>::const_iterator end() const{
		return simplex_set.cend();
	}

	typename std::set<T>::iterator begin(){
		return simplex_set.begin();
	}

	typename std::set<T>::iterator end(){
		return simplex_set.end();
	}



	//@}

	/** @name Queries
	 */
	//@{

	/**
	 * Returns the dimension of the simplex.
	 */
	int inline dimension() const
	{
		return (simplex_set.size() - 1);
	}

	/**
	 * Returns the first vertex of the (oriented) simplex.
	 *
	 * Be careful : assumes the simplex is non-empty.
	 */
	T inline first_vertex() const
	{
		return *(begin());
	}

	//	/**
	//	 * Returns the second vertex of the (oriented) simplex.
	//	 *
	//	 * Be careful : assumes the simplex has at least two vertices.
	//	 */
	//	int inline second_vertex() const
	//	{
	//		assert(simplex_set.size()>=2);
	//		auto it=++(begin());
	//		return *it;
	//	}

	/**
	 * Returns the last vertex of the (oriented) simplex.
	 *
	 * Be careful : assumes the simplex is non-empty.
	 */
	T inline last_vertex() const
	{
		assert(!simplex_set.empty());
		return *(simplex_set.rbegin());
	}
	/**
	 * @return true iff the simplex contains the simplex a, i.e. iff \f$ a \subset (*this) \f$.
	 */
	bool contains(const Skeleton_blocker_simplex & a) const{
		return includes(simplex_set.cbegin(),simplex_set.cend(),a.simplex_set.cbegin(),a.simplex_set.cend());
	}

	/**
	 * @return true iff the simplex contains the difference \f$ a \setminus b \f$.
	 */
	bool contains_difference(const Skeleton_blocker_simplex& a, const Skeleton_blocker_simplex& b) const{
		auto first1 = begin();
		auto last1 = end();

		auto first2 = a.begin();
		auto last2 = a.end();

		while (first2!=last2) {
			// we ignore vertices of b
			if(b.contains(*first2)){
				++first2;
			}
			else{
				if ( (first1==last1) || (*first2<*first1) ) return false;
				if (!(*first1<*first2)) ++first2;
				++first1;
			}
		}
		return true;
	}

	/**
	 * @return true iff the simplex contains the difference \f$ a \setminus \{ x \} \f$.
	 */
	bool contains_difference(const Skeleton_blocker_simplex& a, T x) const{
		auto first1 = begin();
		auto last1 = end();

		auto first2 = a.begin();
		auto last2 = a.end();

		while (first2!=last2) {
			// we ignore vertices of b
			if(x == *first2){
				++first2;
			}
			else{
				if ( (first1==last1) || (*first2<*first1) ) return false;
				if (!(*first1<*first2)) ++first2;
				++first1;
			}
		}
		return true;
	}

	/**
		 * @return true iff the simplex contains the difference \f$ a \setminus \{ x \} \f$.
		 */
		bool contains_difference(const Skeleton_blocker_simplex& a, T x, T y) const{
			auto first1 = begin();
			auto last1 = end();

			auto first2 = a.begin();
			auto last2 = a.end();

			while (first2!=last2) {
				// we ignore vertices of b
				if(x == *first2 || y == *first2){
					++first2;
				}
				else{
					if ( (first1==last1) || (*first2<*first1) ) return false;
					if (!(*first1<*first2)) ++first2;
					++first1;
				}
			}
			return true;
		}


/**
 * @return true iff the simplex contains the vertex v, i.e. iff \f$ v \in (*this) \f$.
 */
bool contains(T v) const{
	return (simplex_set.find(v) != simplex_set.end());
}

/**
 * @return \f$ (*this) \cap a = \emptyset \f$.
 */
bool disjoint(const Skeleton_blocker_simplex& a) const{
	std::vector<T> v;
	v.reserve(std::min(simplex_set.size(), a.simplex_set.size()));

	set_intersection(simplex_set.cbegin(),simplex_set.cend(),
			a.simplex_set.cbegin(),a.simplex_set.cend(),
			std::back_inserter(v));

	return (v.size()==0);
}


bool operator==(const Skeleton_blocker_simplex& other) const{
	return (this->simplex_set == other.simplex_set);
}

bool operator!=(const Skeleton_blocker_simplex& other) const{
	return (this->simplex_set != other.simplex_set);
}

bool operator<(const Skeleton_blocker_simplex& other) const{
	return (std::lexicographical_compare(this->simplex_set.begin(),this->simplex_set.end(),
			other.begin(),other.end()));
}

//@}





/**
 * Display a simplex
 */
friend std::ostream& operator << (std::ostream& o, const Skeleton_blocker_simplex & sigma)
{
	bool first = true;
	o << "{";
	for(auto i : sigma)
	{
		if(first) first = false ;
		else o << ",";
		o << i;
	}
	o << "}";
	return o;
}


};




#endif


