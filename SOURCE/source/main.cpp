#include <vld.h>
#include <SDL.h>
#include <SDL_surface.h>
#include <iostream>
#include "Timer.h"
#include "Renderer.h"
#include "Scene.h"

void ShutDown(SDL_Window* pWindow);

int main(int argc, char* args[])
{
	// Create window
	SDL_Init(SDL_INIT_VIDEO);
	const uint32_t width{ 640 };
	const uint32_t height{ 480 };
	SDL_Window* pWindow = SDL_CreateWindow("RayTracer - Artuur Demeyer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	if (!pWindow) return 1;

	// Initialize framework
	dae::Timer* const pTimer = new dae::Timer();
	dae::Renderer* const pRenderer = new dae::Renderer(pWindow);
	dae::Scene* const pScene = new dae::Scene_W4_ExtraScene();
	float printTimer{ 0.0f };
	bool isLooping{ true };
	bool takeScreenshot{ false };
	pScene->Initialize();
	pTimer->Start();

	// Program loop
	while (isLooping)
	{
		SDL_Event e{};

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					isLooping = false;
					break;
				case SDL_KEYUP:
					if (e.key.keysym.scancode == SDL_SCANCODE_X)
					{
						takeScreenshot = true;
					}
					if (e.key.keysym.scancode == SDL_SCANCODE_F2)
					{
						pRenderer->ToggleShadows();
					}
					if (e.key.keysym.scancode == SDL_SCANCODE_F3)
					{
						pRenderer->CycleLigtingMode();
					}
					if (e.key.keysym.scancode == SDL_SCANCODE_F6)
					{
						pTimer->StartBenchmark();
					}
					break;
			}
		}

		pScene->Update(pTimer);
		pRenderer->SetScene(pScene);
		pRenderer->Render();
		pTimer->Update();
		printTimer += pTimer->GetElapsed();

		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "dFPS: " << pTimer->GetdFPS() << std::endl;
		}

		if (takeScreenshot)
		{
			if (!pRenderer->SaveBufferToImage()) std::cout << "Screenshot saved!" << std::endl;
			else std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
			takeScreenshot = false;
		}
	}

	// Quiting program
	pTimer->Stop();
	delete pScene;
	delete pRenderer;
	delete pTimer;
	ShutDown(pWindow);
	return 0;
}

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}