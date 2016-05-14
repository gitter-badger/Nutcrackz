#include "imaingame.h"

#undef main

int main(int argc, char **argv)
{
	IMainGame *mainGame;

	mainGame->run("Test", 1280, 720, 0);

	return 0;
}