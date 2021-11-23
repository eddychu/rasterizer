#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <memory>
#include "PixelBuffer.h"
#include "DepthBuffer.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
class Renderer
{
public:
	Renderer(const char* title, int width, int height);
	~Renderer();
	void Run();
	void Init();
	void Draw();
	void ShutDown();
	void Update(float delta);
	float GetDelta();
	void ProcessInput();
	void Display();
private:
	void Clear();

	int mWidth;
	int mHeight;
	SDL_Window* mWindow;
	SDL_Surface* mSurface;
	Uint32 mTicksCount;
	bool mIsRunning;

	PixelBuffer pixelBuffer;
	DepthBuffer depthBuffer;



	std::vector<Mesh*> meshes;
	BlinnPhongShader shader;
	Camera camera;

	bool mouseFirst = true;
};
