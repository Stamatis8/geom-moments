# geom_moments

- C++ library to efficiently compute Nth order geometric moments of triangulated meshes
- header-only library 
- STL file import supported

# Dependancies

- Standard Library

- This [https://github.com/sreiter/stl_reader](https://github.com/sreiter/stl_reader) repository, which is used to import STL files. It is already included in the `src` file of this project.

# Usage

Since this is a header-only library, one can simply move the files in `src` to a `path` which the compiler has access to. Then, in the file that the user wants to use this library, they can simply include the `geom_moments.hpp` header by `#include "path/geom_moments.hpp"`. (ie one can copy-paste the contents of `src` to their project folder and then simply `#include geom_moments.hpp`).

# Overview

This library is centered around the `MomentSthOrder()` function which computes the  $(i+j+k) = s^{th}$ order moment for the input geometry. The input geometry is a trianglulated mesh, which as of now can be in two formats, either as a .STL file or as a vector of depth three (see below). We will now demonstate a simple example.

## Simple Example

The goal of this example is two-fold. First, show how to construct the tetrahedron with vertices $(1,0,0)$, $(0,1,0)$, $(0,0,1)$, $(0,0,0)$ in `std::vector` format and then how to calculate its volume. The second goal is to demonstrate how to import STL files and calculate the geometry centroid.

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
		
		std::cout << "( "<<Cx<< " , "<<Cy<< " , "<<Cz<< " ) \n";
		
		return 0;
	}
	

# Documentation

Unfortunately, no organized documentation document exists yet. However each function is documented thoroughly at its definition. However, a brief description of the two central functions `MomentSthOrder()` and `SSV()` follows.

## MomentSthOrder()

	// Accepts geometry as STL file
	double MomentSthOrder(std::string filename,int i, int j, int k, int degree,
			bool is_translation_invariant, bool is_scaling_invariant);
			
	// Accepts geometry as std::vector Object
	double MomentSthOrder(std::vector<std::vector<std::vector<double>>> triangles,int i, int j, int k, int degree,
			bool is_translation_invariant, bool is_scaling_invariant);
	
The `MomentSthOrder()` function calculates the `i,j,k` moment of order `i+j+k` of the input geometry, using the algorithm described in [1]. This geometric moment is defined as $$\cal{G}_{ijk} = \int_V x^iy^jz^k dV$$
This algorithm allows for approximation of $\cal{G}_{ijk}$, by first expressing it explicitly as a power series in some constant $\lamda$ and then truncating the series at the `degree + 1` term. It should be noted that if `degree = i+j+k`, then the result is exact
Finally, the `is_translation_invariant` and `is_scaling_invariant` parameter specify whether to calculate the standard moment (both false), the translation invariant moment and or the scaling invariant moment. Details can be found in [2]

## SSV()
	
	// Accepts geometry as STL file
	std::vector<double> SSV(std::string filename, int order);
	
	// Accepts geometry as std::vector Object
	std::vector<double> SSV(std::vector<std::vector<std::vector<double>>> triangles, int order);

The `SSV()` function stands for *Shape Signature Vector* and is defined in [2]. It is a vector containing all moments of the input geometry up to order s. These moments are the translation and scaling invariant forms of the geometric moment described above. There are a couple of conventions about SSV() that should be noted. Firstly, the $0^{th}$ order moment which is the volume, if scaling invariant, is by definition always equal to 1. So for the volume, the standard moment is used. Further, all $1^{st}$ order moments are by definition equal to zero if the moment is translation invariant and therefore are ignored. This is because the first order moments divided by the volume equal the centroid of the geometry, which in turn if translation invariant, is centered.

## Geometry as a std::vector Object

Both `SSV()` and `MomentSthOrder()` can accept the geometry as a `std::vector<std::vector<std::vector<double>>> triangles` object. The way this container is formatted is the follows:

- `triagles[i]` is the $i^{th}$ triangle
- `triangles[i][j]` is the $j^{th}$ vertex of the $i^{th}$ triangle
- `triangles[i][j][k]` is the $k^{th}$ coodinate of the $j^{th}$ vertex of the $i^{th}$ triangle


# References

1. Efficient 3D Geometric and Zernike moments computation from unstructured surface meshes J. M. Pozo, M. C. Villa-Uriol, A. F. Frangi, Senior Member, IEEE

2. Geometric Moment-Dependent Global Sensitivity Analysis without Simulation Data: Application to Ship Hull Form Optimisation, Shahroz Khan, Panagiotis Kaklis, Andrea Serani, Matteo Diez
