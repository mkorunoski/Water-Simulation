#include <iostream>
#include <SDL2/SDL.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "geometry.h"
#include "Light.h"
#include "WaterDeformer.h"
#include "CubemapTexure.h"
#include "GameTimer.h"

const int DISPLAY_WIDTH = 900;
const int DISPLAY_HEIGHT = 700;
const std::string WND_NAME = "Water Simulation FPS: ";
GameTimer timer;
int frameCnt = 0;
float timeElapsed = 0.0f;

void CalculateFrameStats(const Display& display);

int main(int argc, char** argv)
{	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, WND_NAME);
	Camera camera(
		glm::vec3(40.0f, 20.0f, -40.0f),
		70.0f,
		(float) DISPLAY_WIDTH / (float) DISPLAY_HEIGHT,
		0.1f, 1000.0f);	
	
	Shader groundShader("./res/shaders/groundShader");
	Shader waterShader("./res/shaders/waterShader");

	Texture hillsTexture("./res/textures/hills.jpg");
	Texture hillsHeightmap("./res/heightmaps/hillsHeightmap.jpg");
	Texture waterHeightmap("./res/heightmaps/waterHeightmap.jpg");
	CubemapTexure skybox("./res/cubemaps/", "jpg");

	glm::vec3 lightPos = glm::vec3(-20.0f, 100.0f, 20.0f);
	glm::vec4 white = glm::vec4(1.0f);

	PointLight pointLight(white, white, white, lightPos, 0.1f, 16);

	Material groundMaterial(
		glm::vec4(1.0f),
		glm::vec4(glm::vec3(1.0f), 1.0f),
		glm::vec4(glm::vec3(.0f), .0f),
		4);
	Material waterMaterial(
		glm::vec4(1.0f),
		glm::vec4(glm::vec3(.8f), .6f),
		glm::vec4(glm::vec3(.8f), .2f),
		256);
	
	MeshData groundData;
	Geometry::generateGrid(100, 100, 200, 200, groundData);
	Mesh ground(groundData.vertices, groundData.vertices.size(), groundData.indices, groundData.indices.size());

	MeshData waterData;
	Geometry::generateGrid(100, 100, 400, 400, waterData);
	Mesh water(waterData.vertices, waterData.vertices.size(), waterData.indices, waterData.indices.size());
	WaterDeformer deformer(waterShader.GetProgram());
	
	Transform transform;
	
	SDL_Event e;
	bool isRunning = true;	
	timer.Reset();
	while (isRunning)
	{
		timer.Tick();

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
			{
				int key = e.key.keysym.sym;				
			}
			else if (e.type == SDL_QUIT)
			{			
				isRunning = false;
			}				
		}

		display.Clear(0.0f, 0.0f, 0.0f, 1.0f);		

		// camera.RotateView(50.0f, 0.1f * timer.TotalTime());

		groundShader.Bind();
		groundShader.Update(transform, camera);
		pointLight.Bind(groundShader.GetProgram());
		groundMaterial.Bind(groundShader.GetProgram());
		hillsHeightmap.Bind(groundShader.GetProgram(), "heightmap", 0);
		hillsTexture.Bind(groundShader.GetProgram(), "tex", 1);
		ground.Draw();
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		waterShader.Bind();
		waterShader.Update(transform, camera);
		pointLight.Bind(waterShader.GetProgram());	
		waterMaterial.Bind(waterShader.GetProgram());
		hillsTexture.Bind(waterShader.GetProgram(), "tex", 0);
		skybox.Bind();
		deformer.Update(0.1f * timer.TotalTime());		
		water.Draw();

		glDisable(GL_BLEND);
		display.SwapBuffers();		
		
		CalculateFrameStats(display);
	}

	return 0;
}

void CalculateFrameStats(const Display& display)
{	
	frameCnt++;

	if ((timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::string o = WND_NAME + std::to_string((int)fps);
		display.SetWindowName(o);

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}