#include <windows.h>
#include "window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    Window window(hInstance, nCmdShow);
    window.Run();
    return 0;
}