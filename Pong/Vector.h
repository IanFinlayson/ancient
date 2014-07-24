// Vector.h
// Ian Finlayson
// Defines a generic vector class

#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

// Allows user to use these instead of numbers for indices
// ie myvector[X]
const int X = 0;
const int Y = 1;
const int Z = 2;

// It's templatize so you can use different types and any number of dimensions (In case the next assignment is 3d pong)
template<typename type, int dim>
class Vector
{
public:
	// The actual data
	type data[dim];

	// Default constructor
	Vector( )
	{
		for( int i=0; i<dim; i++ )
		{
			data[i] = 0;
		}
	}

	// Copy Constructor
	Vector( Vector<type,dim> & copy )
	{
		for( int i=0; i<dim; i++ )
		{
			data[i] = copy.data[i];
		}
	}

	// Constructor for 2D vector
	Vector( type x, type y )
	{
		if( dim == 2 )	// protect against overwritng past array bounds
		{
			data[0] = x;
			data[1] = y;
		}
	}

	// Constructor for 3D vector any other dimension and you must set them manually!
	Vector( type x, type y, type z )
	{
		if( dim == 3 )	// protect against overwritng past array bounds
		{
			data[0] = x;
			data[1] = y;
			data[2] = z;
		}
	} 

	// Returns the magnitude of the vector
	type Magnitude( )
	{
		type sum = 0;

		for( int i=0; i<dim; i++ )	//For each dimension
		{
			sum += data[i]*data[i];
		}

		return sqrt( sum );	// Square root of sum
	}

	
	// Normalize the vector
	void Normalize( )
	{
		type mag = Magnitude( );

		for( int i=0; i<dim; i++ )
		{
			data[i] = data[i] / mag;
		}
	}

	// Overloaded vector + vector
	Vector<type,dim> operator+( Vector<type,dim> & rhs )
	{
		Vector<type,dim> temp;

		for( int i=0; i<dim; i++ )
		{
			temp.data[i] = data[i] + rhs.data[i];
		}

		return temp;
	}

	// Overloaded vector - vector
	Vector<type,dim> operator-( Vector<type,dim> & rhs )
	{
		Vector<type,dim> temp;

		for( int i=0; i<dim; i++ )
		{
			temp.data[i] = data[i] - rhs.data[i];
		}

		return temp;
	}

	// Overloaded -vector (unary) operator
	Vector<type,dim> operator-( )
	{
		Vector<type,dim> temp;

		for( int i=0; i<dim; i++ )
		{
			temp.data[i] = -data[i];
		}
		
		return temp;
	}

	// Overloaded vector * scalar
	Vector<type,dim> operator*( type rhs )
	{
		Vector<type,dim> temp;

		for( int i=0; i<dim; i++ )
		{
			temp.data[i] = data[i]*rhs;
		}

		return temp;
	}

	// vector/scalar operator
	Vector<type,dim> operator/( type rhs )
	{
		Vector<type,dim> temp;

		for( int i=0; i<dim; i++ )
		{
			temp.data[i] = data[i]/rhs;
		}

		return temp;
	}

	// = operator
	Vector<type,dim> operator=( Vector<type,dim> & rhs )
	{
		// Copy the data
		for( int i=0; i<dim; i++ )
		{
			data[i] = rhs.data[i];
		}

		return *this;
	}

	// Overloaded [] operator allows for direct indexing
	// works as lvalue and rvalue
	type & operator[]( int index )
	{
		return data[index];
	}

	// Dot Product is the % operator
	type operator%( Vector<type,dim> & rhs )
	{
		type value = 0;

		for( int i=0; i<dim; i++ )
		{
			value += ( data[i] * rhs.data[i] );
		}

		return value;
	}

	// Cross product is the ^ operator
	Vector<type,dim> operator^( Vector<type,dim> & rhs )
	{
		Vector<type,dim> temp;

		if( dim == 3 )	// Only defined for 3D vectors
		{
			temp.data[0] = data[1]*rhs.data[2] - data[2]*rhs.data[1];
			temp.data[1] = data[2]*rhs.data[0] - data[0]*rhs.data[2];
			temp.data[2] = data[0]*rhs.data[1] - data[1]*rhs.data[0];
		}
	}
};

#endif
