
#include "window_controller.h"

WindowController::WindowController(){
	renderer_ = nullptr;
	window_ = nullptr;
}

WindowController::WindowController(const WindowController& other){
	renderer_ = other.renderer_;
	window_ = other.window_;
}

WindowController::~WindowController(){

}

int WindowController::init(){
	window_ = SDL_CreateWindow("SQL Visor", 
															SDL_WINDOWPOS_CENTERED,
															SDL_WINDOWPOS_CENTERED,
															800,
															600,
															SDL_WINDOW_RESIZABLE);
	if(window_ == nullptr){
		printf("Could not create window: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if(renderer_ == nullptr){
		printf("Could not create renderer: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}
	return 0;
}

void WindowController::quit(){
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
}

SDL_Renderer* WindowController::renderer() const {
	return renderer_;
}

SDL_Window* WindowController::window() const {
	return window_;
}
