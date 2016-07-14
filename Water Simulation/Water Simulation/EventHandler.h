#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <map>
#include <GL/glew.h>
#include "Camera.h"

const std::string KEY_LEFT	= "SDLK_LEFT";
const std::string KEY_RIGHT = "SDLK_RIGHT";
const std::string KEY_UP	= "SDLK_UP";
const std::string KEY_DOWN	= "SDLK_DOWN";
const std::string MOUSE_MOTION = "SDL_MOUSEMOTION";

class EventHandler
{
private:
	std::map<std::string, bool> map;

public:
	EventHandler()
	{
		map.insert(std::pair<std::string, bool>(KEY_LEFT, false));
		map.insert(std::pair<std::string, bool>(KEY_RIGHT, false));
		map.insert(std::pair<std::string, bool>(KEY_UP, false));
		map.insert(std::pair<std::string, bool>(KEY_DOWN, false));
		map.insert(std::pair<std::string, bool>(MOUSE_MOTION, false));
	}

	void KeyPressed(const std::string& key)	 { map[key] = true; }
	void KeyReleased(const std::string& key) { map[key] = false; }
		
	void Process(Camera* camera, GLfloat deltaTime, GLfloat xpos, GLfloat ypos)
	{
		if (map[KEY_LEFT] == true)	camera->ProcessKeyboard(LEFT, deltaTime);
		if (map[KEY_RIGHT] == true) camera->ProcessKeyboard(RIGHT, deltaTime);
		if (map[KEY_UP] == true)	camera->ProcessKeyboard(FORWARD, deltaTime);
		if (map[KEY_DOWN] == true)	camera->ProcessKeyboard(BACKWARD, deltaTime);		
		
		camera->ProcessMouseMovement(xpos, -ypos);
	}

	~EventHandler() { map.clear(); }
};

#endif