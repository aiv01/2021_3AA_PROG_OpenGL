#include "Window.h"
#include <iostream>
#include "TriangleDraw.h"
#include "QuadDraw.h"

// #ifdef _WIN32
// extern "C" {
// 	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
// 	__declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
// }
// #endif

int main()
{
	std::cout << "It' Working" << std::endl;
	Window *window = new Window(640, 480, "OpenGLTest");
	float updateTime = 0.f;
	char title[50];

	window->printGLInfo();

	//TriangleDraw currentDraw;
	QuadDraw currentDraw;

	currentDraw.start();
	while (window->isOpened())
	{
		int fps = (int)(1.f / window->getDeltaTime());

		updateTime += window->getDeltaTime();
		if (updateTime >= 1.f)
		{
			updateTime -= 1.f;
			sprintf_s(title, sizeof(title), "Delta Time: %f - Fps: %d", window->getDeltaTime(), fps);
			window->setTitle(title);
		}

		currentDraw.update();

		window->update();
	}

	currentDraw.destroy();
	delete window;
	return 0;
}