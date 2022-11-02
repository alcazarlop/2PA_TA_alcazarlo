
#include "game_controller.h"

GameController::GameController(){
	isRunning_ = 0;
	openImgui_ = false;
}

GameController::GameController(const GameController& other){
	isRunning_ = other.isRunning_;
	openImgui_ = other.openImgui_;
}

GameController::~GameController(){

}

int GameController::init(){

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
		printf("SDL could not init: %s\n", SDL_GetError());
		return isRunning_ = -1;
	}

	isRunning_ = window_.init();

	ImGui::CreateContext();
  ImGui_ImplSDL2_InitForSDLRenderer(window_.window());
  ImGui_ImplSDLRenderer_Init(window_.renderer());

	return isRunning_ = 1;

}

void GameController::input(SDL_Event* e){
	while(SDL_PollEvent(e)){
		ImGui_ImplSDL2_ProcessEvent(e);
		switch(e->type){
			case SDL_QUIT: isRunning_ = 0; break;
			case SDL_KEYDOWN: 
				switch(e->key.keysym.sym){
					case SDLK_ESCAPE: isRunning_ = 0; break;
					case SDLK_F12: openImgui_ = !openImgui_; break;
				}
			break;
		}
	}
}

void GameController::update(){



}

void GameController::render(){
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame(window_.window());
  ImGui::NewFrame();
  SDL_SetRenderDrawColor(window_.renderer(), 0x0, 0x0, 0x0, 0xFF);
  SDL_RenderClear(window_.renderer());

	ImGuiSQLVisor(&db_);
	if(openImgui_) ImGui::ShowDemoWindow();

  ImGui::Render();
  ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
  SDL_RenderPresent(window_.renderer());
}

int GameController::loop(){

	init();
	SDL_Event eventHandler;

	while(isRunning_ > 0){
		input(&eventHandler);
		update();
		render();
	}

	quit();

	return isRunning_;

}

void GameController::quit(){
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
	window_.quit();
	SDL_Quit();
}