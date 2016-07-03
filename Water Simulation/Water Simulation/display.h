#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H

#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>

class Display
{
public:
	Display(int width, int height, const std::string& title);

	void Clear(float r, float g, float b, float a);
	void SwapBuffers();
	void SetWindowName(const std::string& name) const;

	void RenderSceneToTexture();
	void RenderSceneDepthToTexture();
	void RenderOnscreen();
	//GLuint GetSceneTexture() { return m_sceneTexture; }
	GLuint GetSceneDepthTexture() { return m_sceneDepthTexture; }

	virtual ~Display();
protected:
private:
	void operator=(const Display& display) {}
	Display(const Display& display) {}

	void GenTextures();

	SDL_Window* m_window;
	int m_width;
	int m_height;
	SDL_GLContext m_glContext;

	GLuint m_fbo;
	GLuint m_dbo;
	//GLuint m_sceneTexture;
	GLuint m_sceneDepthTexture;
};

#endif
