#include <vector>
#include <iostream>

//include header
#include "../src/geom_moments.hpp"

int main(){

	/* Task 1: Construct Tetrahedron and calculate volume */
	
	//You can verify this is a tetrahedron.
	std::vector<std::vector<std::vector<double>>> triangles = {
						{//triangle 1
							{0,0,0// vertex 1
							},
							{1,0,0// vertex 2
							},
							{0,1,0// etc.
							}
						},
						{//triangle 2
							{0,0,0
							},
							{0,1,0
							},
							{0,0,1
							}
						},
						{//triangle 3
							{0,0,0
							},
							{0,0,1
							},
							{1,0,0
							}
						},
						{//triangle 4
							{0,0,1
							},
							{0,1,0
							},
							{1,0,0
							}
						}
					};
	
	//Calculate volume
	double V = MomentSthOrder(triangles,0,0,0,0,false,false);
		//we have set degree to equal i+j+k = 0+0+0 = 0 to indicate that
		//in the general case this is how one calculates an exact result
		//For this example, degree == 0 is the only choice
		
	std::cout << V << std::endl;
	// you will notice that V < 0. This is because we have the opposite orientation
	// of the triangle mesh for the tetrahedron. But this will always be wrong only up 
	// to sign
	
	/* Task 2: import stl file "geometry.stl" and calculate centroid */
	
	double Cx,Cy,Cz;// coordinates of centroid
	
	//Calculate volume of geometry
	V = MomentSthOrder("geometry.stl",0,0,0,0,false,false);
	
	Cx = MomentSthOrder("geometry.stl",1,0,0,1,false,false)/V;
	
	Cy = MomentSthOrder("geometry.stl",0,1,0,1,false,false)/V;
	
	Cz = MomentSthOrder("geometry.stl",0,0,1,1,false,false)/V;

	// notice that each time we set degree = i + j + k = 1, to get an exact result.
	// however in this specific case it does not matter, because the 1st order terms
	// always vanish. Nevertheless be sure to set degree = i + j + k for higher orders
	
	std::cout << "Volume of geometry.stl: " << V << std::endl;
	std::cout << "Centroid of geometry.stl: " << "( "<<Cx<< " , "<<Cy<< " , "<<Cz<< " ) \n";
	
	return 0;
}
