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

typedef struct Tangent {
    double dx, dy;
} Tangent;

Point points[4]; // 4 pont a spline-hoz
bool draggingPoint[4] = {false, false, false, false};

// Bessel parabola érintő számítása
Tangent bessel_tangent(Point p0, Point p1, Point p2) {
    Tangent tangent;
    tangent.dx = (p2.x - p0.x) / 2.0;
    tangent.dy = (p2.y - p0.y) / 2.0;
    return tangent;
}

// Kör érintő számítása 3 pont alapján
Tangent circle_tangent(Point p0, Point p1, Point p2) {
    // Kör középpontjának számítása
    double A = p1.x - p0.x;
    double B = p1.y - p0.y;
    double C = p2.x - p1.x;
    double D = p2.y - p1.y;
    double E = A * (p0.x + p1.x) + B * (p0.y + p1.y);
    double F = C * (p1.x + p2.x) + D * (p1.y + p2.y);
    double G = 2 * (A * (p2.y - p1.y) - B * (p2.x - p1.x));

    if (G == 0) {
        // A pontok egy vonalon vannak, nem lehet kört számítani
        return (Tangent){0, 0};
    }

    double cx = (D * E - B * F) / G;
    double cy = (A * F - C * E) / G;

    // Érintő vektor számítása
    Tangent tangent;
    tangent.dx = p1.y - cy;
    tangent.dy = cx - p1.x;
    return tangent;
}

// Hermite interpoláció számítása
double hermite_interpolate(double t, double p0, double p1, double m0, double m1) {
    double h00 = (2 * t * t * t) - (3 * t * t) + 1;
    double h10 = (t * t * t) - (2 * t * t) + t;
    double h01 = (-2 * t * t * t) + (3 * t * t);
    double h11 = (t * t * t) - (t * t);
    return h00 * p0 + h10 * m0 + h01 * p1 + h11 * m1;
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
    SDL_Window* window = SDL_CreateWindow("Bessel Spline", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    points[0] = (Point){100, 300};
    points[1] = (Point){300, 200};
    points[2] = (Point){500, 400};
    points[3] = (Point){700, 300};

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
                        if (sqrt(dx * dx + dy * dy) <= POINT_RADIUS) {
                            draggingPoint[i] = true;
                        }
                    }
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < 4; ++i) {
                        draggingPoint[i] = false;
                    }
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                for (int i = 0; i < 4; ++i) {
                    if (draggingPoint[i]) {
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

        // Bessel spline rajzolása
        for (int i = 0; i < 2; ++i) {
            Tangent t0 = bessel_tangent(points[i], points[i + 1], points[i + 2]);
            Tangent t1 = bessel_tangent(points[i + 1], points[i + 2], i + 3 < 4 ? points[i + 3] : points[i + 2]);

            if (i == 1) {
                SDL_RenderDrawLine(renderer, points[i + 1].x, points[i + 1].y, points[i + 1].x + t0.dx, points[i + 1].y + t0.dy);
            }

            for (double t = 0; t <= 1; t += 0.01) {
                double x = hermite_interpolate(t, points[i + 1].x, points[i + 2].x, t0.dx, t1.dx);
                double y = hermite_interpolate(t, points[i + 1].y, points[i + 2].y, t0.dy, t1.dy);
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