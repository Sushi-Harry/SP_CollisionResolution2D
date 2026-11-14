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

CollisionSolver Renderer::DrawStartupMenu(int& entity_cnt, bool& showgrid){
    CollisionSolver selected = CollisionSolver::NONE;

    BeginDrawing();
    ClearBackground(RAYWHITE);

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.4F));
        // Gui window Box data
        float boxW = GetScreenWidth() - 800.0F;
        float boxH = GetScreenHeight() - 100.0F;
        float boxX = (GetScreenWidth() - boxW) / 2.0F;
        float boxY = (GetScreenHeight() - boxH) / 2.0F;

        // Style data
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
        GuiWindowBox({boxX, boxY, boxW, boxH}, "Select the Collision Solver");

        // Button Data
        float bW = 330;
        float bH = 40;
        float padding = 50.0F;
        float bX = boxX + padding;
        float startY = boxY + padding;
        
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
        float sliderVal = (float)entity_cnt;
        GuiSlider({bX, (int)(startY + padding*5), bW, 20}, "50", "3000", &sliderVal, 50.0F, 3000.0F);
        entity_cnt = ((int)(sliderVal / 50.0F)) * 50;
        DrawText(TextFormat("Current Number Of Entities (N) : %d", entity_cnt), (int)bX, (int)(startY + padding*6), 20, DARKGRAY);

        GuiCheckBox({bX, (int)(startY + padding*7), 20, 20}, "Draw Bounds / Grid", &showgrid);
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