#include "Rendering.h"
#include <raylib.h>

Renderer::Renderer(const char* title, uint32_t width, uint32_t height){
    InitWindow(width, height, title);
    SetTargetFPS(60);
}

Renderer::~Renderer(){
    CloseWindow();
}

void Renderer::Draw(std::vector<Entity>& entities){
    BeginDrawing();
        ClearBackground(RAYWHITE);
        // Drawing entities with this loop.
        for(auto &e : entities){
            DrawCircle((int)e._position.x, (int)e._position.y, e._radius, PURPLE);
        }
    EndDrawing();
}