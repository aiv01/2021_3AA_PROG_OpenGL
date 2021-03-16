#include "Window.h"

void print_gl_info() {
	printf("GL VERSION: %s\n", glGetString(GL_VERSION));
	printf("GL RENDERER: %s\n", glGetString(GL_RENDERER));
	printf("GL VENDOR: %s\n", glGetString(GL_VENDOR));
	printf("GLSL VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

Window::Window(int width, int height, const std::string title, bool fullScreen)
{
    if (!glfwInit()) {
		printf("Error Init GLFW\n");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWmonitor* monitor = fullScreen ? glfwGetPrimaryMonitor() : NULL;

	m_window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
	if (m_window == NULL) {
		printf("Error creating window\n");
		return;
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGL()) {
		printf("Error loading glad\n");
		return;
	}

	print_gl_info();

    m_deltaTime = 0.f;
    m_lastTime = glfwGetTime();
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}

void Window::update()
{
	float currTime = glfwGetTime();
	m_deltaTime = (float)(currTime - m_lastTime);
    m_lastTime = currTime;

    glfwSwapBuffers(m_window);
	glfwPollEvents();
}

bool Window::isOpened()
{
    return !glfwWindowShouldClose(m_window);
}

float Window::getDeltaTime()
{
    return m_deltaTime;
}

void Window::setTitle(const std::string title)
{
    glfwSetWindowTitle(m_window, title.c_str());
}