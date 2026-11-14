#include "Rendering.h"
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

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

CollisionSolver Renderer::DrawStartupMenu(){
    CollisionSolver selected = CollisionSolver::NONE;

    BeginDrawing();
    ClearBackground(RAYWHITE);

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.4F));
        // Gui window Box data
        float boxW =400.0F;
        float boxH = 330.0F;
        float boxX = (GetScreenWidth() - boxW) / 2.0F;
        float boxY = (GetScreenHeight() - boxH) / 2.0F;

        // Style data
        GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
        GuiWindowBox({boxX, boxY, boxW, boxH}, "Select the Collision Solver");

        // Button Data
        float bW = 300;
        float bH = 40;
        float bX = boxX + (boxX - bW) / 2.0F;
        float startY = boxY + 45.0F;
        float padding = 50.0F;

        if(GuiButton({bX, startY + padding*0, bW, bH}, "1.) Naive Method -  O(n^2)"))
            selected = CollisionSolver::NAIVE;
        if(GuiButton({bX, startY + padding*1, bW, bH}, "2.) Standard QuadTree"))
            selected = CollisionSolver::QUADTREE;
        if(GuiButton({bX, startY + padding*2, bW, bH}, "3.) Loose QuadTree (with k = 1.5)"))
            selected = CollisionSolver::LOOSE_QUADTREE;
        if(GuiButton({bX, startY + padding*3, bW, bH}, "4.) Spatial Hashing"))
            selected = CollisionSolver::SPATIAL_HASHING;
        if(GuiButton({bX, startY + padding*4, bW, bH}, "5.) Bounding Volume Hierarchy"))
            selected = CollisionSolver::BVH;
        
        EndDrawing();
    return selected;
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