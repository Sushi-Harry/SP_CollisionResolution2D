#include "Application.h"
#include <raylib.h>
#include "EntityManager.h"
#include "NaiveAlgo.h"
#include "QuadTree.h"
#include "QuadTreeAlgo.h"
#include "SpatialHashGrid.h"
#include "SpatialHashGridAlgo.h"

Application::Application(const char* title, uint32_t width, uint32_t height) : _width(width), _height(height){
    _renderer = new Renderer(title, _width, _height);
    _worldBounds = new AABB({
        ._center = Vec2{(double)_width / 2.0, (double)_height / 2.0},
        ._halfDimension = Vec2{(double)_width / 2.0, (double)_height / 2.0}
    });
    // Quad Tree
    _qTree = new QuadTree(*_worldBounds);
    // Spatial Hashmap - Taking a cell size of 40. Why? Cause then stacking one after the other, I can fit 2 balls of radius 10
    _spGrid = new SpatialHashmap(_width, _height, 40);

    SpawnEntities(_entities, 100, 1280, 720);
}

Application::~Application(){
    delete _renderer;
    delete _worldBounds;
    delete _qTree;
    delete _spGrid;
}

/*
void Application::Run(){
    // Sample entity data here
    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        UpdateEntityPositions(_entities, 1280, 720, dt);

        _spGrid->Clear();
        for(Entity& e : _entities){
            e._colliding = false; // Reset to Red
            _spGrid->Insert(&e);   // Notice the '&' in the loop definition to get the real memory address!
        }

        int colCheck = 0;
        // int collisionCount = RunBruteForceCollisionCheck(_entities, colCheck);
        int collisionCount = RunSpatialHashmapCollisionCheck(_entities, *_spGrid, colCheck);
        //    __
        //   | |
        //   | |
        // __| |__
        // \ \/ /
        //  \_/
        // This has to be modified later cause it always draws grid. And that is not okay. Grid drawing has to be the user's choice
        std::vector<AABB> activeBounds;
        _qTree->GetActiveBounds(activeBounds);
        _renderer->Draw(_entities, activeBounds, colCheck, true);
    }
}
*/
/*
THIS IS FOR QUAD TREE TESTING
*/
void Application::Run(){
    // Sample entity data here
    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        UpdateEntityPositions(_entities, 1280, 720, dt);

        _qTree->Clear();
        for(Entity& e : _entities){
            e._colliding = false; // Reset to Red
            _qTree->Insert(&e);   // Notice the '&' in the loop definition to get the real memory address!
        }

        int colCheck = 0;
        // int collisionCount = RunBruteForceCollisionCheck(_entities, colCheck);
        int collisionCount = RunQuadTreeCollisionCheck(_entities, *_qTree, colCheck);
        //    __
        //   | |
        //   | |
        // __| |__
        // \ \/ /
        //  \_/
        // This has to be modified later cause it always draws grid. And that is not okay. Grid drawing has to be the user's choice
        std::vector<AABB> activeBounds;
        _qTree->GetActiveBounds(activeBounds);
        _renderer->Draw(_entities, activeBounds, colCheck, true);
    }
}
