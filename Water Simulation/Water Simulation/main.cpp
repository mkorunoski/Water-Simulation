#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>
#include <SOIL/SOIL.h>

#include "Display.h"
#include "Camera.h"
#include "GameTimer.h"
#include "EventHandler.h"
#include "Renderer.h"

GLuint wndWidth  = 1240;
GLuint wndHeight = 768;

const std::string WND_NAME = "Water Simulation FPS: ";
GameTimer timer;
int frameCnt		= 0;
float timeElapsed	= 0.0f;
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

int main(int argc, char ** argv)
{	
	Display display(WND_NAME, wndWidth, wndHeight);
	Camera camera(glm::vec3(0.0f, 4.0f, -25.0f));

	EventHandler eventHandler;
	
	Renderer renderer(&camera, wndWidth, wndHeight);

	timer.Reset();
	SDL_Event e;	
	while (true)
	{
		display.Clear(0.0f, 0.0f, 0.0f, 1.0f);

		timer.Tick();
		
		int mouseXpos = 0, mouseYpos = 0;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_1: renderer.SetRenderMode(GL_LINE); break;
				case SDLK_2: renderer.SetRenderMode(GL_FILL); break;

				case SDLK_c: renderer.SetWaveGeneratorMode(1); break;
				case SDLK_g: renderer.SetWaveGeneratorMode(0); break;

				case SDLK_a: eventHandler.KeyPressed(KEY_LEFT);  break;
				case SDLK_d: eventHandler.KeyPressed(KEY_RIGHT); break;
				case SDLK_w: eventHandler.KeyPressed(KEY_UP);	 break;
				case SDLK_s: eventHandler.KeyPressed(KEY_DOWN);	 break;
				}
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE: return 0;				
				}
			}
			if (e.type == SDL_KEYUP)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_a: eventHandler.KeyReleased(KEY_LEFT);  break;
				case SDLK_d: eventHandler.KeyReleased(KEY_RIGHT); break;
				case SDLK_w: eventHandler.KeyReleased(KEY_UP);	  break;
				case SDLK_s: eventHandler.KeyReleased(KEY_DOWN);  break;
				}
			}			
			if (e.type == SDL_MOUSEMOTION)
			{
				mouseXpos = e.motion.xrel;
				mouseYpos = e.motion.yrel;
			}			
		}
		eventHandler.Process(&camera, (GLfloat)timer.DeltaTime(), (GLfloat)mouseXpos, (GLfloat)mouseYpos);
		
		renderer.SetDeltaTime((GLfloat)timer.DeltaTime() * 0.1f);
		display.RenderSceneToFrameBuffer();		
		renderer.RenderScene();		
		display.DisplayFrameBufferContent();	

		display.SwapBuffers();

		CalculateFrameStats(display);
	}

	return 0;
}
