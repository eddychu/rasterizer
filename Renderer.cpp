#include "Renderer.h"
#include "BlinnPhongScene.h"
#include "PBRScene.h"
Renderer::Renderer(const char *title, int width, int height)
	: mWidth(width), mHeight(height), mTicksCount(0), mIsRunning(false), pixelBuffer(width, height), depthBuffer(width, height)

{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		exit(1);
	}
	mWindow = SDL_CreateWindow(title,
							   SDL_WINDOWPOS_CENTERED,
							   SDL_WINDOWPOS_CENTERED,
							   mWidth,
							   mHeight,
							   SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		exit(1);
	}
	mSurface = SDL_GetWindowSurface(mWindow);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(SDL_ENABLE);
}

Renderer::~Renderer()
{
	delete scene;
}

void Renderer::Run()
{
	mIsRunning = true;
	Init();
	while (mIsRunning)
	{
		ProcessInput();
		auto delta = GetDelta();
		Update(delta);
		Clear();
		Draw();
		Display();
	}
	ShutDown();
}

void Renderer::Display()
{
	SDL_LockSurface(mSurface);
	memcpy(mSurface->pixels, pixelBuffer.GetPixels(), pixelBuffer.GetHeight() * pixelBuffer.GetWidth() * sizeof(Uint32));
	SDL_UnlockSurface(mSurface);
	SDL_UpdateWindowSurface(mWindow);
}

void Renderer::Init()
{
	// scene = new BlinnPhongScene();
	scene = new PBRScene();
	scene->Init();
}

void Renderer::ShutDown()
{
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Renderer::Update(float delta)
{
	scene->Update(delta);
}

float Renderer::GetDelta()
{
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
	//	;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	//if (deltaTime > 0.05f)
	//{
	//	deltaTime = 0.05f;
	//}
	std::cout << deltaTime << std::endl;
	mTicksCount = SDL_GetTicks();
	return deltaTime;
}

void Renderer::Draw()
{
	scene->Render(pixelBuffer, depthBuffer);
}

void Renderer::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
		scene->ProcessInput(event);
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
}

void Renderer::Clear()
{
	pixelBuffer.Clear();
	depthBuffer.Clear();
}
