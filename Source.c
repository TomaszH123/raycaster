#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <math.h>
#include "headers.h"




int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("abcd", 400, 400, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	int running = 1;
	SDL_Event event;

	int x = 140;
	int y = 115;

	float pdx = 0;
	float pdy = 0;
	float pa = PI;

	pdy = 5 * cos(pa);
	pdx = 5 * sin(pa);

	int map[MAPY][MAPX] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
		{1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
		{1, 0, 0, 0, 1, 1, 0, 1, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_w:
					//y--;
					if (!check_collision(x + pdx, y + pdy, map)) {
						x += pdx;
						y += pdy;
					}
					break;
				case SDLK_s:
					//y++;
					if (!check_collision(x - pdx, y - pdy, map)) {
						x -= pdx;
						y -= pdy;
					}
					break;
				case SDLK_a:
					x--;
					break;
				case SDLK_d:
					x++;
					break;
				case SDLK_RIGHT:
					pa += (PI / 32);
					if (pa > 2 * PI) {
						pa -= 2 * PI;
					}
					pdy = 5 * cos(pa);
					pdx = 5 * sin(pa);
					break;
				case SDLK_LEFT:
					pa -= (PI / 32);
					if (pa < 0) {
						pa += 2 * PI;
					}
					pdy = 5 * cos(pa);
					pdx = 5 * sin(pa);
					
					break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, 0, MAP_HEIGHT, WINDOW_WIDTH, MAP_HEIGHT);

		draw_2d(renderer, map);
		draw_2d_player(renderer, x, y, pdx, pdy);
		//float distance = 
		//ray2(x, y, map, renderer, pa, pa);
		//draw_3d(renderer, distance, 1);
		cast_rays(x, y, map, renderer, pa);


		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	return 0;
}

void draw_2d(SDL_Renderer* renderer, int arr[MAPY][MAPX]) {
	// draw map
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int i = 0; i < MAPX; i++) {
        for (int j = 0; j < MAPY; j++) {
            if (arr[j][i]) {
                //SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_Rect rect = { 100 + i * BLOCK_SIZE2D, MAP_HEIGHT + 10 + j * BLOCK_SIZE2D, BLOCK_SIZE2D, BLOCK_SIZE2D };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

	// draw grids
	int height = MAP_HEIGHT + 10;
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	for (int i = height; i < height + 6 * BLOCK_SIZE2D; i = i + BLOCK_SIZE2D) {
		SDL_RenderDrawLine(renderer, 100, i, 340, i);
	}
	for (int i = 100; i < 100 + 9 * BLOCK_SIZE2D; i = i + BLOCK_SIZE2D) {
		SDL_RenderDrawLine(renderer, i, height, i, MAP_HEIGHT + 160);
	}
}


void draw_2d_player(SDL_Renderer* renderer, int x, int y, float pdx, float pdy) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_Rect rect = { x, MAP_HEIGHT + y, PLAYER_SIZE, PLAYER_SIZE };
	SDL_RenderFillRect(renderer, &rect);

	// draw direction
	//SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x + pdx * 5, y + MAP_HEIGHT + pdy * 5);
}


// return 1 if collision detected
int check_collision(int x, int y, int arr[MAPY][MAPX]) {
	y = y + MAP_HEIGHT;
	for (int i = 0; i < MAPX; i++) {
		for (int j = 0; j < MAPY; j++) {
			if (arr[j][i]) {
				// left top
				int block_ax = 100 + i * BLOCK_SIZE2D;
				int block_ay = MAP_HEIGHT + 10 + j * BLOCK_SIZE2D;
				// right top
				int block_bx = block_ax + BLOCK_SIZE2D;
				// left bottom
				int block_cy = block_ay + BLOCK_SIZE2D;


				if (block_ax <= x && x <= block_bx && block_ay <= y && y <= block_cy) {
					//printf("collision\n");
					return 1;
					
				}
			}
		}
	}
	return 0;
}


float ray(int x, int y, int arr[MAPY][MAPX], SDL_Renderer* renderer, float pa) {
	pa = pa + (PI/2);
	float slope;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	float x2, y2;

	if (pa < 1.45 * PI - 0.01 && pa > 1.76714) {
		slope = -tan(pa);
		x2 = x;
		y2 = y;
		
		while (1) {
			y2 += slope;
			x2++;
			if (check_collision(x2, y2, arr)) {
				SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
				break;
			}
		}
	}
	// left
	else if (pa > 1.55*PI && pa < 7.7557) {
		slope = -tan(pa);
		//SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x -20, y + MAP_HEIGHT + 20 * -slope);

		x2 = x;
		y2 = y;

		while (1) {
			y2 -= slope;
			x2--;
			if (check_collision(x2, y2, arr)) {
				SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
				break;
			}
		}
	}
	else if (pa > 1.5 * PI - 0.01 && pa < 1.5 * PI + 0.01) {
		y2 = y;
		x2 = x;
		while (1) {
			y2--;
			if (check_collision(x, y2, arr)) {
				SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x, y2 + MAP_HEIGHT);
				break;
			}
		}
	}
	// straight line
	else if (pa > 2.5 * PI - 0.01 && pa < 2.5 * PI + 0.01) {
		y2 = y;
		x2 = x;
		while (1) {
			y2++;
			if (check_collision(x, y2, arr)) {
				SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x, y2 + MAP_HEIGHT);
				break;
			}
		}
	}


	// || pa > 4.6142 && pa < 4.6142
	// left
	else if (pa > 4.8104 && pa < 4.8106) {
		slope = -tan((0.5 * PI) - pa);
		x2 = x;
		y2 = y;

		while (1) {
			x2 -= slope;
			y2--;
			if (check_collision(x2, y2, arr)) {
				SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
				break;
			}
		}
	}
	// 4.614214
	else if (pa > 4.614 && pa < 4.615) {
		slope = tan((0.5 * PI) - pa);
		x2 = x;
		y2 = y;

		while (1) {
			x2 += slope;
			y2--;
			if (check_collision(x2, y2, arr)) {
				SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
				break;
			}
		}
	}
	else if (pa > 7.7558 && pa < 7.7559) {
		slope = tan((0.5 * PI) - pa);
		x2 = x;
		y2 = y;

		while (1) {
			x2 -= slope;
			y2++;
			if (check_collision(x2, y2, arr)) {
				SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
				break;
			}
		}
	}
	else if (pa > 1.6689 && pa < 1.6690) {
		slope = tan((0.5 * PI) - pa);
		x2 = x;
		y2 = y;

		while (1) {
			x2 -= slope;
			y2++;
			if (check_collision(x2, y2, arr)) {
				SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
				break;
			}
		}
	}
	else {
		x2 = 0;
		y2 = 0;
	}
	
	float distance = sqrt(pow((x - x2), 2) + pow((y - y2), 2));

	return distance;
}

float ray2(int x, int y, int arr[MAPY][MAPX], SDL_Renderer* renderer, float pa, float ra) {			// upgraded ray function
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	ra = ra + (PI / 2);
	pa = pa + (PI / 2);
	if (ra > 2.5 * PI) {
	//	printf("sdgf");
		//ra -= (2.5 * PI);
	}
	float x2 = x;
	float y2 = y;
	float slope;

	// right up
	//printf("ra: %f\n", ra);
	if (ra >= 1.25 * PI - 0.0001 && ra <= 1.5 * PI + 0.0001) {		//	ra >=1.25*PI && ra <= 1.5 * PI+0.0001
		//printf("a\n");
		slope = -tan(0.5 * PI - ra);

		while (1) {
			y2--;
			x2 -= slope;
			if (check_collision(x2, y2, arr)) {
				break;
			}
		}
		SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
	}
	// right middle up
	else if (ra >= PI - 0.0001 && ra <= 1.25 * PI + 0.0001) {
		//printf("b\n");
		slope = -tan(ra);

		while (1) {
			x2++;
			y2 += slope;
			if (check_collision(x2, y2, arr)) {
				break;
			}
		}
		SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
	}
	// right middle down
	else if (ra >= PI * 0.75 - 0.0001 && ra <= PI + 0.0001) {
		//printf("c\n");
		slope = -tan(ra);
		
		while (1) {
			x2++;
			y2 += slope;
			if (check_collision(x2, y2, arr)) {
				break;
			}
		}
		SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
	}
	// right down
	else if (ra >= PI * 0.5 - 0.0001 && ra <= PI + 0.0001) {
		//printf("d\n");
		slope = -tan(0.5 * PI - ra);

		while (1) {
			y2++;
			x2 += slope;
			if (check_collision(x2, y2, arr)) {
				break;
			}
		}
		SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
	}

	// left down
	else if (ra <= 2.5 * PI + 0.0001 && ra >= 2.25 * PI - 0.0001) {
		//printf("e\n");
		slope = -tan(0.5 * PI - ra);

		while (1) {
			y2++;
			x2 += slope;
			if (check_collision(x2, y2, arr)) {
				break;
			}
		}
		SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
	}
	// left middle down
	else if (ra <= 2.25 * PI + 0.0001 && ra >= 2 * PI - 0.0001) {
		//printf("f\n");
		slope = -tan(ra);

		while (1) {
			x2--;
			y2 -= slope;
			if (check_collision(x2, y2, arr)) {
				break;
			}
		}
		SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
	}
	// left middle up
	else if (ra <= 2 * PI + 0.0001 && ra >= 1.75 * PI - 0.0001) {
		//printf("g\n");
		slope = -tan(ra);

		while (1) {
			x2--;
			y2 -= slope;
			if (check_collision(x2, y2, arr)) {
				break;
			}
		}
		SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
	}
	// left up
	else if (ra <= 1.75 * PI + 0.0001 && ra >= 1.5 * PI - 0.0001) {
		//printf("h\n");
		slope = -tan(0.5 * PI - ra);

		while (1) {
			y2--;
			x2 -= slope;
			if (check_collision(x2, y2, arr)) {
				break;
			}
		}
		SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// test 
	else if (ra >= PI * 1) {
		//printf("d\n");
		slope = -tan(0.5 * PI - ra);

		while (1) {
			y2++;
			x2 += slope;
			if (check_collision(x2, y2, arr)) {
				break;
			}
		}
		SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
	}

	// left down
	else {
		//printf("e\n");
		slope = -tan(0.5 * PI - ra);

		while (1) {
			y2++;
			x2 += slope;
			if (check_collision(x2, y2, arr)) {
				break;
			}
		}
		SDL_RenderDrawLine(renderer, x, y + MAP_HEIGHT, x2, y2 + MAP_HEIGHT);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/*else {
		printf("ra: %f\n", ra / PI);
	}*/


	float ca = pa - ra;
	if (ca < 0.5*PI) {
		ca += 2 * PI;
	}
	if (ca > 2.5 * PI) {
		ca -= 2 * PI;
	}
	ca = (PI) / ra;
	float distance = sqrt(pow((x - x2), 2) + pow((y - y2), 2));
	//distance = distance * cos(ca+0.5*PI);

	return distance;
}

void draw_3d(SDL_Renderer* renderer, float distance, int b) {
	const int MIDDLE_HEIGHT = (WINDOW_HEIGHT - (WINDOW_HEIGHT - MAP_HEIGHT)) / 2;
	const int MIDDLE_WIDTH = WINDOW_WIDTH / 2;

	float a = (MIDDLE_HEIGHT) - distance;

	SDL_SetRenderDrawColor(renderer, 255-a*0.5, 0 + a * 0.5, 255-a*0.5, 255);
	SDL_RenderDrawLine(renderer, MIDDLE_WIDTH + b, MIDDLE_HEIGHT + a, MIDDLE_WIDTH + b, MIDDLE_HEIGHT - a);
}


float* cast_rays(int x, int y, int arr[MAPY][MAPX], SDL_Renderer* renderer, float pa) {
	int middle = ceil(RAY_QUANTITY / 2);
	float angle = PI / (2048);
	int a = -middle + 1;
	for (int i = 0; i < RAY_QUANTITY; i++) {
		//printf("pa: %f\n",pa + angle * a);
		float distances = ray2(x, y, arr, renderer, pa, pa + angle * a);

		draw_3d(renderer, distances, a*1);

		a++;
	}
}
