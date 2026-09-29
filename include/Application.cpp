#include "Application.h"
#include <raylib.h>
#include "EntityManager.h"
#include "NaiveAlgo.h"

Application::Application(const char* title, uint32_t width, uint32_t height) : _width(width), _height(height){
    _renderer = new Renderer(title, _width, _height);
}

Application::~Application(){
    delete _renderer;
}

void Application::Run(){
    // Sample entity data here
    SpawnEntities(_entities, 100, 1280, 720);
    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        UpdateEntityPositions(_entities, 1280, 720, dt);
        
        int colCheck = 0;
        RunBruteForceCollisionCheck(_entities, colCheck);

        _renderer->Draw(_entities);
    }
}