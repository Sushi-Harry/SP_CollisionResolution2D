#ifndef RENDERING_H
#define RENDERING_H

#include <vector>
#include "SpatialTypes.h"

class Renderer{
public:
    Renderer(const char* title, uint32_t width, uint32_t height);
    ~Renderer();

    void Draw(std::vector<Entity>& entities, std::vector<AABB>& treeBounds, int checkCount, bool showGrid);

private:
    void DrawGUI(std::vector<Entity>& entities, std::vector<AABB>& treeBounds, int checkCount, bool showGrid);
};

#endif