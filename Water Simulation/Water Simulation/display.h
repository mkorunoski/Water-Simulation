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
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_MULTISAMPLE);

		InitOffscreenRenderTarget();
	}

	void Clear(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		glClearColor(r, g, b, a);
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

private:
	GLuint framebuffer;
	GLuint framebufferColorTex;
	GLuint framebufferDepthStencilTex;

public:
	void InitOffscreenRenderTarget()
	{
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glGenTextures(1, &framebufferColorTex);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferColorTex);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferColorTex, 0);

		GLuint frameBufferDepthStencilRBO;
		glGenRenderbuffers(1, &frameBufferDepthStencilRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, frameBufferDepthStencilRBO);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBufferDepthStencilRBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderSceneToFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		Clear(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void DisplayFrameBufferContent()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
};

#endif