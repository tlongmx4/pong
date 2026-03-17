# Learn C With Pong

A fully functional Pong game built in C using SDL2, created as a first project while learning the C programming language.

## Dependencies

- [SDL2](https://www.libsdl.org/)
- [SDL2_ttf](https://wiki.libsdl.org/SDL2_ttf/FrontPage)

Install both on macOS with Homebrew:

```
brew install sdl2 sdl2_ttf
```

## Building

```
clang pong.c -o pong -I/opt/homebrew/opt/sdl2/include -I/opt/homebrew/opt/sdl2_ttf/include $(sdl2-config --cflags --libs) -lSDL2_ttf -L/opt/homebrew/opt/sdl2_ttf/lib
```

## Running

```
./pong
```

## Controls

| Key | Action |
|-----|--------|
| W | Move paddle up |
| S | Move paddle down |

## Game Rules

- First to **10 points** wins
- Must win by **2 or more points**
- If the score reaches **6-0**, the leading player wins immediately (mercy rule)
- The right paddle is controlled by the CPU

## About

This project was built from scratch over two days as an introduction to C programming. It covers SDL2 window and renderer setup, structs, game loops, collision detection, CPU AI, and font rendering with SDL2_ttf.