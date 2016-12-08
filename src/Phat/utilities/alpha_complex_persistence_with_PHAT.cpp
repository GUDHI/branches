#include <boost/program_options.hpp>

#include <CGAL/Epick_d.h>

#include <gudhi/Alpha_complex.h>
#include <gudhi/Compute_persistence_with_phat.h>

#include <iostream>
#include <string>
#include <limits>  // for numeric_limits




void program_options(int argc, char * argv[]
                     , std::string & off_file_points
                     , std::string & output_file_diag
                     , Filtration_value & alpha_square_max_value
                     , int & coeff_field_characteristic
                     , Filtration_value & min_persistence);

int main(int argc, char **argv) {
  std::string off_file_points;
  std::string output_file_diag;
  Filtration_value alpha_square_max_value;
  int coeff_field_characteristic;
  Filtration_value min_persistence;

  program_options(argc, argv, off_file_points, output_file_diag, alpha_square_max_value,
                  coeff_field_characteristic, min_persistence);

  // ----------------------------------------------------------------------------
  // Init of an alpha complex from an OFF file
  // ----------------------------------------------------------------------------
  using Kernel = CGAL::Epick_d< CGAL::Dynamic_dimension_tag >;
  Gudhi::alphacomplex::Alpha_complex<Kernel> alpha_complex_from_file(off_file_points, alpha_square_max_value);

  // ----------------------------------------------------------------------------
  // Display information about the alpha complex
  // ----------------------------------------------------------------------------
  std::cout << "Alpha complex is of dimension " << alpha_complex_from_file.dimension() <<
      " - " << alpha_complex_from_file.num_simplices() << " simplices - " <<
      alpha_complex_from_file.num_vertices() << " vertices." << std::endl;

  // Sort the simplices in the order of the filtration
  alpha_complex_from_file.initialize_filtration();

  std::cout << "Simplex_tree dim: " << alpha_complex_from_file.dimension() << std::endl;
  
  Gudhi::phat_interface::Compute_persistence_with_phat< Gudhi::alphacomplex::Alpha_complex<Kernel> > phat(&alpha_complex_from_file); 
  phat.compute_persistence_pairs();
  std::pair< std::vector< std::vector<Filtration_value> >, std::vector< std::vector< std::pair<Filtration_value, Filtration_value> > > > persistence = phat.get_the_intervals();
  Gudhi::phat_interface::write_intervals_to_file_Gudhi_format<Filtration_value>(persistence, output_file_diag.c_str());
 
  return 0;
}

void program_options(int argc, char * argv[]
                     , std::string & off_file_points
                     , std::string & output_file_diag
                     , Filtration_value & alpha_square_max_value
                     , int & coeff_field_characteristic
                     , Filtration_value & min_persistence) {
  namespace po = boost::program_options;
  po::options_description hidden("Hidden options");
  hidden.add_options()
      ("input-file", po::value<std::string>(&off_file_points),
       "Name of file containing a point set. Format is one point per line:   X1 ... Xd ");

  po::options_description visible("Allowed options", 100);
  visible.add_options()
      ("help,h", "produce help message")
      ("output-file,o", po::value<std::string>(&output_file_diag)->default_value(std::string()),
       "Name of file in which the persistence diagram is written. Default print in std::cout")
      ("max-alpha-square-value,r",
      po::value<Filtration_value>(&alpha_square_max_value)->default_value(std::numeric_limits<Filtration_value>::infinity()),
       "Maximal alpha square value for the Alpha complex construction.")
      ("field-charac,p", po::value<int>(&coeff_field_characteristic)->default_value(11),
       "Characteristic p of the coefficient field Z/pZ for computing homology.")
      ("min-persistence,m", po::value<Filtration_value>(&min_persistence),
       "Minimal lifetime of homology feature to be recorded. Default is 0. Enter a negative value to see zero length intervals");

  po::positional_options_description pos;
  pos.add("input-file", 1);

  po::options_description all;
  all.add(visible).add(hidden);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
            options(all).positional(pos).run(), vm);
  po::notify(vm);

  if (vm.count("help") || !vm.count("input-file")) {
    std::cout << std::endl;
    std::cout << "Compute the persistent homology with coefficient field Z/pZ \n";
    std::cout << "of an Alpha complex defined on a set of input points.\n \n";
    std::cout << "The output diagram contains one bar per line, written with the convention: \n";
    std::cout << "   p   dim b d \n";
    std::cout << "where dim is the dimension of the homological feature,\n";
    std::cout << "b and d are respectively the birth and death of the feature and \n";
    std::cout << "p is the characteristic of the field Z/pZ used for homology coefficients." << std::endl << std::endl;

    std::cout << "Usage: " << argv[0] << " [options] input-file" << std::endl << std::endl;
    std::cout << visible << std::endl;
    std::abort();
  }
}
