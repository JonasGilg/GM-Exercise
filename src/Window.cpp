#include "Window.h"
#include <utility>

Window::Window(const std::string &name, int width, int height) : running(false) {
    windowHandle = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                                    SDL_WINDOW_OPENGL);
    SDL_SetWindowResizable(windowHandle, SDL_TRUE);
    glContext = SDL_GL_CreateContext(windowHandle);
}

void Window::close() {
    running = false;
}

void Window::setOnDisplay(std::function<void(void)> displayFunction) {
    this->displayFunction = std::move(displayFunction);
}

void Window::setOnResize(std::function<void(Sint32, Sint32)> resizeFunction) {
    this->resizeFunction = std::move(resizeFunction);
}

void Window::setOnKeyDown(std::function<void(SDL_Keycode)> keyDownFunction) {
    this->keyDownFunction = std::move(keyDownFunction);
}

void Window::setOnMouseButtonDown(std::function<void(Uint8, Sint32, Sint32)> mouseDownFunction) {
    this->mouseDownFunction = std::move(mouseDownFunction);
}

void Window::setOnMouseButtonUp(std::function<void(Uint8, Sint32, Sint32)> mouseUpFunction) {
    this->mouseUpFunction = std::move(mouseUpFunction);
}

void Window::setOnMouseMove(std::function<void(Sint32, Sint32)> mouseMoveFunction) {
    this->mouseMoveFunction = std::move(mouseMoveFunction);
}

void Window::run() {
    running = true;
    loop();
}

void Window::loop() {
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_WINDOWEVENT_RESIZED:
                    resizeFunction(event.window.data1, event.window.data2);
                    break;

                case SDL_KEYDOWN:
                    keyDownFunction(event.key.keysym.sym);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    mouseDownFunction(event.button.button, event.button.x, event.button.y);
                    break;

                case SDL_MOUSEBUTTONUP:
                    mouseUpFunction(event.button.button, event.button.x, event.button.y);
                    break;

                case SDL_MOUSEMOTION:
                    mouseMoveFunction(event.motion.x, event.motion.y);
                    break;

                case SDL_QUIT:
                    running = false;

                default:
                    break;
            }
        }

        displayFunction();
        SDL_GL_SwapWindow(windowHandle);
    }
}

Window::~Window() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(windowHandle);
}
