#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

const double POINT_RADIUS = 10.0;
const int N_POINTS = 4;
const int NUM_CURVE_POINTS = 100; // Pontok száma a Bezier görbén

typedef struct Point {
    double x;
    double y;
} Point;

Point lerp(Point a, Point b, double t) {
    Point result;
    result.x = (1 - t) * a.x + t * b.x;
    result.y = (1 - t) * a.y + t * b.y;
    return result;
}

void drawAuxiliaryLines(SDL_Renderer* renderer, Point* points, int numPoints, double t) {
    Point tempPoints[numPoints];
    for (int i = 0; i < numPoints; ++i) {
        tempPoints[i] = points[i];
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    for (int k = numPoints - 1; k > 0; --k) {
        for (int i = 0; i < k; ++i) {
            SDL_RenderDrawLine(renderer, tempPoints[i].x, tempPoints[i].y, tempPoints[i + 1].x, tempPoints[i + 1].y);
            tempPoints[i] = lerp(tempPoints[i], tempPoints[i + 1], t);
        }
    }
}

void drawBezierCurve(SDL_Renderer* renderer, Point* points, int numPoints) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    Point prevPoint = points[0];
    for (int i = 1; i < NUM_CURVE_POINTS; ++i) {
        double t = (double)i / (NUM_CURVE_POINTS - 1);
        Point tempPoints[numPoints];
        for (int j = 0; j < numPoints; ++j) {
            tempPoints[j] = points[j];
        }
        for (int k = numPoints - 1; k > 0; --k) {
            for (int j = 0; j < k; ++j) {
                tempPoints[j] = lerp(tempPoints[j], tempPoints[j + 1], t);
            }
        }
        Point currentPoint = tempPoints[0];
        SDL_RenderDrawLine(renderer, prevPoint.x, prevPoint.y, currentPoint.x, currentPoint.y);
        prevPoint = currentPoint;
    }
}

int main(int argc, char* argv[]) {
    int error_code;
    SDL_Window* window;
    bool need_run;
    SDL_Event event;
    SDL_Renderer* renderer;
    int mouse_x, mouse_y;
    double t = 0.5;
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

    window = SDL_CreateWindow("De-casteljau", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
                    break;
                case SDL_MOUSEBUTTONUP:
                    selected_point = NULL;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_q) {
                        need_run = false;
                    }
                    break;
                case SDL_QUIT:
                    need_run = false;
                    break;
                case SDL_MOUSEWHEEL:
                    t += event.wheel.y * 0.01;
                    if (t < 0.0) t = 0.0;
                    if (t > 1.0) t = 1.0;
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        for (int i = 0; i < N_POINTS; ++i) {
            SDL_RenderDrawLine(renderer, points[i].x - POINT_RADIUS, points[i].y, points[i].x + POINT_RADIUS, points[i].y);
            SDL_RenderDrawLine(renderer, points[i].x, points[i].y - POINT_RADIUS, points[i].x, points[i].y + POINT_RADIUS);
        }

        SDL_SetRenderDrawColor(renderer, 160, 160, 160, SDL_ALPHA_OPAQUE);
        for (int i = 1; i < N_POINTS; ++i) {
            SDL_RenderDrawLine(renderer, points[i - 1].x, points[i - 1].y, points[i].x, points[i].y);
        }

        drawAuxiliaryLines(renderer, points, N_POINTS, t);
        drawBezierCurve(renderer, points, N_POINTS);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}