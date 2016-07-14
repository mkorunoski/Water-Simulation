#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "Shader.h"

class Display
{
private:
	SDL_Window* window;
	SDL_GLContext glContext;
	GLuint width;
	GLuint height;

public:
	Display(const std::string& wndName, GLuint width, GLuint height)
	{
		this->width = width;
		this->height = height;

		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_SetRelativeMouseMode(SDL_TRUE);

		window = SDL_CreateWindow(wndName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
		glContext = SDL_GL_CreateContext(window);
		
		glewInit();

		glViewport(0, 0, width, height);
		glEnable(GL_DEPTH_TEST);	
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}

	void Clear(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		glClearColor(r, g, b , a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void SwapBuffers()
	{
		SDL_GL_SwapWindow(window);
	}

	void SetWindowName(std::string& wndName) const
	{
		SDL_SetWindowTitle(window, wndName.c_str());
	}
	
	~Display()
	{
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
};

#endif