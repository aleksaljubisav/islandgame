#include <iostream>
#include <SDL.h>

#include "helpers.h"
#include "gameLoop.h"

int main(int argc, char* args[])
{
	// Initialization
	if (!init())
	{
		std::cout << "Initialization failed." << std::endl;
	}
	else
	{
		// Loading media
		if (!loadMedia())
		{
			std::cout << "Loading madia failed." << std::endl;
		}
		else
		{
			gameLoop();
		}
	}

	close();

	//system("pause");

	return 0;
}