#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstdint>
#include "Rendering.h"
#include <vector>
#include "SpatialTypes.h"

class QuadTree;
class SpatialHashmap;

class Application{
public:
    Application(const char* title, uint32_t width, uint32_t height);
    ~Application();

    void Run();

private:
    
    Renderer *_renderer;
    std::vector<Entity> _entities;
    AABB *_worldBounds;

    // This is for Quad Tree Implementation
    QuadTree *_qTree;
    SpatialHashmap *_spGrid;
    // This is for Spatial Hashmap
    

    uint32_t _width, _height;
};

#endif