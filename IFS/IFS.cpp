#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initsialiseeri GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Määra akna omadused
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Keela akna suuruse muutmine

    // Loo aken
    GLFWwindow* window = glfwCreateWindow(800, 600, "Infinity Space", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Tee OpenGL kontekst aktiivseks
    glfwMakeContextCurrent(window);

    // Initsialiseeri GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // OpenGL põhiseaded
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    // Peamine renderdamistsükkel
    while (!glfwWindowShouldClose(window)) {
        // Puhasta ekraan
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Lisa oma renderduskood siia

        // Vaheta puhver
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Korista ressursid
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}