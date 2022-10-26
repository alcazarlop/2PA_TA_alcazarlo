
#include <SDL.h>
#include "game_controller.h"

int main(int argc, char** argv){

	argc = 0;
	argv = nullptr;

	GameController* gc = new GameController();
	return gc->loop();
}