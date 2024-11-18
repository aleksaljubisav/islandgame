#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>
#include <queue>

#include "gVariablesConstants.h"
#include "gameLoop.h"
#include "backend.h"

using namespace std;

void renderGrid(vector<vector<int>> grid)
{
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			if (grid.at(i).at(j) == 0)
			{
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
			}
			else
			{
				int redSpectrum = 0 + (grid.at(i).at(j) - 1) * 165 / gMaxHeight;
				int greenSpectrum = 128 - (grid.at(i).at(j) - 1) * 86 / gMaxHeight;
				int blueSpectrum = 0 + (grid.at(i).at(j) - 1) * 42 / gMaxHeight;
				SDL_SetRenderDrawColor(gRenderer, redSpectrum, greenSpectrum, blueSpectrum, 255);
			}

			SDL_Rect field = { j * fieldSize, i * fieldSize, fieldSize, fieldSize };
			SDL_RenderFillRect(gRenderer, &field);
		
		}
	}

	SDL_RenderPresent(gRenderer);
}

double handleClick(int mouseX, int mouseY, vector<vector<double>> grid)
{
	int j = mouseX / fieldSize;
	int i = mouseY / fieldSize;

	if (i >= 0 && i < fieldSize && j >= 0 && j < fieldSize)
	{
		double height = grid.at(i).at(j);

		if (height > 0)
		{
			std::cout << "LAND HEIGHT: " << height << std::endl;
		}
		else
		{
			std::cout << "~~~~~~~~~~~~~~~" << std::endl;
		}
		return height;
	}
	else
	{
		return 0;
	}
}


double highestIsland(const vector<vector<int>>& heights, vector<vector<double>>& highestIsland) {

	int heightsSize = (int)heights.size();

	double max = 0.0;

	for (int i = 0; i < heightsSize; i++)
	{
		for (int j = 0; j < heightsSize; j++)
		{
			if (highestIsland.at(i).at(j) == -1)
			{
				if (heights.at(i).at(j) == 0)
				{
					highestIsland.at(i).at(j) = 0;
				}
				else
				{
					queue<pair<int, int>> next;
					queue<pair<int, int>> visited;

					next.push(make_pair(i, j));

					highestIsland.at(i).at(j) = 0;

					int sum = 0, count = 0;

					while (!next.empty()) {
						pair<int, int> current_position = next.front();
						next.pop();

						visited.push(current_position);

						int curr_i = current_position.first, curr_j = current_position.second;
						if (heights.at(curr_i).at(curr_j) > gMaxHeight)
						{
							gMaxHeight = heights.at(curr_i).at(curr_j); // Za opseg boja
						}
						sum += heights.at(curr_i).at(curr_j);
						count++;

						//cout << "Visited: " << curr_i << " " << curr_j << ". Sum: " << sum << ". Count: " << count << endl;
						if (0 <= curr_i - 1 && highestIsland.at(curr_i - 1).at(curr_j) == -1 && heights.at(curr_i - 1).at(curr_j) > 0) {
							next.push(make_pair(curr_i - 1, curr_j));
							highestIsland.at(curr_i - 1).at(curr_j) = 0;
						}
						if (curr_i + 1 < heightsSize && highestIsland.at(curr_i + 1).at(curr_j) == -1 && heights.at(curr_i + 1).at(curr_j) > 0)
						{
							next.push(make_pair(curr_i + 1, curr_j));
							highestIsland.at(curr_i + 1).at(curr_j) = 0;
						}
						if (0 <= curr_j - 1 && highestIsland.at(curr_i).at(curr_j - 1) == -1 && heights.at(curr_i).at(curr_j - 1) > 0)
						{
							next.push(make_pair(curr_i, curr_j - 1));
							highestIsland.at(curr_i).at(curr_j - 1) = 0;
						}
						if (curr_j + 1 < heightsSize && highestIsland.at(curr_i).at(curr_j + 1) == -1 && heights.at(curr_i).at(curr_j + 1) > 0)
						{
							next.push(make_pair(curr_i, curr_j + 1));
							highestIsland.at(curr_i).at(curr_j + 1) = 0;
						}
					}
					double average = sum * 1.0 / count;
					if (average > max)
					{
						max = average;
					}
					while (!visited.empty())
					{
						pair<int, int> current_position = visited.front();
						visited.pop();
						highestIsland.at(current_position.first).at(current_position.second) = average;
					}
				}
			}
		}
	}

	return max;
}

void renderWinScreen()
{
	SDL_SetRenderDrawColor(gRenderer, 34, 187, 69, 100);
	SDL_Rect map = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(gRenderer, &map);
	gTextTextureWin.render((SCREEN_WIDTH - gTextTextureWin.getWidth()) / 2, (SCREEN_HEIGHT - gTextTextureWin.getHeight()) / 2 - 50);
	gTextTextureRestart.render((SCREEN_WIDTH - gTextTextureRestart.getWidth()) / 2, (SCREEN_HEIGHT - gTextTextureRestart.getHeight()) / 2 + 50);

	SDL_RenderPresent(gRenderer);
}

void renderLoseScreen()
{
	SDL_SetRenderDrawColor(gRenderer, 193, 66, 63, 100);
	SDL_Rect map = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(gRenderer, &map);
	gTextTextureLose.render((SCREEN_WIDTH - gTextTextureLose.getWidth()) / 2, (SCREEN_HEIGHT - gTextTextureLose.getHeight()) / 2 - 50);
	gTextTextureRestart.render((SCREEN_WIDTH - gTextTextureRestart.getWidth()) / 2, (SCREEN_HEIGHT - gTextTextureRestart.getHeight()) / 2 + 50);

	SDL_RenderPresent(gRenderer);
}

void getNewGrid(vector<vector<int>>& heights)
{
	string url = "https://jobfair.nordeus.com/jf24-fullstack-challenge/test";
	string mapData;

	fetchMapData(url, mapData);
	parseMapData(mapData, heights);

}


void gameLoop()
{
	vector<vector<int>> heights;
	getNewGrid(heights);

	bool quit = false;
	bool finished = false;
	int attempts = 0;

	SDL_Event e;


	int heightsSize = (int)heights.size();
	vector<vector<double>> avgHeights(heightsSize);
	fill(avgHeights.begin(), avgHeights.end(), vector<double>(heightsSize, -1));
		
	double max = highestIsland(heights, avgHeights);
	double clickedHeight = 0;
	
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);
	renderGrid(heights);

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN && !finished)
			{
				clickedHeight = handleClick(e.button.x, e.button.y, avgHeights);

				if (clickedHeight > 0)
				{
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
					SDL_RenderClear(gRenderer);
					if (clickedHeight < max)
					{
						attempts++;
						if (attempts > 2)
						{
							finished = true;
							renderLoseScreen();
						}
						else
						{
							renderGrid(heights);
						}
					}
					else
					{
						renderWinScreen();
						finished = true;
					}
				}
			}
			else if(e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_r && finished)
				{
					finished = false;
					attempts = 0;


					heights.clear();
					getNewGrid(heights);

					fill(avgHeights.begin(), avgHeights.end(), vector<double>(heightsSize, -1));
					max = highestIsland(heights, avgHeights);
					clickedHeight = 0;


					SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
					SDL_RenderClear(gRenderer);
					renderGrid(heights);
				}
			}
		}
	}

}


/*	vector<vector<int>> heights = {
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};
*/