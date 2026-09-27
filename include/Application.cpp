#include "Application.h"
#include <raylib.h>

Application::Application(const char* title, uint32_t width, uint32_t height) : _width(width), _height(height){
    _renderer = new Renderer(title, _width, _height);
}

Application::~Application(){

}

void Application::Run(){
    while(!WindowShouldClose()){
        // Actual drawing and simulation happens here
    }
}