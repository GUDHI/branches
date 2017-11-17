/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Pawel Dlotko
 *
 *    Copyright (C) 2017  Swansea University UK
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


#include <gudhi/reader_utils.h>
#include <gudhi/Hasse_diagram.h>
#include <gudhi/Persistent_cohomology.h>

// standard stuff
#include <iostream>
#include <string>
#include <vector>
#include <cstddef>

int main(int argc, char** argv) 
{ 
	typedef Gudhi::Hasse_diagram::Cell<int,double> Cell;
	//in this example we will construct a CW decomposition of two dimensional 
	//torus:
	//  ______________________
	//  |A|__B___|C|___D___|A|     
	//  | |      | |       | | 
	//  |M|   N  |O|   P   |M|     
	//  |_|______|_|_______|_|
	//  |I|___J__|K|___L___|I|
	//  | |      | |       | |
	//  |E|   F  |G|   H   |E|
	//  |_|______|_|_______|_|
	//  |A|__B___|C|___D___|A|
	//
	//Here is the corresponding filtration:
	//  ______________________
	//  |0|__1___|1|___1___|0|     
	//  | |      | |       | | 
	//  |1|   2  |1|   2   |1|     
	//  |_|______|_|_______|_|
	//  |1|___1__|1|___1___|1|
	//  | |      | |       | |
	//  |1|   2  |1|   2   |1|
	//  |_|______|_|_______|_|
	//  |0|__1___|1|___1___|0|
  
  //cretign cells
  Cell* A = new Cell(0,0.0);
  Cell* B = new Cell(1,1.0);
  Cell* C = new Cell(0,1.0);
  Cell* D = new Cell(1,1.0);
  Cell* E = new Cell(1,1.0);
  Cell* F = new Cell(2,2.0);
  Cell* G = new Cell(1,1.0);
  Cell* H = new Cell(2,2.0);
  Cell* I = new Cell(0,1.0);
  Cell* J = new Cell(1,1.0);
  Cell* K = new Cell(0,1.0);
  Cell* L = new Cell(1,1.0);
  Cell* M = new Cell(1,1.0);
  Cell* N = new Cell(2,2.0);
  Cell* O = new Cell(1,1.0);
  Cell* P = new Cell(2,2.0);
  
  //setting up boundaries and coboundaries of cells:
  //for cell A:
  std::vector< std::pair<Cell*,int> >& coboundary_of_A = A->get_coBoundary();
  coboundary_of_A.push_back( std::pair<Cell*,int>( B,1 ) );
  coboundary_of_A.push_back( std::pair<Cell*,int>( E,1 ) );
  coboundary_of_A.push_back( std::pair<Cell*,int>( M,1 ) );
  coboundary_of_A.push_back( std::pair<Cell*,int>( D,1 ) );
  
  //for cell B:  
  std::vector< std::pair<Cell*,int> >& coboundary_of_B = B->get_coBoundary();  
  coboundary_of_B.push_back( std::pair<Cell*,int>( F,1 ) );
  coboundary_of_B.push_back( std::pair<Cell*,int>( N,1 ) );
  std::vector< std::pair<Cell*,int> >& boundary_of_B = B->get_boundary();
  boundary_of_B.push_back( std::pair<Cell*,int>( A,1 ) );
  boundary_of_B.push_back( std::pair<Cell*,int>( C,1 ) );
  
  //for cell C:
  std::vector< std::pair<Cell*,int> >& coboundary_of_C = C->get_coBoundary();  
  coboundary_of_C.push_back( std::pair<Cell*,int>( B,1 ) );
  coboundary_of_C.push_back( std::pair<Cell*,int>( G,1 ) );
  coboundary_of_C.push_back( std::pair<Cell*,int>( D,1 ) );
  coboundary_of_C.push_back( std::pair<Cell*,int>( O,1 ) );
  
  //for cell D
  std::vector< std::pair<Cell*,int> >& coboundary_of_D = D->get_coBoundary();  
  coboundary_of_D.push_back( std::pair<Cell*,int>( H,1 ) );
  coboundary_of_D.push_back( std::pair<Cell*,int>( P,1 ) );
  std::vector< std::pair<Cell*,int> >& boundary_of_D = D->get_boundary();
  boundary_of_D.push_back( std::pair<Cell*,int>( A,1 ) );
  boundary_of_D.push_back( std::pair<Cell*,int>( C,1 ) );
  
  //for cell E
  std::vector< std::pair<Cell*,int> >& coboundary_of_E = E->get_coBoundary();  
  coboundary_of_E.push_back( std::pair<Cell*,int>( F,1 ) );
  coboundary_of_E.push_back( std::pair<Cell*,int>( H,1 ) );
  std::vector< std::pair<Cell*,int> >& boundary_of_E = E->get_boundary();
  boundary_of_E.push_back( std::pair<Cell*,int>( A,1 ) );
  boundary_of_E.push_back( std::pair<Cell*,int>( I,1 ) );
  
  //for cell F
  std::vector< std::pair<Cell*,int> >& boundary_of_F = F->get_boundary();
  boundary_of_F.push_back( std::pair<Cell*,int>( B,1 ) );
  boundary_of_F.push_back( std::pair<Cell*,int>( E,1 ) );
  boundary_of_F.push_back( std::pair<Cell*,int>( G,1 ) );
  boundary_of_F.push_back( std::pair<Cell*,int>( J,1 ) );  
  
  //for cell G
  std::vector< std::pair<Cell*,int> >& coboundary_of_G = G->get_coBoundary();  
  coboundary_of_G.push_back( std::pair<Cell*,int>( F,1 ) );
  coboundary_of_G.push_back( std::pair<Cell*,int>( H,1 ) );
  std::vector< std::pair<Cell*,int> >& boundary_of_G = G->get_boundary();
  boundary_of_G.push_back( std::pair<Cell*,int>( K,1 ) );
  boundary_of_G.push_back( std::pair<Cell*,int>( C,1 ) );
  
  //for cell H
  std::vector< std::pair<Cell*,int> >& boundary_of_H = H->get_boundary();
  boundary_of_H.push_back( std::pair<Cell*,int>( D,1 ) );
  boundary_of_H.push_back( std::pair<Cell*,int>( E,1 ) );
  boundary_of_H.push_back( std::pair<Cell*,int>( L,1 ) );
  boundary_of_H.push_back( std::pair<Cell*,int>( G,1 ) );
  
  //for cell I:
  std::vector< std::pair<Cell*,int> >& coboundary_of_I = I->get_coBoundary();  
  coboundary_of_I.push_back( std::pair<Cell*,int>( E,1 ) );
  coboundary_of_I.push_back( std::pair<Cell*,int>( J,1 ) );
  coboundary_of_I.push_back( std::pair<Cell*,int>( M,1 ) );
  coboundary_of_I.push_back( std::pair<Cell*,int>( L,1 ) );
  
  //for cell J
  std::vector< std::pair<Cell*,int> >& coboundary_of_J = J->get_coBoundary();  
  coboundary_of_J.push_back( std::pair<Cell*,int>( F,1 ) );
  coboundary_of_J.push_back( std::pair<Cell*,int>( N,1 ) );
  std::vector< std::pair<Cell*,int> >& boundary_of_J = J->get_boundary();
  boundary_of_J.push_back( std::pair<Cell*,int>( I,1 ) );
  boundary_of_J.push_back( std::pair<Cell*,int>( K,1 ) );
  
  //for cell K:
  std::vector< std::pair<Cell*,int> >& coboundary_of_K = K->get_coBoundary();  
  coboundary_of_K.push_back( std::pair<Cell*,int>( G,1 ) );
  coboundary_of_K.push_back( std::pair<Cell*,int>( J,1 ) );
  coboundary_of_K.push_back( std::pair<Cell*,int>( L,1 ) );  
  coboundary_of_K.push_back( std::pair<Cell*,int>( O,1 ) );  
    
  //for cell L
  std::vector< std::pair<Cell*,int> >& coboundary_of_L = L->get_coBoundary();  
  coboundary_of_L.push_back( std::pair<Cell*,int>( H,1 ) );
  coboundary_of_L.push_back( std::pair<Cell*,int>( P,1 ) );
  std::vector< std::pair<Cell*,int> >& boundary_of_L = L->get_boundary();
  boundary_of_L.push_back( std::pair<Cell*,int>( K,1 ) );
  boundary_of_L.push_back( std::pair<Cell*,int>( I,1 ) );  

  //for cell M
  std::vector< std::pair<Cell*,int> >& coboundary_of_M = M->get_coBoundary();  
  coboundary_of_M.push_back( std::pair<Cell*,int>( N,1 ) );
  coboundary_of_M.push_back( std::pair<Cell*,int>( P,1 ) );
  std::vector< std::pair<Cell*,int> >& boundary_of_M = M->get_boundary();
  boundary_of_M.push_back( std::pair<Cell*,int>( A,1 ) );
  boundary_of_M.push_back( std::pair<Cell*,int>( I,1 ) );  
  
  //for cell N
  std::vector< std::pair<Cell*,int> >& boundary_of_N = N->get_boundary();
  boundary_of_N.push_back( std::pair<Cell*,int>( J,1 ) );
  boundary_of_N.push_back( std::pair<Cell*,int>( M,1 ) );
  boundary_of_N.push_back( std::pair<Cell*,int>( O,1 ) );
  boundary_of_N.push_back( std::pair<Cell*,int>( B,1 ) );  
  
  //for cell O
  std::vector< std::pair<Cell*,int> >& coboundary_of_O = O->get_coBoundary();  
  coboundary_of_O.push_back( std::pair<Cell*,int>( N,1 ) );
  coboundary_of_O.push_back( std::pair<Cell*,int>( P,1 ) );
  std::vector< std::pair<Cell*,int> >& boundary_of_O = O->get_boundary();
  boundary_of_O.push_back( std::pair<Cell*,int>( K,1 ) );
  boundary_of_O.push_back( std::pair<Cell*,int>( C,1 ) );    
  
  //for cell P
  std::vector< std::pair<Cell*,int> >& boundary_of_P = P->get_boundary();
  boundary_of_P.push_back( std::pair<Cell*,int>( L,1 ) );
  boundary_of_P.push_back( std::pair<Cell*,int>( O,1 ) );
  boundary_of_P.push_back( std::pair<Cell*,int>( M,1 ) );
  boundary_of_P.push_back( std::pair<Cell*,int>( D,1 ) );   
  
  std::vector< Cell* > vect_of_cells = {A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P};
  
  Gudhi::Hasse_diagram::Hasse_diagram<Cell> hd( vect_of_cells );
  
  return 0;
}

