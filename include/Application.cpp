#include "Application.h"
#include <raylib.h>
#include "EntityManager.h"
#include "NaiveAlgo.h"
#include "QuadTree.h"
#include "QuadTreeAlgo.h"
#include "SpatialHashGrid.h"
#include "SpatialHashGridAlgo.h"
#include "Loose_QuadTree.h"
#include "Loose_QuadTreeAlgo.h"
#include "BVH.h"
#include "BVHAlgo.h"

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
    // Loose Quad Tree
    _loose_qTree = new LooseQuadTree(*_worldBounds);
    // BVH
    _bvh = new BoundingVolumeHierarchy(10000);

    SpawnEntities(_entities, 1000, 1280, 720);
}

Application::~Application(){
    delete _renderer;
    delete _worldBounds;
    delete _qTree;
    delete _spGrid;
    delete _loose_qTree;
}

// Spatial Hashmap testing code here 
// Results: 100 Entities: around 200-250 collision checks
// 1000 Entities: around 4000 collision checks
// void Application::Run(){
//     // Sample entity data here
//     while(!WindowShouldClose()){
//         float dt = GetFrameTime();
//         UpdateEntityPositions(_entities, 1280, 720, dt);

//         _spGrid->Clear();
//         for(Entity& e : _entities){
//             e._colliding = false; // Reset to Red
//             _spGrid->Insert(&e);   // Notice the '&' in the loop definition to get the real memory address!
//         }

//         int colCheck = 0;
//         // int collisionCount = RunBruteForceCollisionCheck(_entities, colCheck);
//         int collisionCount = RunSpatialHashmapCollisionCheck(_entities, *_spGrid, colCheck);
//         //    __
//         //   | |
//         //   | |
//         // __| |__
//         // \ \/ /
//         //  \_/
//         // This has to be modified later cause it always draws grid. And that is not okay. Grid drawing has to be the user's choice
//         std::vector<AABB> activeBounds;
//         _qTree->GetActiveBounds(activeBounds);
//         _renderer->Draw(_entities, activeBounds, colCheck, true);
//     }
// }


// THIS IS FOR QUAD TREE TESTING
// Results: 100 Entities: 2500-2600 collision checks
// 1000 Entities: Around 44,000 collision checks
// void Application::Run(){
//     // Sample entity data here
//     while(!WindowShouldClose()){
//         float dt = GetFrameTime();
//         UpdateEntityPositions(_entities, 1280, 720, dt);

//         _qTree->Clear();
//         for(Entity& e : _entities){
//             e._colliding = false; // Reset to Red
//             _qTree->Insert(&e);   // Notice the '&' in the loop definition to get the real memory address!
//         }

//         int colCheck = 0;
//         // int collisionCount = RunBruteForceCollisionCheck(_entities, colCheck);
//         int collisionCount = RunQuadTreeCollisionCheck(_entities, *_qTree, colCheck);
//         //    __
//         //   | |
//         //   | |
//         // __| |__
//         // \ \/ /
//         //  \_/
//         // This has to be modified later cause it always draws grid. And that is not okay. Grid drawing has to be the user's choice
//         std::vector<AABB> activeBounds;
//         _qTree->GetActiveBounds(activeBounds);
//         _renderer->Draw(_entities, activeBounds, colCheck, true);
//     }
// }


// Alright so this is the run function for testing Loose Quad Trees
// Results: 1000 Entities: around 660k (yeah 660,000) collision checks
// 100 Entities: ~7200 collision checks
void Application::Run(){
    // Sample entity data here
    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        UpdateEntityPositions(_entities, 1280, 720, dt);

        _loose_qTree->Clear();
        for(Entity& e : _entities){
            e._colliding = false;
            _loose_qTree->Insert(&e);
        }

        int colCheck = 0;
        // int collisionCount = RunBruteForceCollisionCheck(_entities, colCheck);
        int collisionCount = RunLooseQuadTreeCollisionCheck(_entities, *_loose_qTree, colCheck);
        //    __
        //   | |
        //   | |
        // __| |__
        // \ \/ /
        //  \_/
        // This has to be modified later cause it always draws grid. And that is not okay. Grid drawing has to be the user's choice
        std::vector<AABB> activeBounds;
        _loose_qTree->GetActiveBounds(activeBounds);
        _renderer->Draw(_entities, activeBounds, colCheck, true);
    }
}


// // This one is for testing the BVH implementation
// // Results: 100 entities: Avg 1600 to a little above 1700 collision checks
// // 1000 entities: 32k - 33k collision checks
// void Application::Run(){
//     while (!WindowShouldClose()){
//         float dt = GetFrameTime();
//         UpdateEntityPositions(_entities, 1280, 720, dt);

//         std::vector<Entity*> bvhPointers;
//         bvhPointers.reserve(_entities.size());
//         for(Entity& e : _entities){
//             e._colliding = false;
//             bvhPointers.push_back(&e);
//         }

//         _bvh->Build(bvhPointers);

//         int colCheck = 0;
//         int collisionCount = RunBVHCollisionCheck(_entities, *_bvh, colCheck);

//         std::vector<AABB> activeBounds;
//         _bvh->GetActiveBounds(activeBounds);
//         _renderer->Draw(_entities, activeBounds, colCheck, true);
//     }
// }