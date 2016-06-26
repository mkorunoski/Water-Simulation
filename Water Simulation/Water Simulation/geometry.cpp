#include "geometry.h"

float GetHeight(float x, float z)
{
	return 0.3f * (z * sin(0.1f * x) + x * cos(0.1f * z));;
}

glm::vec3 GetNormal(float x, float z)
{
	// n = (-df/dx, 1, -df/dz)
	glm::vec3 n(
		-0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z),
		 1.0f,
		-0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z));
	glm::vec3 unitNormal = glm::normalize(n);	
	return unitNormal;
}

void Geometry::generateGrid(bool flat, int width, int depth, unsigned int m, unsigned int n, MeshData& meshData)
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
			if (flat)
			{
				meshData.vertices[i * n + j].SetPos(glm::vec3(x, 0.0f, z));
				meshData.vertices[i * n + j].SetNormal(glm::vec3(0.0f, 1.0f, 0.0f));
				meshData.vertices[i * n + j].SetTexCoord(glm::vec2(j * du, i * dv));
			}
			else
			{
				meshData.vertices[i * n + j].SetPos(glm::vec3(x, GetHeight(x, z), z));
				meshData.vertices[i * n + j].SetNormal(GetNormal(x, z));
				meshData.vertices[i * n + j].SetTexCoord(glm::vec2(j * du, i * dv));
			}			
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