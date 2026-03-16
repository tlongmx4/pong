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
#define PADDLE_SPEED 1
#define CPU_SPEED 0.75f
#define BALL_SPEED 0.75f

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
    
    typedef struct {
        float PositionX;
        float PositionY;
        float VelocityX;
        float VelocityY;
    } Ball;
    
    Ball ball = {WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2, BALL_SPEED, BALL_SPEED};

    int resetTimer = 0;
    int leftScore = 0;
    int rightScore = 0;
    bool gameOver = false;

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
            {ball.PositionX, ball.PositionY, BALL_SIZE, BALL_SIZE});

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

        ball.PositionX += ball.VelocityX;
        ball.PositionY += ball.VelocityY;

        if (rightPaddle.y + rightPaddle.height / 2 < ball.PositionY -30) {
            rightPaddle.y += CPU_SPEED;
        } else if (rightPaddle.y + rightPaddle.height / 2 > ball.PositionY +30) {
            rightPaddle.y -= CPU_SPEED;  
        }

        if ((ball.PositionX <= leftPaddle.x + leftPaddle.width && ball.PositionX >= leftPaddle.x) &&
            (ball.PositionY + BALL_SIZE >= leftPaddle.y && ball.PositionY <= leftPaddle.y + leftPaddle.height)) {
            ball.VelocityX = -ball.VelocityX;
            ball.PositionX = leftPaddle.x + leftPaddle.width; // Prevent sticking to the paddle
        }

        if ((ball.PositionX + BALL_SIZE >= rightPaddle.x && ball.PositionX + BALL_SIZE <= rightPaddle.x + rightPaddle.width) &&
            (ball.PositionY + BALL_SIZE >= rightPaddle.y && ball.PositionY <= rightPaddle.y + rightPaddle.height)) {
            ball.VelocityX = -ball.VelocityX;
            ball.PositionX = rightPaddle.x - BALL_SIZE; // Prevent sticking to the paddle
        }

        if (ball.PositionX < 0 || ball.PositionX + BALL_SIZE > WINDOW_WIDTH) {
            if (ball.PositionX < 0) {
                rightScore++;
            } else {
                leftScore++;
            }

            if (leftScore == 11 && rightScore == 0 || leftScore == 0 && rightScore == 11) {
                gameOver = true; // mercy rule
            } else if ((leftScore >= 21 || rightScore >= 21) && abs(leftScore - rightScore) >= 2) {
                gameOver = true; // normal win condition
            }

            ball.PositionX = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
            ball.PositionY = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
            ball.VelocityX = 0;
            ball.VelocityY = 0;
            leftPaddle.y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
            rightPaddle.y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
            resetTimer = 60;
        }

        if (resetTimer > 0) {
            resetTimer--;
        } else if (ball.VelocityX == 0 && ball.VelocityY == 0) {
            ball.VelocityX = BALL_SPEED * (rand() % 2 == 0 ? 1 : -1);
            ball.VelocityY = BALL_SPEED * (rand() % 2 == 0 ? 1 : -1);
        }

        if (ball.PositionY <= 0 && ball.VelocityY < 0) {
            ball.VelocityY = -ball.VelocityY;
            ball.PositionY = 0;
        } else if (ball.PositionY + BALL_SIZE >= WINDOW_HEIGHT && ball.VelocityY > 0) {
            ball.VelocityY = -ball.VelocityY;
            ball.PositionY = WINDOW_HEIGHT - BALL_SIZE;
        }

        if (leftPaddle.y < 0) leftPaddle.y = 0;
        if (leftPaddle.y + leftPaddle.height > WINDOW_HEIGHT) leftPaddle.y = WINDOW_HEIGHT - leftPaddle.height;
        if (rightPaddle.y < 0) rightPaddle.y = 0;
        if (rightPaddle.y + rightPaddle.height > WINDOW_HEIGHT) rightPaddle.y = WINDOW_HEIGHT - rightPaddle.height;
    }

    SDL_Quit();
    return 0;
}



