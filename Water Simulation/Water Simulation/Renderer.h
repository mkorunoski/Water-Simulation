#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "Geometry.h"
#include "Transformation.h"
#include "WaveGenerators.h"
#include "Texture.h"
#include "CubemapTexture.h"
#include "Material.h"
#include "Light.h"

enum UniformLoc
{
	MODEL = 10,	
	INVERSE_TRANSPOSE,
	EYE_POSITION = 20,
	SKYBOX_TEX = 30,
};

class Renderer
{
private:
	GLuint wndWidth;
	GLuint wndHeight;
	Camera* camera;	

	// Shaders
	static const GLuint NUM_SHADERS = 4;
	Shader defaultShader;
	Shader waveGeneratorGPUShader;
	Shader waveGeneratorCPUShader;
	Shader skyboxShader;
	
	// Lights
	DirectionalLight directionalLight;	
	
	// Meshes
	std::vector<Vertex> waterVertices;
	std::vector<GLuint> waterIndices;
	Mesh skyboxMesh;
	Mesh waterMesh;
	Mesh floorMesh;

	Material skyboxMaterial;	
	Material waterMaterial;
	Material floorMaterial;

	Transformation skyboxTransformation;
	Transformation waterTransformation;		
	Transformation floorTransformation;

	WaveGeneratorGPU waveGeneratorGPU;
	WaveGeneratorCPU waveGeneratorCPU;

	CubemapTexture skyboxTex;
	Texture checkeredTexture;

	GLuint UBO;
	
public:
	// For animation	
	GLfloat dt	= 0.0f;	

private:
	void CompileShaders()
	{
		defaultShader			= Shader("./res/shaders/default.vs", "./res/shaders/default.fs", "default");
		waveGeneratorGPUShader	= Shader("./res/shaders/waveGeneratorGPU.vs", "./res/shaders/waveGenerator.fs", "waveGeneratorGPU");
		waveGeneratorCPUShader	= Shader("./res/shaders/waveGeneratorCPU.vs", "./res/shaders/waveGenerator.fs", "waveGeneratorCPU");
		skyboxShader			= Shader("./res/shaders/skybox.vs", "./res/shaders/skybox.fs", "skybox");
	}

	void SetupLights()
	{
		directionalLight = DirectionalLight(
			glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f),
			glm::vec3(-50.0f, 20.0f, 0.0f));
	}

	void ActivateDirectionalLights(Shader& shader)
	{
		directionalLight.SetUniforms(shader.GetProgram());
		shader.Use();
			directionalLight.Use();
		shader.Unuse();
	}

	void AcivateLights(Shader& shader)
	{
		ActivateDirectionalLights(shader);
	}
	
	void LoadMeshes()
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		Geometry::GenerateCube(vertices);
		skyboxMesh = Mesh(vertices);
		Geometry::GeneratePlane(50, 50, 200, 200, waterVertices, waterIndices);
		waterMesh  = Mesh(waterVertices, waterIndices);
		Geometry::GeneratePlane(50, 50, 2, 2, vertices, indices);
		floorMesh  = Mesh(vertices, indices);

		skyboxTransformation = Transformation();
		waterTransformation	 = Transformation();
		floorTransformation  = Transformation();

		waveGeneratorGPU = WaveGeneratorGPU(waveGeneratorGPUShader.GetProgram());
		waveGeneratorCPU = WaveGeneratorCPU();
		
		skyboxMaterial = Material(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 1);
		waterMaterial  = Material(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 256);
		floorMaterial  = Material(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 128);

		skyboxTex = CubemapTexture("./res/textures/cubemaps/", "png");
		checkeredTexture = Texture("./res/textures/checkered.jpg");
	}

	void SetupUniformBufferObjects()
	{
		const GLuint BINDING_POINT0 = 0;
		GLuint UBIndices[NUM_SHADERS];
		const GLchar* UB_NAME = "ViewProjection";
		const GLuint PROGRAMS[NUM_SHADERS] = {  // add the reference to the new shader's program here
			defaultShader.GetProgram(),
			waveGeneratorGPUShader.GetProgram(),
			waveGeneratorCPUShader.GetProgram(),
			skyboxShader.GetProgram()
		};
		GLuint i = 0;

		glGenBuffers(1, &UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT0, UBO);
		
		for (i = 0; i < NUM_SHADERS; ++i)
		{
			UBIndices[i] = glGetUniformBlockIndex(PROGRAMS[i], UB_NAME);
			glUniformBlockBinding(PROGRAMS[i], UBIndices[i], BINDING_POINT0);
		}

		glm::mat4 projection = glm::perspective(70.0f, (GLfloat)wndWidth / (GLfloat)wndHeight, 0.1f, 1000.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

public:
	void SetDeltaTime(GLfloat dt) { this->dt += dt; }

	Renderer(Camera* camera, GLuint wndWidth, GLuint wndHeight)
	{		
		this->camera	= camera;
		this->wndWidth	= wndWidth;
		this->wndHeight = wndHeight;

		CompileShaders();
		SetupLights();
		LoadMeshes();
		SetupUniformBufferObjects();
	}
	
	void RenderScene()
	{	
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->GetViewMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Skybox		
		skyboxTransformation.Scale(glm::vec3(500.0f));
		skyboxTransformation.Translate(camera->GetEyePos());
		skyboxShader.Use();
			glUniformMatrix4fv(UniformLoc::MODEL, 1, false, glm::value_ptr(skyboxTransformation.GetModel()));
			glFrontFace(GL_CW);
			skyboxTex.Use();
			skyboxMesh.DrawArrays();
			skyboxTex.Unuse();
			glFrontFace(GL_CCW);
		skyboxShader.Unuse();

		// Lights
		AcivateLights(defaultShader);

		// Floor
		floorTransformation.Translate(glm::vec3(0.0f, -2.0f, 0.0f));
		defaultShader.Use();
			glUniform3fv(UniformLoc::EYE_POSITION, 1, glm::value_ptr(camera->GetEyePos()));
			glUniformMatrix4fv(UniformLoc::MODEL, 1, false, glm::value_ptr(floorTransformation.GetModel()));
			glUniformMatrix4fv(UniformLoc::INVERSE_TRANSPOSE, 1, false, glm::value_ptr(floorTransformation.GetInverseTranspose()));			
			floorMaterial.Use(defaultShader.GetProgram());
			checkeredTexture.Use(defaultShader.GetProgram(), "maps.diffuse", 0);
			floorMesh.DrawElements();
			checkeredTexture.Unuse();
		defaultShader.Unuse();

		// Water
		RenderWaterGPU();
	}

private:
	void RenderWaterGPU()
	{
		// Lights
		AcivateLights(waveGeneratorGPUShader);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		waterTransformation.Translate(glm::vec3(0.0f, 2.0f, 0.0f));
		waveGeneratorGPUShader.Use();
			glUniform3fv(UniformLoc::EYE_POSITION, 1, glm::value_ptr(camera->GetEyePos()));
			glUniformMatrix4fv(UniformLoc::MODEL, 1, false, glm::value_ptr(waterTransformation.GetModel()));
			glUniformMatrix4fv(UniformLoc::INVERSE_TRANSPOSE, 1, false, glm::value_ptr(waterTransformation.GetInverseTranspose()));			
			waveGeneratorGPU.Update(dt);
			waterMaterial.Use(waveGeneratorGPUShader.GetProgram());
			skyboxTex.Use();
			waterMesh.DrawElements();
		waveGeneratorGPUShader.Unuse();

		glDisable(GL_BLEND);
	}

	void RenderWaterCPU()
	{
		// Lights
		AcivateLights(waveGeneratorCPUShader);

		waveGeneratorCPU.Update(dt, waterVertices);
		waterMesh.UpdateBufferData(waterVertices);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		waterTransformation.Translate(glm::vec3(0.0f, 2.0f, 0.0f));
		waveGeneratorCPUShader.Use();
			glUniform3fv(UniformLoc::EYE_POSITION, 1, glm::value_ptr(camera->GetEyePos()));
			glUniformMatrix4fv(UniformLoc::MODEL, 1, false, glm::value_ptr(waterTransformation.GetModel()));
			glUniformMatrix4fv(UniformLoc::INVERSE_TRANSPOSE, 1, false, glm::value_ptr(waterTransformation.GetInverseTranspose()));
			waterMaterial.Use(waveGeneratorCPUShader.GetProgram());
			skyboxTex.Use();
			waterMesh.DrawElements();
		waveGeneratorCPUShader.Unuse();

		glDisable(GL_BLEND);
	}

public:		
	~Renderer() { }
};

#endif