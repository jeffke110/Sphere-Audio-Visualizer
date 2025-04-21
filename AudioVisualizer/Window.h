#ifndef WINDOW_H
#define WINDOW_H


// Window.h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
    Window(unsigned int width, unsigned int height, const char* title)
        : width(width), height(height), title(title), window(nullptr) {
    }

    bool init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return false;
        }

        glEnable(GL_DEPTH_TEST);
        return true;
    }

    GLFWwindow* get() const { return window; }
    bool shouldClose() const { return glfwWindowShouldClose(window); }
    void swapBuffers() { glfwSwapBuffers(window); }
    void pollEvents() { glfwPollEvents(); }
    void terminate() { glfwTerminate(); }

private:
    unsigned int width;
    unsigned int height;
    const char* title;
    GLFWwindow* window;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};

#endif // WINDOW_H