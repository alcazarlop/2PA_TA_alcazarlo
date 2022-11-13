
#ifndef __WINDOW_CONTROLLER_H__
#define __WINDOW_CONTROLLER_H__ 1

#include <SDL.h>
#include <stdio.h>

class WindowController {

public:
	WindowController();
	WindowController(const WindowController& other);
	~WindowController();

	/** @brief Initialize the window controller
	 * 
	 * The function initialized the SDL_Renderer and SDL_Window,
	 * it return 0 if everything is created correctly,
	 * or -1 if SDL could not create a window or renderer
	 * 
	 * @return If the window controller have been initialize correctly
	 */ 
	int init();

	/** @brief Destroy the SDL_Renderer and SDL_Window
	 */ 
	void quit();

	/** @brief Return a pointer to a SDL_Renderer
	 * @return A pointer to SDL_Renderer
	 */ 
	SDL_Renderer* renderer() const;

	/** @brief Return a pointer to a SDL_Window
	 * @return A pointer to SDL_Window
	 */ 
	SDL_Window* window() const;

private:
	SDL_Renderer* renderer_;
	SDL_Window* window_;

};

#endif // __WINDOW_CONTROLLER_H__