#pragma once

#include <string>
#include <functional>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

class Window {
    SDL_Window *windowHandle;
    SDL_GLContext glContext;

    bool running;

    std::function<void(void)> displayFunction;
    std::function<void(Sint32, Sint32)> resizeFunction;
    std::function<void(SDL_Keycode)> keyDownFunction;
    std::function<void(Uint8, Sint32, Sint32)> mouseDownFunction;
    std::function<void(Uint8, Sint32, Sint32)> mouseUpFunction;
    std::function<void(Sint32, Sint32)> mouseMoveFunction;

public:
    Window(const std::string &name, int width, int height);

    void run();

    void setOnDisplay(std::function<void(void)> displayFunction);
    void setOnResize(std::function<void(Sint32, Sint32)> resizeFunction);
    void setOnKeyDown(std::function<void(SDL_Keycode)> keyDownFunction);
    void setOnMouseButtonDown(std::function<void(Uint8, Sint32, Sint32)> mouseDownFunction);
    void setOnMouseButtonUp(std::function<void(Uint8, Sint32, Sint32)> mouseUpFunction);
    void setOnMouseMove(std::function<void(Sint32, Sint32)> mouseMoveFunction);

    void close();

    ~Window();

private:
    void loop();
};
