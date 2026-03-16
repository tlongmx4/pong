#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 100
#define BALL_SIZE 10
#define PADDLE_SPEED 3
#define BALL_SPEED 3

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());

        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
    "Pong", 
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, 
    WINDOW_WIDTH, 
    WINDOW_HEIGHT, 
    0
);

     if (!window) {
        printf("Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    typedef struct {
            float x, y;
            float width, height;
            float speed;
        } Paddle;

        Paddle leftPaddle = {50, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED};
        Paddle rightPaddle = {WINDOW_WIDTH - 50 - PADDLE_WIDTH, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED};

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderFillRect(renderer, &(SDL_Rect)
            {WINDOW_WIDTH / 2 - BALL_SIZE / 2, 
            WINDOW_HEIGHT / 2 - BALL_SIZE / 2, 
            BALL_SIZE, BALL_SIZE});

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &(SDL_Rect)
            {leftPaddle.x, leftPaddle.y, leftPaddle.width, leftPaddle.height});
        SDL_RenderFillRect(renderer, &(SDL_Rect)
            {rightPaddle.x, rightPaddle.y, rightPaddle.width, rightPaddle.height});
        SDL_RenderPresent(renderer);

        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W]) {
            leftPaddle.y -= leftPaddle.speed;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {
            leftPaddle.y += leftPaddle.speed;
        }
    }

    SDL_Quit();
    return 0;
}



