#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
    public:
        Window(int width, int height, const std::string title, bool fullScreen = false);
        ~Window();
        void update();
        bool isOpened();
        float getDeltaTime();
        void setTitle(const std::string title);
        void printGLInfo();

    private:
        float m_deltaTime;
        float m_lastTime;
        int m_width;
        int m_height;
        GLFWwindow* m_window;
};