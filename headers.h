#pragma once

#define WINDOW_HEIGHT 1200	// 800
#define WINDOW_WIDTH 1000
#define MAP_HEIGHT 600  // 600
#define BLOCK_SIZE2D 30
#define MAPX 10
#define MAPY 10
#define PI 3.14159265359
#define PLAYER_SIZE 6
#define RAY_QUANTITY 981		// always odd number



void draw_2d(SDL_Renderer* renderer, int arr[MAPY][MAPX]);
void draw_2d_player(SDL_Renderer* renderer, int x, int y, float pdx, float pdy);
//void rotate(SDL_Renderer* renderer, float pdx, float pdy, float pa);
int check_collision(int x, int y, int arr[MAPY][MAPX]);
float ray(int x, int y, int arr[MAPY][MAPX], SDL_Renderer* renderer, float pa);
float ray2(int x, int y, int arr[MAPY][MAPX], SDL_Renderer* renderer, float pa, float ra);	// upgraded ray function
void draw_3d(SDL_Renderer* renderer, float distance);
float* cast_rays(int x, int y, int arr[MAPY][MAPX], SDL_Renderer* renderer, float pa);