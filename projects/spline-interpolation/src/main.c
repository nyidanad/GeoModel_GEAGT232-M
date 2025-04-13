#include <SDL2/SDL.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

const double POINT_RADIUS = 10.0;
const int N_POINTS = 4;

/**
 * A simple point structure.
 */
typedef struct Point
{
  double x;
  double y;
} Point;

/**
 * C/SDL2 framework for experimentation with curves.
 */
int main(int argc, char* argv[])
{
  int error_code;
  SDL_Window* window;
  bool need_run;
  SDL_Event event;
  SDL_Renderer* renderer;

  int mouse_x, mouse_y;
  int i;

  Point* selected_point = NULL;
  Point points[N_POINTS];
  points[0].x = 200;
  points[0].y = 200;
  points[1].x = 400;
  points[1].y = 200;
  points[2].x = 200;
  points[2].y = 400;
  points[3].x = 400;
  points[3].y = 400;

  error_code = SDL_Init(SDL_INIT_EVERYTHING);
  if (error_code != 0) {
    printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
    return error_code;
  }

  window = SDL_CreateWindow(
    "Splines",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    800, 600, 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  /*
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  for (i = 0; i < 255; ++i) {
    SDL_SetRenderDrawColor(renderer, 255, i, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, 0, 0, 500, i * 2);
  }
  SDL_RenderPresent(renderer);
  */

  need_run = true;
  while (need_run) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_MOUSEBUTTONDOWN:
        SDL_GetMouseState(&mouse_x, &mouse_y);
        selected_point = NULL;
        for (int i = 0; i < N_POINTS; ++i) {
          double dx = points[i].x - mouse_x;
          double dy = points[i].y - mouse_y;
          double distance = sqrt(dx * dx + dy * dy);
          if (distance < POINT_RADIUS) {
            selected_point = points + i;
          }
        }
        break;
      case SDL_MOUSEMOTION:
        if (selected_point != NULL) {
          SDL_GetMouseState(&mouse_x, &mouse_y);
          selected_point->x = mouse_x;
          selected_point->y = mouse_y;
        }
        // NOTE: It has not optimized for efficient redraw!
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        // Draw the control points 
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        for (int i = 0; i < N_POINTS; ++i) {
          SDL_RenderDrawLine(renderer, points[i].x - POINT_RADIUS, points[i].y, points[i].x + POINT_RADIUS, points[i].y);
          SDL_RenderDrawLine(renderer, points[i].x, points[i].y - POINT_RADIUS, points[i].x, points[i].y + POINT_RADIUS);
        }
        // Draw the segments
        SDL_SetRenderDrawColor(renderer, 160, 160, 160, SDL_ALPHA_OPAQUE);
        for (int i = 1; i < N_POINTS; ++i) {
          SDL_RenderDrawLine(renderer, points[i - 1].x, points[i - 1].y, points[i].x, points[i].y);
        }
        // Display the results
        SDL_RenderPresent(renderer);
        break;
      case SDL_MOUSEBUTTONUP:
        selected_point = NULL;
        break;
      case SDL_KEYDOWN:
        case SDL_SCANCODE_Q:
        need_run = false;
        break;
      case SDL_QUIT:
        need_run = false;
        break;
      }
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

