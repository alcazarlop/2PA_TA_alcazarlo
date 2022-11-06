
#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__ 1

#include <SDL.h>
#include <stdio.h>
#include "window_controller.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_controller.h"
#include "sql_controller.h"

class GameController {

public:
	GameController();
	GameController(const GameController& other);
	~GameController();

	int init();
	int loop();
	void quit();

	void input(SDL_Event* e);
	void update();
	void render();

private:
	int isRunning_;
	bool openImgui_;
	SQLController db_;
	WindowController window_;

};

#endif // __GAME_CONTROLLER_H__