#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstdint>
#include "Rendering.h"
#include <vector>
#include "SpatialTypes.h"

class QuadTree;

class Application{
public:
    Application(const char* title, uint32_t width, uint32_t height);
    ~Application();

    void Run();

private:
    
    Renderer *_renderer;
    std::vector<Entity> _entities;
    AABB *_worldBounds;
    QuadTree *_qTree;
    uint32_t _width, _height;
};

#endif