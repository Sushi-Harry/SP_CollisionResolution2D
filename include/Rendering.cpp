#include "Rendering.h"
#include <raylib.h>

Renderer::Renderer(const char* title, uint32_t width, uint32_t height){
    InitWindow(width, height, title);
    SetTargetFPS(60);
}

Renderer::~Renderer(){
    CloseWindow();
}

void Renderer::Draw(std::vector<Entity>& entities, std::vector<AABB>& treeBounds, int checkCount, bool showGrid){
    BeginDrawing();
        ClearBackground(RAYWHITE);
        // Drawing entities with this loop.
        for(auto &e : entities){
            if(!e._colliding){
                DrawCircle((int)e._position.x, (int)e._position.y, e._radius, BLACK);
            }else{
                DrawCircle((int)e._position.x, (int)e._position.y, e._radius, RED);
            }
        }

        DrawGUI(entities, treeBounds, checkCount, showGrid);
    EndDrawing();
}

void Renderer::DrawGUI(std::vector<Entity>& entities, std::vector<AABB>& treeBounds, int checkCount, bool showGrid){
    /// Grid drawing loop
    if(showGrid){
        for(const auto& b : treeBounds){
            DrawRectangleLines(
                (int)(b._center.x - b._halfDimension.x),
                (int)(b._center.y - b._halfDimension.y),
                (int)(b._halfDimension.x*2.0),
                (int)(b._halfDimension.y*2.0),
                GREEN
            );
        }
    }

    // DEBUGGING GUI
    DrawText("DEBUGGING DATA", 20, 15, 15, BLACK);
    DrawFPS(20, 30);
    DrawText(TextFormat("Entities (N): %d", (int)entities.size()), 20, 50, 22, BLACK);

    DrawText(TextFormat("Collision Checks: %d", checkCount), 20, 75, 22, BLACK);
}