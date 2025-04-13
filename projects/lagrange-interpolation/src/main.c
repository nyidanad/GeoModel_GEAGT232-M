#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

const double POINT_RADIUS = 10.0;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

typedef struct Point {
    double x, y;
} Point;

Point points[4];
bool dragging[4] = {false, false, false, false};

double lagrange_interpolate(double t, Point* points, int degree, int n) {
    double result = 0.0;
    for (int i = 0; i <= degree; ++i) {
        double term = points[i].y;
        for (int j = 0; j <= degree; ++j) {
            if (i != j) {
                term *= (t - points[j].x) / (points[i].x - points[j].x);
            }
        }
        result += term;
    }
    return result;
}

void draw_points(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < 4; ++i) {
        SDL_RenderDrawLine(renderer, points[i].x - POINT_RADIUS, points[i].y, points[i].x + POINT_RADIUS, points[i].y);
        SDL_RenderDrawLine(renderer, points[i].x, points[i].y - POINT_RADIUS, points[i].x, points[i].y + POINT_RADIUS);
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Lagrange Interpolation Curve", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    points[0] = (Point){200, 300};
    points[1] = (Point){400, 200};
    points[2] = (Point){600, 400};
    points[3] = (Point){700, 100};
    
    bool running = true;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < 4; ++i) {
                        double dx = points[i].x - event.button.x;
                        double dy = points[i].y - event.button.y;
                        if (sqrt(dx*dx + dy*dy) <= POINT_RADIUS) {
                            dragging[i] = true;
                        }
                    }
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < 4; ++i) {
                        dragging[i] = false;
                    }
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                for (int i = 0; i < 4; ++i) {
                    if (dragging[i]) {
                        points[i].x = event.motion.x;
                        points[i].y = event.motion.y;
                    }
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        
        draw_points(renderer);
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        for (double t = points[0].x; t <= points[3].x; t += 1) {
            double y = lagrange_interpolate(t, points, 3, 4);
            if (t >= 0 && t < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
                SDL_RenderDrawPoint(renderer, (int)t, (int)y);
            }
        }
        
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
