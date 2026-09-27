#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstdint>
#include "Rendering.h"

class Application{
public:
    Application(const char* title, uint32_t width, uint32_t height);
    ~Application();

    void Run();

private:
    
    Renderer *_renderer;
    uint32_t _width, _height;
};

#endif