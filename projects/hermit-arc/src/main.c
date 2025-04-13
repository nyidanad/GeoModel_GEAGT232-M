#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

const double POINT_RADIUS = 10.0;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const double TANGENT_CHANGE_SPEED = 5.0; // Görgő érzékenysége
const double TANGENT_ROTATION_SPEED = 0.1; // Forgatási sebesség

typedef struct Point {
    double x, y;
} Point;

typedef struct Tangent {
    double dx, dy;
} Tangent;

Point points[2];
Tangent tangents[2];
bool draggingPoint[2] = {false, false};
bool draggingTangent[2] = {false, false};
int selectedTangent = -1;
int selectedPointForScroll = -1; // Kiválasztott pont görgetéshez

// Hermite interpoláció számítása
double hermite_interpolate(double t, double p0, double p1, double m0, double m1) {
    double h00 = (2 * t * t * t) - (3 * t * t) + 1;
    double h10 = (t * t * t) - (2 * t * t) + t;
    double h01 = (-2 * t * t * t) + (3 * t * t);
    double h11 = (t * t * t) - (t * t);
    return h00 * p0 + h10 * m0 + h01 * p1 + h11 * m1;
}

void draw_points_and_tangents(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < 2; ++i) {
        SDL_RenderDrawLine(renderer, points[i].x - POINT_RADIUS, points[i].y, points[i].x + POINT_RADIUS, points[i].y);
        SDL_RenderDrawLine(renderer, points[i].x, points[i].y - POINT_RADIUS, points[i].x, points[i].y + POINT_RADIUS);
        SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[i].x + tangents[i].dx, points[i].y + tangents[i].dy);
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Hermite Curve", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    points[0] = (Point){200, 300};
    points[1] = (Point){600, 300};
    tangents[0] = (Tangent){100, -100};
    tangents[1] = (Tangent){100, 100};

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < 2; ++i) {
                        double dx = points[i].x - event.button.x;
                        double dy = points[i].y - event.button.y;
                        if (sqrt(dx * dx + dy * dy) <= POINT_RADIUS) {
                            draggingPoint[i] = true;
                            selectedPointForScroll = i; // Kiválasztott pont görgetéshez
                        }

                        dx = (points[i].x + tangents[i].dx) - event.button.x;
                        dy = (points[i].y + tangents[i].dy) - event.button.y;
                        if (sqrt(dx * dx + dy * dy) <= POINT_RADIUS) {
                            draggingTangent[i] = true;
                            selectedTangent = i;
                        }
                    }
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < 2; ++i) {
                        draggingPoint[i] = false;
                        draggingTangent[i] = false;
                    }
                    selectedTangent = -1;
                    selectedPointForScroll = -1; // Görgétéshez kiválasztott pont törlése.
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                for (int i = 0; i < 2; ++i) {
                    if (draggingPoint[i]) {
                        points[i].x = event.motion.x;
                        points[i].y = event.motion.y;
                    }
                    if (draggingTangent[i]) {
                        tangents[i].dx = event.motion.x - points[i].x;
                        tangents[i].dy = event.motion.y - points[i].y;
                    }
                }
            } else if (event.type == SDL_MOUSEWHEEL) {
                if (selectedPointForScroll != -1) {
                    if (SDL_GetModState() & KMOD_SHIFT) { // Ha a Shift lenyomva van
                        double angle = atan2(tangents[selectedPointForScroll].dy, tangents[selectedPointForScroll].dx);
                        angle += event.wheel.y * TANGENT_ROTATION_SPEED;
                        double length = sqrt(tangents[selectedPointForScroll].dx * tangents[selectedPointForScroll].dx + tangents[selectedPointForScroll].dy * tangents[selectedPointForScroll].dy);
                        tangents[selectedPointForScroll].dx = length * cos(angle);
                        tangents[selectedPointForScroll].dy = length * sin(angle);
                    } else {
                        tangents[selectedPointForScroll].dx += event.wheel.y * TANGENT_CHANGE_SPEED;
                        tangents[selectedPointForScroll].dy += event.wheel.y * TANGENT_CHANGE_SPEED;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        draw_points_and_tangents(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        for (double t = 0; t <= 1; t += 0.01) {
            double x = hermite_interpolate(t, points[0].x, points[1].x, tangents[0].dx, tangents[1].dx);
            double y = hermite_interpolate(t, points[0].y, points[1].y, tangents[0].dy, tangents[1].dy);
            if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
                SDL_RenderDrawPoint(renderer, (int)x, (int)y);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}