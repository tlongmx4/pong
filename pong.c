#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 100
#define BALL_SIZE 10
#define PADDLE_SPEED 5.0f
#define CPU_SPEED 4.5f
#define BALL_SPEED 5.0f

typedef struct {
    float x, y;
    float width, height;
    float speed;
} Paddle;

typedef struct {
    float PositionX;
    float PositionY;
    float VelocityX;
    float VelocityY;
} Ball;

void drawScore(SDL_Renderer *renderer, int leftScore, int rightScore) {
    TTF_Font *font = TTF_OpenFont("/System/Library/Fonts/SFNS.ttf", 24);
    if (!font) {
        printf("Could not load font: %s\n", TTF_GetError());
        return;
    }

    char scoreText[20];
    sprintf(scoreText, "%d - %d", leftScore, rightScore);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, scoreText, white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {WINDOW_WIDTH / 2 - textWidth / 2, 20, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void drawWinner(SDL_Renderer *renderer, const char *message) {
    TTF_Font *font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 48);
    if (!font) {
        printf("Could not load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, message, white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {WINDOW_WIDTH / 2 - textWidth / 2, WINDOW_HEIGHT / 2 - textHeight / 2, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());

        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Could not initialize SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
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

    Paddle leftPaddle = {50, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED};
    Paddle rightPaddle = {WINDOW_WIDTH - 50 - PADDLE_WIDTH, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED};
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

        SDL_Delay(16); // ~60 FPS

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderFillRect(renderer, &(SDL_Rect)
            {ball.PositionX, ball.PositionY, BALL_SIZE, BALL_SIZE});

        SDL_RenderFillRect(renderer, &(SDL_Rect)
            {leftPaddle.x, leftPaddle.y, leftPaddle.width, leftPaddle.height});
        SDL_RenderFillRect(renderer, &(SDL_Rect)
            {rightPaddle.x, rightPaddle.y, rightPaddle.width, rightPaddle.height});
        drawScore(renderer, leftScore, rightScore);
        SDL_RenderPresent(renderer);

        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W]) {
            leftPaddle.y -= leftPaddle.speed;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {
            leftPaddle.y += leftPaddle.speed;
        }

        ball.PositionX += ball.VelocityX;
        ball.PositionY += ball.VelocityY;

        if (rightPaddle.y + rightPaddle.height / 2 < ball.PositionY - 30) {
            rightPaddle.y += CPU_SPEED;
        } else if (rightPaddle.y + rightPaddle.height / 2 > ball.PositionY + 30) {
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

            if ((leftScore == 6 && rightScore == 0) || (leftScore == 0 && rightScore == 6)) {
                gameOver = true; // mercy rule
            } else if ((leftScore >= 10 || rightScore >= 10) && abs(leftScore - rightScore) >= 2) {
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

        if (gameOver) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            
            if (leftScore > rightScore) {
                drawWinner(renderer, "You win!");
                printf("You win! %d - %d\n", leftScore, rightScore);
            } else {
                drawWinner(renderer, "You lose!!");
                printf("You lose! %d - %d\n", rightScore, leftScore);
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(5000);
            running = false;
        }
    }

    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}



