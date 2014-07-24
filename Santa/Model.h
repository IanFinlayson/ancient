// Model.h
// Represents a milkshape (.ms3d) model
// Ian Finlayson

#ifndef MODEL_H
#define MODEL_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Model
{
private:
	// A group of triangles
	struct Mesh
	{
		int material_index;
		int num_triangles;
		int* triangle_indices;
	};

	// Encompasses a material of a mesh...
	struct Material
	{
		float ambient[4], diffuse[4], specular[4], emissive[4];
		float shininess;
		GLuint texture;
		char* texture_filename;
	};

	// 3 vertices (plus textures)
	struct Triangle
	{
		float vertex_normals[3][3];
		float s[3], t[3];
		int vertex_indices[3];
	};

	// A single vertex...
	struct Vertex
	{
		char bone_id;	// for skeletal animation (not used but must be loaded)
		float location[3];
	};

	// Model data...
	int num_meshes;
	Mesh* meshes;

	int num_materials;
	Material* materials;

	int num_triangles;
	Triangle* triangles;

	int num_vertices;
	Vertex* vertices;

public:
	Model( const char* filename );
	~Model( );

	void Draw( );
};

#endif