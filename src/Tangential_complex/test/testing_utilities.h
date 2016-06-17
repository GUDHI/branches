/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Clement Jamin
 *
 *    Copyright (C) 2016 INRIA
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

#ifndef GUDHI_TC_TEST_TEST_UTILITIES_H
#define GUDHI_TC_TEST_TEST_UTILITIES_H

#include <gudhi/Spatial_tree_data_structure.h>
#include <gudhi/Tangential_complex/utilities.h>

#include <CGAL/Random.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/point_generators_d.h>
#include <CGAL/function_objects.h>
#include <CGAL/IO/Triangulation_off_ostream.h>
#include <CGAL/iterator.h>

#include <fstream>
#include <iterator>

// construct_point: dim 2
template <typename Kernel>
typename Kernel::Point_d construct_point(
  const Kernel &k,
  typename Kernel::FT x1, typename Kernel::FT x2)
{
  typename Kernel::FT tab[2];
  tab[0] = x1; tab[1] = x2;
  return k.construct_point_d_object()(2, &tab[0], &tab[2]);
}

// construct_point: dim 3
template <typename Kernel>
typename Kernel::Point_d construct_point(
  const Kernel &k,
  typename Kernel::FT x1, typename Kernel::FT x2, typename Kernel::FT x3)
{
  typename Kernel::FT tab[3];
  tab[0] = x1; tab[1] = x2; tab[2] = x3;
  return k.construct_point_d_object()(3, &tab[0], &tab[3]);
}

// construct_point: dim 4
template <typename Kernel>
typename Kernel::Point_d construct_point(
  const Kernel &k,
  typename Kernel::FT x1, typename Kernel::FT x2, typename Kernel::FT x3,
  typename Kernel::FT x4)
{
  typename Kernel::FT tab[4];
  tab[0] = x1; tab[1] = x2; tab[2] = x3; tab[3] = x4;
  return k.construct_point_d_object()(4, &tab[0], &tab[4]);
}

// construct_point: dim 5
template <typename Kernel>
typename Kernel::Point_d construct_point(
  const Kernel &k,
  typename Kernel::FT x1, typename Kernel::FT x2, typename Kernel::FT x3,
  typename Kernel::FT x4, typename Kernel::FT x5)
{
  typename Kernel::FT tab[5];
  tab[0] = x1; tab[1] = x2; tab[2] = x3; tab[3] = x4; tab[4] = x5;
  return k.construct_point_d_object()(5, &tab[0], &tab[5]);
}

// construct_point: dim 6
template <typename Kernel>
typename Kernel::Point_d construct_point(
  const Kernel &k,
  typename Kernel::FT x1, typename Kernel::FT x2, typename Kernel::FT x3,
  typename Kernel::FT x4, typename Kernel::FT x5, typename Kernel::FT x6)
{
  typename Kernel::FT tab[6];
  tab[0] = x1; tab[1] = x2; tab[2] = x3; tab[3] = x4; tab[4] = x5; tab[5] = x6;
  return k.construct_point_d_object()(6, &tab[0], &tab[6]);
}

template <typename Kernel>
std::vector<typename Kernel::Point_d> generate_points_on_plane(
  std::size_t num_points, int intrinsic_dim, int ambient_dim)
{
  typedef typename Kernel::Point_d Point;
  typedef typename Kernel::FT FT;
  Kernel k;
  CGAL::Random rng;
  std::vector<Point> points;
  points.reserve(num_points);
  for (std::size_t i = 0 ; i < num_points ; )
  {
    std::vector<FT> pt(ambient_dim, FT(0));
    for (int j = 0 ; j < intrinsic_dim ; ++j)
      pt[j] = rng.get_double(-5., 5.);
    /*for (int j = intrinsic_dim ; j < ambient_dim ; ++j)
      pt[j] = rng.get_double(-0.01, 0.01);*/
        
    Point p = k.construct_point_d_object()(ambient_dim, pt.begin(), pt.end());
    points.push_back(p);
    ++i;
  }
  return points;
}

template <typename Kernel>
std::vector<typename Kernel::Point_d> generate_points_on_moment_curve(
  std::size_t num_points, int dim,
  typename Kernel::FT min_x , typename Kernel::FT max_x)
{
  typedef typename Kernel::Point_d Point;
  typedef typename Kernel::FT FT;
  Kernel k;
  CGAL::Random rng;
  std::vector<Point> points;
  points.reserve(num_points);
  for (std::size_t i = 0 ; i < num_points ; )
  {
    FT x = rng.get_double(min_x, max_x);
    std::vector<FT> coords;
    coords.reserve(dim);
    for (int p = 1 ; p <= dim ; ++p)
      coords.push_back(std::pow(CGAL::to_double(x), p));
    Point p = k.construct_point_d_object()(
      dim, coords.begin(), coords.end());
    points.push_back(p);
    ++i;
  }
  return points;
}


// R = big radius, r = small radius
template <typename Kernel/*, typename TC_basis*/>
std::vector<typename Kernel::Point_d> generate_points_on_torus_3D(
  std::size_t num_points, double R, double r, bool uniform = false
  /*, std::vector<TC_basis> *p_tangent_planes = NULL*/)
{
  typedef typename Kernel::Point_d Point;
  typedef typename Kernel::Vector_d Vector;
  typedef typename Kernel::FT FT;
  Kernel k;
  CGAL::Random rng;
  
  //typename Kernel::Construct_vector_d cstr_vec = k.construct_vector_d_object();

  // if uniform
  std::size_t num_lines = (std::size_t)sqrt(num_points);
  std::size_t num_cols = num_points/num_lines + 1;

  std::vector<Point> points;
  points.reserve(num_points);
  for (std::size_t i = 0 ; i < num_points ; )
  {
    FT u, v;
    if (uniform)
    {
      std::size_t k1 = i / num_lines;
      std::size_t k2 = i % num_lines;
      u = 6.2832 * k1 / num_lines;
      v = 6.2832 * k2 / num_lines;
    }
    else
    {
      u = rng.get_double(0, 6.2832);
      v = rng.get_double(0, 6.2832);
    }
    double tmp = cos(u/2)*sin(v) - sin(u/2)*sin(2.*v);
    Point p = construct_point(k,
      (R + r * std::cos(u)) * std::cos(v),
      (R + r * std::cos(u)) * std::sin(v),
      r * std::sin(u));
    points.push_back(p);
    ++i;
    /*if (p_tangent_planes)
    {
      TC_basis tp(p);
      tp.push_back(cstr_vec(
        -r * std::cos(v) * std::sin(u),
        -r * std::sin(v) * std::sin(u),
        r * std::cos(u)));
      tp.push_back(cstr_vec(
        -(R + r * std::cos(u)) * std::sin(v),
        (R + r * std::cos(u)) * std::cos(v),
        0));
      p_tangent_planes->push_back(
        CGAL::tangential_complex::internal::compute_gram_schmidt_basis(sp, k));
    }*/
  }
  return points;
}

template <typename Kernel, typename OutputIterator>
static void generate_uniform_points_on_torus_d(
  const Kernel &k, int dim, std::size_t num_slices,
  OutputIterator out,
  double radius_noise_percentage = 0., 
  std::vector<typename Kernel::FT> current_point = std::vector<typename Kernel::FT>())
{
  static CGAL::Random rng;
  if (current_point.size() == 2*dim)
  {
    *out++ = k.construct_point_d_object()(
      static_cast<int>(current_point.size()), 
      current_point.begin(), current_point.end());
  }
  else
  {
    for (std::size_t slice_idx = 0 ; slice_idx < num_slices ; ++slice_idx)
    {
      double radius_noise_ratio = 1.;
      if (radius_noise_percentage > 0.)
      {
        radius_noise_ratio = rng.get_double(
          (100. - radius_noise_percentage)/100., 
          (100. + radius_noise_percentage)/100.);
      }
      std::vector<typename Kernel::FT> cp2 = current_point;
      FT alpha = 6.2832 * slice_idx / num_slices;
      cp2.push_back(radius_noise_ratio*std::cos(alpha));
      cp2.push_back(radius_noise_ratio*std::sin(alpha));
      generate_uniform_points_on_torus_d(
        k, dim, num_slices, out, radius_noise_percentage, cp2);
    }
  }
}

template <typename Kernel>
std::vector<typename Kernel::Point_d> generate_points_on_torus_d(
  std::size_t num_points, int dim, bool uniform = false, 
  double radius_noise_percentage = 0.)
{
  typedef typename Kernel::Point_d Point;
  typedef typename Kernel::FT FT;
  Kernel k;
  static CGAL::Random rng;

  std::vector<Point> points;
  points.reserve(num_points);
  if (uniform)
  {
    std::size_t num_slices = (std::size_t)std::pow(num_points, 1./dim);
    generate_uniform_points_on_torus_d(
      k, dim, num_slices, std::back_inserter(points), radius_noise_percentage);
  }
  else
  {
    for (std::size_t i = 0 ; i < num_points ; )
    {
      double radius_noise_ratio = 1.;
      if (radius_noise_percentage > 0.)
      {
        radius_noise_ratio = rng.get_double(
          (100. - radius_noise_percentage)/100., 
          (100. + radius_noise_percentage)/100.);
      }
      std::vector<typename Kernel::FT> pt;
      pt.reserve(dim*2);
      for (int curdim = 0 ; curdim < dim ; ++curdim)
      {
        FT alpha = rng.get_double(0, 6.2832);
        pt.push_back(radius_noise_ratio*std::cos(alpha));
        pt.push_back(radius_noise_ratio*std::sin(alpha));
      }

      Point p = k.construct_point_d_object()(pt.begin(), pt.end());
      points.push_back(p);
      ++i;
    }
  }
  return points;
}

template <typename Kernel>
std::vector<typename Kernel::Point_d> generate_points_on_sphere_d(
  std::size_t num_points, int dim, double radius, 
  double radius_noise_percentage = 0.)
{
  typedef typename Kernel::Point_d Point;
  Kernel k;
  CGAL::Random rng;
  CGAL::Random_points_on_sphere_d<Point> generator(dim, radius);
  std::vector<Point> points;
  points.reserve(num_points);
  for (std::size_t i = 0 ; i < num_points ; )
  {
    Point p = *generator++;
    if (radius_noise_percentage > 0.)
    {
      double radius_noise_ratio = rng.get_double(
        (100. - radius_noise_percentage)/100., 
        (100. + radius_noise_percentage)/100.);
      
      typename Kernel::Point_to_vector_d k_pt_to_vec =
        k.point_to_vector_d_object();
      typename Kernel::Vector_to_point_d k_vec_to_pt =
        k.vector_to_point_d_object();
      typename Kernel::Scaled_vector_d k_scaled_vec =
        k.scaled_vector_d_object();
      p = k_vec_to_pt(k_scaled_vec(k_pt_to_vec(p), radius_noise_ratio));
    }
    points.push_back(p);
    ++i;
  }
  return points;
}

template <typename Kernel>
std::vector<typename Kernel::Point_d> generate_points_on_two_spheres_d(
  std::size_t num_points, int dim, double radius, 
  double distance_between_centers, double radius_noise_percentage = 0.)
{
  typedef typename Kernel::FT FT;
  typedef typename Kernel::Point_d Point;
  typedef typename Kernel::Vector_d Vector;
  Kernel k;
  CGAL::Random rng;
  CGAL::Random_points_on_sphere_d<Point> generator(dim, radius);
  std::vector<Point> points;
  points.reserve(num_points);

  std::vector<FT> t(dim, FT(0));
  t[0] = distance_between_centers;
  Vector c1_to_c2(t.begin(), t.end());

  for (std::size_t i = 0 ; i < num_points ; )
  {
    Point p = *generator++;
    if (radius_noise_percentage > 0.)
    { 
      double radius_noise_ratio = rng.get_double(
        (100. - radius_noise_percentage)/100., 
        (100. + radius_noise_percentage)/100.);
      
      typename Kernel::Point_to_vector_d k_pt_to_vec =
        k.point_to_vector_d_object();
      typename Kernel::Vector_to_point_d k_vec_to_pt =
        k.vector_to_point_d_object();
      typename Kernel::Scaled_vector_d k_scaled_vec =
        k.scaled_vector_d_object();
      p = k_vec_to_pt(k_scaled_vec(k_pt_to_vec(p), radius_noise_ratio));
    }
    
    typename Kernel::Translated_point_d k_transl =
      k.translated_point_d_object();
    Point p2 = k_transl(p, c1_to_c2);
    points.push_back(p);
    points.push_back(p2);
    i += 2;
  }
  return points;
}

// Product of a 3-sphere and a circle => d = 3 / D = 5
template <typename Kernel>
std::vector<typename Kernel::Point_d> generate_points_on_3sphere_and_circle(
  std::size_t num_points, double sphere_radius)
{
  typedef typename Kernel::FT FT;
  typedef typename Kernel::Point_d Point;
  typedef typename Kernel::Vector_d Vector;
  Kernel k;
  CGAL::Random rng;
  CGAL::Random_points_on_sphere_d<Point> generator(3, sphere_radius);
  std::vector<Point> points;
  points.reserve(num_points);
  
  typename Kernel::Translated_point_d k_transl =
    k.translated_point_d_object();
  typename Kernel::Compute_coordinate_d k_coord =
    k.compute_coordinate_d_object();
  for (std::size_t i = 0 ; i < num_points ; )
  {
    Point p_sphere = *generator++; // First 3 coords

    FT alpha = rng.get_double(0, 6.2832);
    std::vector<FT> pt(5);
    pt[0] = k_coord(p_sphere, 0);
    pt[1] = k_coord(p_sphere, 1);
    pt[2] = k_coord(p_sphere, 2);
    pt[3] = std::cos(alpha);
    pt[4] = std::sin(alpha);
    Point p(pt.begin(), pt.end());
    points.push_back(p);
    ++i;
  }
  return points;
}

// a = big radius, b = small radius
template <typename Kernel>
std::vector<typename Kernel::Point_d> generate_points_on_klein_bottle_3D(
  std::size_t num_points, double a, double b, bool uniform = false)
{
  typedef typename Kernel::Point_d Point;
  typedef typename Kernel::FT FT;
  Kernel k;
  CGAL::Random rng;

  // if uniform
  std::size_t num_lines = (std::size_t)sqrt(num_points);
  std::size_t num_cols = num_points/num_lines + 1;

  std::vector<Point> points;
  points.reserve(num_points);
  for (std::size_t i = 0 ; i < num_points ; )
  {
    FT u, v;
    if (uniform)
    {
      std::size_t k1 = i / num_lines;
      std::size_t k2 = i % num_lines;
      u = 6.2832 * k1 / num_lines;
      v = 6.2832 * k2 / num_lines;
    }
    else
    {
      u = rng.get_double(0, 6.2832);
      v = rng.get_double(0, 6.2832);
    }
    double tmp = cos(u/2)*sin(v) - sin(u/2)*sin(2.*v);
    Point p = construct_point(k,
      (a + b*tmp)*cos(u),
      (a + b*tmp)*sin(u),
      b*(sin(u/2)*sin(v) + cos(u/2)*sin(2.*v)));
    points.push_back(p);
    ++i;
  }
  return points;
}

// a = big radius, b = small radius
template <typename Kernel>
std::vector<typename Kernel::Point_d> generate_points_on_klein_bottle_4D(
  std::size_t num_points, double a, double b, double noise = 0., bool uniform = false)
{
  typedef typename Kernel::Point_d Point;
  typedef typename Kernel::FT FT;
  Kernel k;
  CGAL::Random rng;

  // if uniform
  std::size_t num_lines = (std::size_t)sqrt(num_points);
  std::size_t num_cols = num_points/num_lines + 1;

  std::vector<Point> points;
  points.reserve(num_points);
  for (std::size_t i = 0 ; i < num_points ; )
  {
    FT u, v;
    if (uniform)
    {
      std::size_t k1 = i / num_lines;
      std::size_t k2 = i % num_lines;
      u = 6.2832 * k1 / num_lines;
      v = 6.2832 * k2 / num_lines;
    }
    else
    {
      u = rng.get_double(0, 6.2832);
      v = rng.get_double(0, 6.2832);
    }
    Point p = construct_point(k,
        (a + b*cos(v))*cos(u) + (noise == 0. ? 0. : rng.get_double(0, noise)),
        (a + b*cos(v))*sin(u) + (noise == 0. ? 0. : rng.get_double(0, noise)),
        b*sin(v)*cos(u/2)     + (noise == 0. ? 0. : rng.get_double(0, noise)),
        b*sin(v)*sin(u/2)     + (noise == 0. ? 0. : rng.get_double(0, noise)));
    points.push_back(p);
    ++i;
  }
  return points;
}


// a = big radius, b = small radius
template <typename Kernel>
std::vector<typename Kernel::Point_d>
generate_points_on_klein_bottle_variant_5D(
  std::size_t num_points, double a, double b, bool uniform = false)
{
  typedef typename Kernel::Point_d Point;
  typedef typename Kernel::FT FT;
  Kernel k;
  CGAL::Random rng;

  // if uniform
  std::size_t num_lines = (std::size_t)sqrt(num_points);
  std::size_t num_cols = num_points/num_lines + 1;

  std::vector<Point> points;
  points.reserve(num_points);
  for (std::size_t i = 0 ; i < num_points ; )
  {
    FT u, v;
    if (uniform)
    {
      std::size_t k1 = i / num_lines;
      std::size_t k2 = i % num_lines;
      u = 6.2832 * k1 / num_lines;
      v = 6.2832 * k2 / num_lines;
    }
    else
    {
      u = rng.get_double(0, 6.2832);
      v = rng.get_double(0, 6.2832);
    }
    FT x1 = (a + b*cos(v))*cos(u);
    FT x2 = (a + b*cos(v))*sin(u);
    FT x3 = b*sin(v)*cos(u/2);
    FT x4 = b*sin(v)*sin(u/2);
    FT x5 = x1 + x2 + x3 + x4;

    Point p = construct_point(k, x1, x2, x3, x4, x5);
    points.push_back(p);
    ++i;
  }
  return points;
}

#endif // GUDHI_MESH_3_TEST_TEST_UTILITIES_H
