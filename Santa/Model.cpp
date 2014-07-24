// Model.cpp
// Ian Finlayson

#include <fstream>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "BmpTexture.h"
#include "Model.h"

// We need to align all of these structures
// on one byte boundaries so it does not get
// messed up by the OS word aligning our data
#pragma pack( push, packing )
#pragma pack( 1 )	// One byte...
#define PACK_STRUCT

// Stuff specific to the .ms3d format...
typedef unsigned char byte;
typedef unsigned short word;

// File header
struct MS3DHeader
{
	char ID[10];
	int version;
} PACK_STRUCT;

// Vertex information
struct MS3DVertex
{
	byte flags;
	float vertex[3];
	char boneID;
	byte ref_count;
} PACK_STRUCT;

// Triangle information
struct MS3DTriangle
{
	word flags;
	word vertex_indices[3];
	float vertex_normals[3][3];
	float s[3], t[3];
	byte smoothing_group;
	byte group_index;
} PACK_STRUCT;

// Material information
struct MS3DMaterial
{
    char name[32];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emissive[4];
    float shininess;
    float transparency;
    byte mode;
    char texture[128];
    char alphamap[128];
} PACK_STRUCT;

// Joint information
struct MS3DJoint
{
	byte flags;
	char name[32];
	char parent_name[32];
	float rotation[3];
	float translation[3];
	word num_rotation_keyframes;
	word num_translation_keyframes;
} PACK_STRUCT;

// Keyframe data
struct MS3DKeyframe
{
	float time;
	float parameter[3];
} PACK_STRUCT;

// Undo the byte alignment stuff...
#pragma pack( pop, packing )
#undef PACK_STRUCT

// Load the model...
Model::Model( const char* filename )
{
	std::fstream input_file( filename, std::ios::in | std::ios::binary );

	// Find the size of the file...
	input_file.seekg( 0, std::ios::end );
	long file_size = input_file.tellg( );
	input_file.seekg( 0, std::ios::beg );

	// Read the file into memory and close it...
	byte* buffer = new byte[file_size];
	input_file.read( reinterpret_cast<char*>( buffer ), file_size );
	input_file.close( );

	// Read the header
	const byte* file_ptr = buffer;
	MS3DHeader* header = ( MS3DHeader* )file_ptr;
	file_ptr += sizeof MS3DHeader;

	// Get the number of vertices and allocate space for them...
	int nvertices = *(word*)file_ptr; 
	num_vertices = nvertices;
	vertices = new Vertex[nvertices];
	file_ptr += sizeof word;

	// Read the vertices from the buffer...
	for( int i=0; i<nvertices; i++ )
	{
		MS3DVertex *pVertex = ( MS3DVertex* )file_ptr;
		vertices[i].bone_id = pVertex->boneID;
		memcpy( vertices[i].location, pVertex->vertex, sizeof( float )*3 );
		file_ptr += sizeof MS3DVertex;
	}

	// Find the number of triangles and allocate space...
	int nTriangles = *( word* )file_ptr;
	num_triangles = nTriangles;
	triangles = new Triangle[nTriangles];
	file_ptr += sizeof word;

	// Read the triangles...
	for( int i=0; i<nTriangles; i++ )
	{
		MS3DTriangle *pTriangle = ( MS3DTriangle* )file_ptr;
		int vertex_indices[3] = { pTriangle->vertex_indices[0], pTriangle->vertex_indices[1], pTriangle->vertex_indices[2] };
		float t[3] = { 1.0f-pTriangle->t[0], 1.0f-pTriangle->t[1], 1.0f-pTriangle->t[2] };
		memcpy( triangles[i].vertex_normals, pTriangle->vertex_normals, sizeof( float )*3*3 );
		memcpy( triangles[i].s, pTriangle->s, sizeof( float )*3 );
		memcpy( triangles[i].t, t, sizeof( float )*3 );
		memcpy( triangles[i].vertex_indices, vertex_indices, sizeof( int )*3 );
		file_ptr += sizeof MS3DTriangle;
	}

	// Find the number of meshes
	int nGroups = *( word* )file_ptr;
	num_meshes = nGroups;
	meshes = new Mesh[nGroups];
	file_ptr += sizeof( word );

	// And read them...
	for( int i=0; i<nGroups; i++ )
	{
		file_ptr += sizeof byte;	// flags
		file_ptr += 32;				// name

		word nTriangles = *( word* )file_ptr;
		file_ptr += sizeof word;
		int* pTriangleIndices = new int[nTriangles];
		for( int j=0; j<nTriangles; j++ )
		{
			pTriangleIndices[j] = *( word* )file_ptr;
			file_ptr += sizeof word;
		}

		char materialIndex = *( char* )file_ptr;
		file_ptr += sizeof( char );
	
		meshes[i].material_index = materialIndex;
		meshes[i].num_triangles = nTriangles;
		meshes[i].triangle_indices = pTriangleIndices;
	}

	// Read material information
	int nMaterials = *( word* )file_ptr;
	num_materials = nMaterials;
	materials = new Material[nMaterials];
	file_ptr += sizeof word;
	for( i=0; i<nMaterials; i++ )
	{
		MS3DMaterial *pMaterial = ( MS3DMaterial* )file_ptr;
		memcpy( materials[i].ambient, pMaterial->ambient, sizeof( float )*4 );
		memcpy( materials[i].diffuse, pMaterial->diffuse, sizeof( float )*4 );
		memcpy( materials[i].specular, pMaterial->specular, sizeof( float )*4 );
		memcpy( materials[i].emissive, pMaterial->emissive, sizeof( float )*4 );
		materials[i].shininess = pMaterial->shininess;
		materials[i].texture_filename =  new char[strlen( pMaterial->texture )+1];
		strcpy( materials[i].texture_filename, pMaterial->texture );
		file_ptr += sizeof MS3DMaterial;
	}

	// Reclaim memory...
	delete [] buffer;
}

Model::~Model( )
{
	// Deallocate all the memory...
	for( int i=0; i<num_meshes; i++ )
	{
		delete [] meshes[i].triangle_indices;
	}
	for( int i=0; i<num_materials; i++ )
	{
		delete [] materials[i].texture_filename;
	}

	delete [] meshes;
	delete [] materials;
	delete [] triangles;
	delete [] vertices;
}


void Model::Draw( )
{
	// Draw by groups...
	for( int i=0; i<num_meshes; i++ )
	{
		// Draw the triangles in this mesh...
		glBegin( GL_TRIANGLES );
			for( int j=0; j<meshes[i].num_triangles; j++ )
			{
				int triangle_index = meshes[i].triangle_indices[j];
				const Triangle* tri = &triangles[triangle_index];

				for( int k=0; k<3; k++ )
				{
					int index = tri->vertex_indices[k];

					// Draw textues and vertices...
					glTexCoord2f( tri->s[k], tri->t[k] );
					glVertex3fv( vertices[index].location );
				}
			}
		glEnd();
	}
}

