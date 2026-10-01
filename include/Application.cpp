#include "Application.h"
#include <raylib.h>
#include "EntityManager.h"
#include "NaiveAlgo.h"
#include "QuadTree.h"
#include "QuadTreeAlgo.h"

Application::Application(const char* title, uint32_t width, uint32_t height) : _width(width), _height(height){
    _renderer = new Renderer(title, _width, _height);
    _worldBounds = new AABB({
        ._center = Vec2{(double)_width / 2.0, (double)_height / 2.0},
        ._halfDimension = Vec2{(double)_width / 2.0, (double)_height / 2.0}
    });
    _qTree = new QuadTree(*_worldBounds);
    
    // Spawning Entities
    SpawnEntities(_entities, 100, 1280, 720);
    // Insert the data into the _qTree
    // for(Entity e : _entities){
    //     _qTree->Insert(&e);
    // }
}

Application::~Application(){
    delete _renderer;
    delete _worldBounds;
    delete _qTree;
}

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

        _renderer->Draw(_entities);
    }
}