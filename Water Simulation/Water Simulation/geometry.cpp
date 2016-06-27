#include "geometry.h"

void Geometry::generateGrid(int width, int depth, unsigned int m, unsigned int n, MeshData& meshData)
{
	unsigned int vertexCount = m * n;
	unsigned int faceCount = 2 * (m - 1) * (n - 1);
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;
	float dx = (float) width / (n - 1);
	float dz = (float) depth / (m - 1);
	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	meshData.vertices.resize(vertexCount);
	for (unsigned int i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dz;
		for (unsigned int j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;
			meshData.vertices[i * n + j].SetPos(glm::vec3(x, 0.0f, z));
			meshData.vertices[i * n + j].SetNormal(glm::vec3(0.0f, 1.0f, 0.0f));
			meshData.vertices[i * n + j].SetTexCoord(glm::vec2(j * du, i * dv));
		}
	}

	meshData.indices.resize(faceCount * 3);	
	unsigned int k = 0;
	for (unsigned int i = 0; i < m - 1; ++i)
	{
		for (unsigned int j = 0; j < n - 1; ++j)
		{
			meshData.indices[k] = i*n + j;
			meshData.indices[k + 1] = i*n + j + 1;
			meshData.indices[k + 2] = (i + 1)*n + j;
			meshData.indices[k + 3] = (i + 1)*n + j;
			meshData.indices[k + 4] = i*n + j + 1;
			meshData.indices[k + 5] = (i + 1)*n + j + 1;
			k += 6;
		}
	}			
}