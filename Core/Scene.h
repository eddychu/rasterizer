#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "PixelBuffer.h"
#include "DepthBuffer.h"
struct Scene
{
    virtual ~Scene(){};
    virtual void Init() = 0;
    virtual void Update(float dt) = 0;
    virtual void Render(PixelBuffer &pixelBuffer, DepthBuffer &depthBuffer) = 0;
    virtual void ProcessInput(SDL_Event &event) = 0;
};