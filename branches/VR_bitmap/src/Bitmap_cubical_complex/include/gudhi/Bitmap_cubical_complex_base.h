/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Pawel Dlotko
 *
 *    Copyright (C) 2015  INRIA Sophia-Saclay (France)
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

#ifndef BITMAP_CUBICAL_COMPLEX_BASE_H_
#define BITMAP_CUBICAL_COMPLEX_BASE_H_

#include <gudhi/Bitmap_cubical_complex/counter.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <limits>
#include <utility>  // for pair<>

namespace Gudhi {

namespace Cubical_complex {

/**
 * This is a class implementing a basic bitmap data structure to store cubical complexes.
 * It implements only the most basic subroutines.
 * The idea of the bitmap is the following. Our aim is to have a memory efficient
 * data structure to store d-dimensional cubical complex
 * C being a cubical decomposition
 * of a rectangular region of a space. This is achieved by storing C as a
 * vector of bits (this is where the name 'bitmap' came from).
 * Each cell is represented by a single
 * bit (in case of black and white bitmaps, or by a single element of a type T
 * (here T is a filtration type of a bitmap, typically a double).
 * All the informations needed for homology and
 * persistent homology computations (like dimension of a cell, boundary and
 * coboundary elements of a cell, are then obtained from the
 * position of the element in C.
 * The default filtration used in this implementation is the lower star filtration.
 */
template <typename T>
class Bitmap_cubical_complex_base {
 public:
  typedef T filtration_type;

  /**
   *Default constructor
   **/
  Bitmap_cubical_complex_base() :
  total_number_of_cells(0) { }
  /**
   * There are a few constructors of a Bitmap_cubical_complex_base class.
   * First one, that takes vector<unsigned>, creates an empty bitmap of a dimension equal
   * the number of elements in the
   * input vector and size in the i-th dimension equal the number in the position i-of the input vector.
   */
  Bitmap_cubical_complex_base(const std::vector<unsigned>& sizes);
  /**
   * The second constructor takes as a input a Perseus style file. For more details,
   * please consult the documentations of
   * Perseus software as well as examples attached to this
   * implementation.
   **/
  Bitmap_cubical_complex_base(const char* perseus_style_file);
  /**
   * The last constructor of a Bitmap_cubical_complex_base class accepts vector of dimensions (as the first one)
   * together with vector of filtration values of top dimensional cells.
   **/
  Bitmap_cubical_complex_base(const std::vector<unsigned>& dimensions, const std::vector<T>& top_dimensional_cells);

  /**
   * The functions get_boundary_of_a_cell, get_coboundary_of_a_cell, get_dimension_of_a_cell
   * and get_cell_data are the basic
   * functions that compute boundary / coboundary / dimension and the filtration
   * value form a position of a cell in the structure of a bitmap. The input parameter of all of those function is a
   * non-negative integer, indicating a position of a cube in the data structure.
   * In the case of functions that compute (co)boundary, the output is a vector if non-negative integers pointing to
   * the positions of (co)boundary element of the input cell.
   */
  virtual inline std::vector< size_t > get_boundary_of_a_cell(size_t cell)const;
  /**
   * The functions get_coboundary_of_a_cell, get_coboundary_of_a_cell,
   * get_dimension_of_a_cell and get_cell_data are the basic
   * functions that compute boundary / coboundary / dimension and the filtration
   * value form a position of a cell in the structure of a bitmap.
   * The input parameter of all of those function is a non-negative integer,
   * indicating a position of a cube in the data structure.
   * In the case of functions that compute (co)boundary, the output is a vector if
   * non-negative integers pointing to the
   * positions of (co)boundary element of the input cell.
   **/
  inline std::vector< size_t > get_coboundary_of_a_cell(size_t cell)const;
  /**
   * In the case of get_dimension_of_a_cell function, the output is a non-negative integer
   * indicating the dimension of a cell.
   **/
  inline unsigned get_dimension_of_a_cell(size_t cell)const;
  /**
   * In the case of get_cell_data, the output parameter is a reference to the value of a cube in a given position.
   **/
  inline T& get_cell_data(size_t cell);


  /**
   * Typical input used to construct a baseBitmap class is a filtration given at the top dimensional cells.
   * Then, there are a few ways one can pick the filtration of lower dimensional
   * cells. The most typical one is by so called lower star filtration. This function is always called by any
   * constructor which takes the top dimensional cells. If you use such a constructor,
   * then there is no need to call this function. Call it only if you are putting the filtration
   * of the cells by your own (for instance by using Top_dimensional_cells_iterator).
   **/
  void impose_lower_star_filtration();  // assume that top dimensional cells are already set.

  /**
   * Returns dimension of a complex.
   **/
  inline unsigned dimension()const {
    return sizes.size();
  }

  /**
   * Returns number of all cubes in the data structure.
   **/
  inline unsigned size_of_bitmap()const {
    return this->data.size();
  }

  /**
   * Writing to stream operator.
   **/
  template <typename K>
  friend std::ostream& operator<<(std::ostream & os, const Bitmap_cubical_complex_base<K>& b);

  // ITERATORS

  /**
   * Iterator through all cells in the complex (in order they appear in the structure -- i.e.
   * in lexicographical order).
   **/
  typedef typename std::vector< T >::iterator all_cells_iterator;

  all_cells_iterator all_cells_begin()const {
    return this->data.begin();
  }

  all_cells_iterator all_cells_end()const {
    return this->data.end();
  }


  typedef typename std::vector< T >::const_iterator all_cells_const_iterator;

  all_cells_const_iterator all_cells_const_begin()const {
    return this->data.begin();
  }

  all_cells_const_iterator all_cells_const_end()const {
    return this->data.end();
  }

  /**
   * Iterator through top dimensional cells of the complex. The cells appear in order they are stored
   * in the structure (i.e. in lexicographical order)
   **/
  class Top_dimensional_cells_iterator : std::iterator< std::input_iterator_tag, double > {
   public:
    Top_dimensional_cells_iterator(Bitmap_cubical_complex_base& b) : b(b) {
      this->counter = std::vector<size_t>(b.dimension());
      // std::fill( this->counter.begin() , this->counter.end() , 0 );
    }

    Top_dimensional_cells_iterator operator++() {
      // first find first element of the counter that can be increased:
      size_t dim = 0;
      while ((dim != this->b.dimension()) && (this->counter[dim] == this->b.sizes[dim] - 1))++dim;

      if (dim != this->b.dimension()) {
        ++this->counter[dim];
        for (size_t i = 0; i != dim; ++i) {
          this->counter[i] = 0;
        }
      } else {
        ++this->counter[0];
      }
      return *this;
    }

    Top_dimensional_cells_iterator operator++(int) {
      Top_dimensional_cells_iterator result = *this;
      ++(*this);
      return result;
    }

    Top_dimensional_cells_iterator& operator=(const Top_dimensional_cells_iterator& rhs) {
      this->counter = rhs.counter;
      this->b = rhs.b;
      return *this;
    }

    bool operator==(const Top_dimensional_cells_iterator& rhs)const {
      if (&this->b != &rhs.b)return false;
      if (this->counter.size() != rhs.counter.size())return false;
      for (size_t i = 0; i != this->counter.size(); ++i) {
        if (this->counter[i] != rhs.counter[i])return false;
      }
      return true;
    }

    bool operator!=(const Top_dimensional_cells_iterator& rhs)const {
      return !(*this == rhs);
    }

    T& operator*() {
      // given the counter, compute the index in the array and return this element.
      unsigned index = 0;
      for (size_t i = 0; i != this->counter.size(); ++i) {
        index += (2 * this->counter[i] + 1) * this->b.multipliers[i];
      }
      return this->b.data[index];
    }

    size_t compute_index_in_bitmap()const {
      size_t index = 0;
      for (size_t i = 0; i != this->counter.size(); ++i) {
        index += (2 * this->counter[i] + 1) * this->b.multipliers[i];
      }
      return index;
    }

    void print_counter()const {
      for (size_t i = 0; i != this->counter.size(); ++i) {
        std::cout << this->counter[i] << " ";
      }
    }
    friend class Bitmap_cubical_complex_base;
   protected:
    std::vector< size_t > counter;
    Bitmap_cubical_complex_base& b;
  };

  Top_dimensional_cells_iterator top_dimensional_cells_begin() {
    Top_dimensional_cells_iterator a(*this);
    return a;
  }

  Top_dimensional_cells_iterator top_dimensional_cells_end() {
    Top_dimensional_cells_iterator a(*this);
    for (size_t i = 0; i != this->dimension(); ++i) {
      a.counter[i] = this->sizes[i] - 1;
    }
    a.counter[0]++;
    return a;
  }


  //****************************************************************************************************************//
  //****************************************************************************************************************//
  //****************************************************************************************************************//
  //****************************************************************************************************************//

  inline size_t number_cells()const {
    return this->total_number_of_cells;
  }

  //****************************************************************************************************************//
  //****************************************************************************************************************//
  //****************************************************************************************************************//
  //****************************************************************************************************************//

 protected:
  std::vector<unsigned> sizes;
  std::vector<unsigned> multipliers;
  std::vector<T> data;
  size_t total_number_of_cells;

  void set_up_containers(const std::vector<unsigned>& sizes) {
    unsigned multiplier = 1;
    for (size_t i = 0; i != sizes.size(); ++i) {
      this->sizes.push_back(sizes[i]);
      this->multipliers.push_back(multiplier);
      multiplier *= 2 * sizes[i] + 1;
    }
    this->data = std::vector<T>(multiplier, std::numeric_limits<T>::max());
    this->total_number_of_cells = multiplier;
  }

  size_t compute_position_in_bitmap(const std::vector< unsigned >& counter) {
    size_t position = 0;
    for (size_t i = 0; i != this->multipliers.size(); ++i) {
      position += this->multipliers[i] * counter[i];
    }
    return position;
  }

  std::vector<unsigned> compute_counter_for_given_cell(size_t cell)const {
    std::vector<unsigned> counter;
    counter.reserve(this->sizes.size());
    for (size_t dim = this->sizes.size(); dim != 0; --dim) {
      counter.push_back(cell / this->multipliers[dim - 1]);
      cell = cell % this->multipliers[dim - 1];
    }
    std::reverse(counter.begin(), counter.end());
    return counter;
  }
  void read_perseus_style_file(const char* perseus_style_file);
  void setup_bitmap_based_on_top_dimensional_cells_list(const std::vector<unsigned>& sizes_in_following_directions,
                                                        const std::vector<T>& top_dimensional_cells);
  Bitmap_cubical_complex_base(const char* perseus_style_file, std::vector<bool> directions);
  Bitmap_cubical_complex_base(const std::vector<unsigned>& sizes, std::vector<bool> directions);
  Bitmap_cubical_complex_base(const std::vector<unsigned>& dimensions, const std::vector<T>& top_dimensional_cells,
                              std::vector<bool> directions);
};

template <typename K>
std::ostream& operator<<(std::ostream & out, const Bitmap_cubical_complex_base<K>& b) {
  for (typename Bitmap_cubical_complex_base<K>::all_cells_const_iterator
       it = b.all_cells_const_begin(); it != b.all_cells_const_end(); ++it) {
    out << *it << " ";
  }
  return out;
}

template <typename T>
Bitmap_cubical_complex_base<T>::Bitmap_cubical_complex_base(const std::vector<unsigned>& sizes) {
  this->set_up_containers(sizes);
}

template <typename T>
void Bitmap_cubical_complex_base<T>::setup_bitmap_based_on_top_dimensional_cells_list(const std::vector<unsigned>& sizes_in_following_directions,
                                                                                      const std::vector<T>& top_dimensional_cells) {
  this->set_up_containers(sizes_in_following_directions);

  size_t number_of_top_dimensional_elements = 1;
  for (size_t i = 0; i != sizes_in_following_directions.size(); ++i) {
    number_of_top_dimensional_elements *= sizes_in_following_directions[i];
  }
  if (number_of_top_dimensional_elements != top_dimensional_cells.size()) {
    std::cerr <<
        "Error in constructor\
        Bitmap_cubical_complex_base\
       ( std::vector<size_t> sizes_in_following_directions , std::vector<T> top_dimensional_cells ).\
       Number of top dimensional elements that follow from sizes_in_following_directions vector is different\
       than the size of top_dimensional_cells vector." << std::endl;
    throw ("Error in constructor Bitmap_cubical_complex_base( std::vector<size_t> sizes_in_following_directions,\
        std::vector<T> top_dimensional_cells )\
      . Number of top dimensional elements that follow from sizes_in_following_directions vector is different than the\
       size of top_dimensional_cells vector.");
  }

  Bitmap_cubical_complex_base<T>::Top_dimensional_cells_iterator it(*this);
  size_t index = 0;
  for (it = this->top_dimensional_cells_begin(); it != this->top_dimensional_cells_end(); ++it) {
    (*it) = top_dimensional_cells[index];
    ++index;
  }
  this->impose_lower_star_filtration();
}

template <typename T>
Bitmap_cubical_complex_base<T>::Bitmap_cubical_complex_base
(const std::vector<unsigned>& sizes_in_following_directions, const std::vector<T>& top_dimensional_cells) {
  this->setup_bitmap_based_on_top_dimensional_cells_list(sizes_in_following_directions, top_dimensional_cells);
}

template <typename T>
void Bitmap_cubical_complex_base<T>::read_perseus_style_file(const char* perseus_style_file) {
  bool dbg = false;
  std::ifstream inFiltration;
  inFiltration.open(perseus_style_file);
  unsigned dimensionOfData;
  inFiltration >> dimensionOfData;

  if (dbg) {
    std::cerr << "dimensionOfData : " << dimensionOfData << std::endl;
  }

  std::vector<unsigned> sizes;
  sizes.reserve(dimensionOfData);
  for (size_t i = 0; i != dimensionOfData; ++i) {
    unsigned size_in_this_dimension;
    inFiltration >> size_in_this_dimension;
    sizes.push_back(size_in_this_dimension);
    if (dbg) {
      std::cerr << "size_in_this_dimension : " << size_in_this_dimension << std::endl;
    }
  }
  this->set_up_containers(sizes);

  Bitmap_cubical_complex_base<T>::Top_dimensional_cells_iterator it(*this);
  it = this->top_dimensional_cells_begin();

  while (!inFiltration.eof()) {
    double filtrationLevel;
    inFiltration >> filtrationLevel;
    if (dbg) {
      std::cerr << "Cell of an index : "
          << it.compute_index_in_bitmap()
          << " and dimension: "
          << this->get_dimension_of_a_cell(it.compute_index_in_bitmap())
          << " get the value : " << filtrationLevel << std::endl;
    }
    *it = filtrationLevel;
    ++it;
  }
  inFiltration.close();
  this->impose_lower_star_filtration();
}

template <typename T>
Bitmap_cubical_complex_base<T>::Bitmap_cubical_complex_base(const char* perseus_style_file,
                                                            std::vector<bool> directions) {
  // this constructor is here just for compatibility with a class that creates cubical complexes with periodic boundary
  // conditions. It ignores the last parameter of the function.
  this->read_perseus_style_file(perseus_style_file);
}

template <typename T>
Bitmap_cubical_complex_base<T>::Bitmap_cubical_complex_base(const std::vector<unsigned>& sizes,
                                                            std::vector<bool> directions) :
  total_number_of_cells(0) {
  // this constructor is here just for compatibility with a class that creates cubical complexes with periodic boundary
  // conditions. It ignores the last parameter of the function.
  this->set_up_containers(sizes);
}

template <typename T>
Bitmap_cubical_complex_base<T>::Bitmap_cubical_complex_base(const std::vector<unsigned>& dimensions,
                                                            const std::vector<T>& top_dimensional_cells,
                                                            std::vector<bool> directions) :
  total_number_of_cells(0) {
  // this constructor is here just for compatibility with a class that creates cubical complexes with periodic
  // boundary conditions. It ignores the last parameter of the function.
  this->setup_bitmap_based_on_top_dimensional_cells_list(dimensions, top_dimensional_cells);
}

template <typename T>
Bitmap_cubical_complex_base<T>::Bitmap_cubical_complex_base(const char* perseus_style_file) :
  total_number_of_cells(0) {
  this->read_perseus_style_file(perseus_style_file);
}

template <typename T>
std::vector< size_t > Bitmap_cubical_complex_base<T>::get_boundary_of_a_cell(size_t cell)const {
  std::vector< size_t > boundary_elements;

  // Speed traded of for memory. Check if it is better in practice.
  boundary_elements.reserve(this->dimension()*2);

  size_t cell1 = cell;
  for (size_t i = this->multipliers.size(); i != 0; --i) {
    unsigned position = cell1 / this->multipliers[i - 1];
    if (position % 2 == 1) {
      boundary_elements.push_back(cell - this->multipliers[ i - 1 ]);
      boundary_elements.push_back(cell + this->multipliers[ i - 1 ]);
    }
    cell1 = cell1 % this->multipliers[i - 1];
  }
  return boundary_elements;
}

template <typename T>
std::vector< size_t > Bitmap_cubical_complex_base<T>::get_coboundary_of_a_cell(size_t cell)const {
  std::vector<unsigned> counter = this->compute_counter_for_given_cell(cell);
  std::vector< size_t > coboundary_elements;
  size_t cell1 = cell;
  for (size_t i = this->multipliers.size(); i != 0; --i) {
    unsigned position = cell1 / this->multipliers[i - 1];
    if (position % 2 == 0) {
      if ((cell > this->multipliers[i - 1]) && (counter[i - 1] != 0)) {
        coboundary_elements.push_back(cell - this->multipliers[i - 1]);
      }
      if (
          (cell + this->multipliers[i - 1] < this->data.size()) && (counter[i - 1] != 2 * this->sizes[i - 1])) {
        coboundary_elements.push_back(cell + this->multipliers[i - 1]);
      }
    }
    cell1 = cell1 % this->multipliers[i - 1];
  }
  return coboundary_elements;
}

template <typename T>
unsigned Bitmap_cubical_complex_base<T>::get_dimension_of_a_cell(size_t cell)const {
  bool dbg = false;
  if (dbg) std::cerr << "\n\n\n Computing position o a cell of an index : " << cell << std::endl;
  unsigned dimension = 0;
  for (size_t i = this->multipliers.size(); i != 0; --i) {
    unsigned position = cell / this->multipliers[i - 1];

    if (dbg) std::cerr << "i-1 :" << i - 1 << std::endl;
    if (dbg) std::cerr << "cell : " << cell << std::endl;
    if (dbg) std::cerr << "position : " << position << std::endl;
    if (dbg) std::cerr << "multipliers[" << i - 1 << "] = " << this->multipliers[i - 1] << std::endl;
    if (dbg) getchar();

    if (position % 2 == 1) {
      if (dbg)std::cerr << "Nonzero length in this direction \n";
      dimension++;
    }
    cell = cell % this->multipliers[i - 1];
  }
  return dimension;
}

template <typename T>
inline T& Bitmap_cubical_complex_base<T>::get_cell_data(size_t cell) {
  return this->data[cell];
}

template <typename T>
void Bitmap_cubical_complex_base<T>::impose_lower_star_filtration() {
  bool dbg = false;

  // this vector will be used to check which elements have already been taken care of
  // in imposing lower star filtration:
  std::vector<bool> is_this_cell_considered(this->data.size(), false);

  size_t size_to_reserve = 1;
  for (size_t i = 0; i != this->multipliers.size(); ++i) {
    size_to_reserve *= (size_t) ((this->multipliers[i] - 1) / 2);
  }

  std::vector<size_t> indices_to_consider;
  indices_to_consider.reserve(size_to_reserve);
  // we assume here that we already have a filtration on the top dimensional cells and
  // we have to extend it to lower ones.
  typename Bitmap_cubical_complex_base<T>::Top_dimensional_cells_iterator it(*this);
  for (it = this->top_dimensional_cells_begin(); it != this->top_dimensional_cells_end(); ++it) {
    indices_to_consider.push_back(it.compute_index_in_bitmap());
  }

  while (indices_to_consider.size()) {
    if (dbg) {
      std::cerr << "indices_to_consider in this iteration \n";
      for (size_t i = 0; i != indices_to_consider.size(); ++i) {
        std::cout << indices_to_consider[i] << "  ";
      }
      getchar();
    }
    std::vector<size_t> new_indices_to_consider;
    for (size_t i = 0; i != indices_to_consider.size(); ++i) {
      std::vector<size_t> bd = this->get_boundary_of_a_cell(indices_to_consider[i]);
      for (size_t boundaryIt = 0; boundaryIt != bd.size(); ++boundaryIt) {
        if (dbg) {
          std::cerr << "filtration of a cell : " << bd[boundaryIt] << " is : " << this->data[ bd[boundaryIt] ] <<
              " while of a cell: " << indices_to_consider[i] << " is: " << this->data[ indices_to_consider[i] ] <<
              std::endl;
          getchar();
        }
        if (this->data[ bd[boundaryIt] ] > this->data[ indices_to_consider[i] ]) {
          this->data[ bd[boundaryIt] ] = this->data[ indices_to_consider[i] ];
          if (dbg) {
            std::cerr << "Setting the value of a cell : " << bd[boundaryIt] << " to : " <<
                this->data[ indices_to_consider[i] ] << std::endl;
            getchar();
          }
        }
        if (is_this_cell_considered[ bd[boundaryIt] ] == false) {
          new_indices_to_consider.push_back(bd[boundaryIt]);
          is_this_cell_considered[ bd[boundaryIt] ] = true;
        }
      }
    }
    indices_to_consider.swap(new_indices_to_consider);
  }
}

template <typename T>
bool compareFirstElementsOfTuples(const std::pair< std::pair< T, size_t >, char >& first,
                                  const std::pair< std::pair< T, size_t >, char >& second) {
  if (first.first.first < second.first.first) {
    return true;
  } else {
    if (first.first.first > second.first.first) {
      return false;
    }
    // in this case first.first.first == second.first.first, so we need to compare dimensions
    return first.second < second.second;
  }
}

}  // namespace Cubical_complex

}  // namespace Gudhi

#endif  // BITMAP_CUBICAL_COMPLEX_BASE_H_
