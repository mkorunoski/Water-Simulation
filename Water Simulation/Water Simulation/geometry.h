#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>

#include "Mesh.h"

// Constant normals
const glm::vec3 POS_X = glm::vec3(+1.0f, 0.0f, 0.0f);
const glm::vec3 NEG_X = glm::vec3(-1.0f, 0.0f, 0.0f);
const glm::vec3 POS_Y = glm::vec3(0.0f, +1.0f, 0.0f);
const glm::vec3 NEG_Y = glm::vec3(0.0f, -1.0f, 0.0f);
const glm::vec3 POS_Z = glm::vec3(0.0f, 0.0f, +1.0f);
const glm::vec3 NEG_Z = glm::vec3(0.0f, 0.0f, -1.0f);
// Constant texture coordinates
const glm::vec2 TEX_COORDS_ARR[] = { glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f) };

class Geometry
{
public:
	static void GeneratePlane(GLint width, GLint depth, GLuint m, GLuint n, std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
	{
		vertices.clear();
		indices.clear();

		GLuint vertexCount = m * n;
		GLuint faceCount  = 2 * (m - 1) * (n - 1);
		GLfloat halfWidth = 0.5f * width;
		GLfloat halfDepth = 0.5f * depth;
		GLfloat dx = (GLfloat)width / (n - 1);
		GLfloat dz = (GLfloat)depth / (m - 1);
		GLfloat du = 1.0f / (n - 1);
		GLfloat dv = 1.0f / (m - 1);

		vertices.resize(vertexCount);
		for (GLuint i = 0; i < m; ++i)
		{
			GLfloat z = halfDepth - i * dz;
			for (GLuint j = 0; j < n; ++j)
			{
				GLfloat x = -halfWidth + j * dx;
				vertices[i * n + j].position	= glm::vec3(x, 0.0f, z);
				vertices[i * n + j].texCoords	= glm::vec2(j * du, i * dv);
				vertices[i * n + j].normal		= POS_Y;
				vertices[i * n + j].tangent		= POS_X;
			}
		}

		indices.resize(faceCount * 3);
		GLuint k = 0;
		for (GLuint i = 0; i < m - 1; ++i)
		{
			for (GLuint j = 0; j < n - 1; ++j)
			{
				indices[k]	  = i*n + j;
				indices[k + 1] = i*n + j + 1;
				indices[k + 2] = (i + 1)*n + j;
				indices[k + 3] = (i + 1)*n + j;
				indices[k + 4] = i*n + j + 1;
				indices[k + 5] = (i + 1)*n + j + 1;
				k += 6;
			}
		}		
	}

	static void GenerateCube(std::vector<Vertex>& vertices)
	{
		vertices.clear();		

		vertices.resize(36);
		
		glm::vec3 a(-0.5f, -0.5f, -0.5f);
		glm::vec3 b(+0.5f, -0.5f, -0.5f);
		glm::vec3 c(+0.5f, -0.5f, +0.5f);
		glm::vec3 d(-0.5f, -0.5f, +0.5f);
		
		glm::vec3 e(-0.5f, +0.5f, +0.5f);
		glm::vec3 f(+0.5f, +0.5f, +0.5f);
		glm::vec3 g(+0.5f, +0.5f, -0.5f);		
		glm::vec3 h(-0.5f, +0.5f, -0.5f);

		glm::vec3 normal;
		glm::vec3 tangent;
		GLuint i = 0;
		//front
		normal	= POS_Z;
		tangent = POS_X;
		vertices[i++] = Vertex(d, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(c, normal, TEX_COORDS_ARR[1], tangent);
		vertices[i++] = Vertex(f, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(d, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(f, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(e, normal, TEX_COORDS_ARR[3], tangent);

		//back
		normal	= NEG_Z;
		tangent = NEG_X;
		vertices[i++] = Vertex(b, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(a, normal, TEX_COORDS_ARR[1], tangent);
		vertices[i++] = Vertex(h, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(b, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(h, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(g, normal, TEX_COORDS_ARR[3], tangent);

		//left
		normal	= NEG_X;
		tangent = POS_Z;
		vertices[i++] = Vertex(a, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(d, normal, TEX_COORDS_ARR[1], tangent);
		vertices[i++] = Vertex(e, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(a, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(e, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(h, normal, TEX_COORDS_ARR[3], tangent);

		//right
		normal	= POS_X;
		tangent = NEG_Z;
		vertices[i++] = Vertex(c, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(b, normal, TEX_COORDS_ARR[1], tangent);
		vertices[i++] = Vertex(g, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(c, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(g, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(f, normal, TEX_COORDS_ARR[3], tangent);

		//top
		normal	= POS_Y;
		tangent = POS_X;
		vertices[i++] = Vertex(e, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(f, normal, TEX_COORDS_ARR[1], tangent);
		vertices[i++] = Vertex(g, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(e, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(g, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(h, normal, TEX_COORDS_ARR[3], tangent);

		//bottom
		normal	= NEG_Y;
		tangent = POS_X;
		vertices[i++] = Vertex(a, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(b, normal, TEX_COORDS_ARR[1], tangent);
		vertices[i++] = Vertex(c, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(a, normal, TEX_COORDS_ARR[0], tangent);
		vertices[i++] = Vertex(c, normal, TEX_COORDS_ARR[2], tangent);
		vertices[i++] = Vertex(d, normal, TEX_COORDS_ARR[3], tangent);
	}
		
	~Geometry() { }
};

#endif