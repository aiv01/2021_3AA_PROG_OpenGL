#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#ifdef _WIN32 
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif

void print_gl_info() {
	printf("GL VERSION: %s\n", glGetString(GL_VERSION));
	printf("GL RENDERER: %s\n", glGetString(GL_RENDERER));
	printf("GL VENDOR: %s\n", glGetString(GL_VENDOR));
	printf("GLSL VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

int main() {
	std::cout << "It' Working" << std::endl;

	if (!glfwInit()) {
		printf("Error Init GLFW\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	int width = 640;
	int height = 480;
	GLFWmonitor* monitor = NULL;
	monitor = glfwGetPrimaryMonitor();
	GLFWwindow* window = glfwCreateWindow(width, height, "OpengGL", monitor, NULL);
	if (window == NULL) {
		printf("Error creating window\n");
		return 2;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) {
		printf("Error loading glad\n");
		return 3;
	}

	print_gl_info();

	double currTime = glfwGetTime();
	double lastTime = currTime;

	float updateTime = 0.f;
	char title[50];

	while(!glfwWindowShouldClose(window)) {
		lastTime = currTime;
		currTime = glfwGetTime();
		float deltaTime = (float)(currTime - lastTime);
		int fps = (int)(1.f / deltaTime);

		updateTime += deltaTime;
		if (updateTime >= 1.f) {
			updateTime -= 1.f;
			sprintf_s(title, sizeof(title), "Delta Time: %f - Fps: %d", deltaTime, fps);
			glfwSetWindowTitle(window, title);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	return 0;	
}