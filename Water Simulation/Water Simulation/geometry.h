#ifndef GEOMETRY_INCLUDED_H
#define GEOMETRY_INCLUDED_H

#include <vector>
#include "mesh.h"

typedef struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
} MeshData;

class Geometry
{	
public:
	static void generateGrid(int width, int depth, unsigned int m, unsigned int n, MeshData& meshData);
};

#endif