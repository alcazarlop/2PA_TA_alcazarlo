
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

	/** @brief Initialize the program
	 * 
	 * The function calls all the initializers that are needed to start the program.
	 * All the initializers should be called here if need it
	 * 
	 */
	int init();

	/** @brief Implements the structure of a basic game loop
	 * 
	 * The function is a wrapper that englobes, input, update and draw function,
	 * it also calls quit function, once the loop has finished,
	 * and init function before this is started
	 * 
	 */
	int loop();

	/** @brief Close and free the memory used by the program
	 * 
	 * All destructors and deallocation functions are, and must be, call in this function
	 * 
	 */ 
	void quit();

	/** @brief Manages the input 
	 * 
	 * All inputs implemented are, and must be, call in this function
	 * 
	 * @param e A reference to SDL pool event 
	 */
	void input(SDL_Event* e);

	/** @brief Updates the program
	 * 
	 * All updates are, and must be, call in this function
	 * 
	 */
	void update();

	/** @brief Updates the renderer of the program
	 * 
	 * All the render and draw calls are, and must be, call in this function 
	 * 
	 */ 
	void render();

private:
	int isRunning_;
	bool openImgui_;
	SQLController db_;
	WindowController window_;

};

#endif // __GAME_CONTROLLER_H__