#pragma once

#include <SDL2/SDL.h>
#include <string>

class Window {
    SDL_Window *windowHandle;

public:
    Window(const std::string &name, int width, int height) {
        windowHandle = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    }
};
