#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "resource.h"  // Võimalik, et peate kohandama teed

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Funktsioonide tüübid DLL-ist
typedef HGLOBAL(*LoadSpaceImageResourceFunc)();
typedef void (*FreeSpaceImageResourceFunc)(HGLOBAL);

GLuint loadTextureFromMemory(HGLOBAL hImageMemory, int& width, int& height) {
    // Hangi pildi andmed
    LPVOID pImageData = GlobalLock(hImageMemory);
    if (!pImageData) {
        std::cerr << "Failed to lock global memory" << std::endl;
        return 0;
    }

    DWORD imageSize = GlobalSize(hImageMemory);

    // Laadige pilt kasutades stb_image.h teeki
    int channels;
    unsigned char* image = stbi_load_from_memory(
        (unsigned char*)pImageData,
        imageSize,
        &width,
        &height,
        &channels,
        STBI_rgb_alpha
    );

    if (!image) {
        std::cerr << "Failed to load image from memory" << std::endl;
        GlobalUnlock(hImageMemory);
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Vabastage mälu
    stbi_image_free(image);
    GlobalUnlock(hImageMemory);

    return textureID;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Laadi DLL

    AllocConsole(); // Võimaldab konsooli väljundit
    FILE* pCout;
    freopen_s(&pCout, "CONOUT$", "w", stdout);

    std::cout << "Program started..." << std::endl;

    HMODULE hDll = LoadLibrary(L"IfsTexture.dll");
    if (hDll == NULL) {
        std::cerr << "Failed to load DLL: " << GetLastError() << std::endl;
        return -1;
    }

    // Hangi funktsioonid
    LoadSpaceImageResourceFunc loadImage =
        (LoadSpaceImageResourceFunc)GetProcAddress(hDll, "LoadSpaceImageResource");
    FreeSpaceImageResourceFunc freeImage =
        (FreeSpaceImageResourceFunc)GetProcAddress(hDll, "FreeSpaceImageResource");

    if (!loadImage || !freeImage) {
        std::cerr << "Failed to get function addresses: " << GetLastError() << std::endl;
        FreeLibrary(hDll);
        return -1;
    }

    // Laadi pilt DLL-ist
    HGLOBAL hImageMemory = loadImage();
    if (hImageMemory == NULL) {
        std::cerr << "Failed to load image from DLL" << std::endl;
        FreeLibrary(hDll);
        return -1;
    }

    // Initsialiseeri GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        freeImage(hImageMemory);
        FreeLibrary(hDll);
        return -1;
    }

    // Loo aken
    // Enne akna loomist lisage täiendav veakontroll
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1366, 768, "Infinity Space", NULL, NULL);
    if (!window) {
        // Täpsema veateate saamiseks
        const char* errorDescription;
        int errorCode = glfwGetError(&errorDescription);
        std::cerr << "Failed to create GLFW window. Error code: "
            << errorCode << ", Description: "
            << (errorDescription ? errorDescription : "Unknown error")
            << std::endl;
        glfwTerminate();
        return -1;
    }

    // Tee OpenGL kontekst aktiivseks
    glfwMakeContextCurrent(window);

    // Initsialiseeri GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        freeImage(hImageMemory);
        FreeLibrary(hDll);
        return -1;
    }

    int width, height;
    GLuint backgroundTexture = loadTextureFromMemory(hImageMemory, width, height);
    if (backgroundTexture == 0) {
        std::cerr << "Failed to load background texture" << std::endl;
        freeImage(hImageMemory);
        FreeLibrary(hDll);
        return -1;
    }

    // Peamine renderdamistsükkel
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Renderdage taust
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);

        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-1, -1);
        glTexCoord2f(1, 0); glVertex2f(1, -1);
        glTexCoord2f(1, 1); glVertex2f(1, 1);
        glTexCoord2f(0, 1); glVertex2f(-1, 1);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Puhastamine
    freeImage(hImageMemory);
    FreeLibrary(hDll);
    glfwTerminate();

    return 0;
}