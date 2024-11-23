#include "pch.h"
#include <windows.h>
#include <iostream>
#include "resource.h"

extern "C" {
    __declspec(dllexport) HGLOBAL LoadSpaceImageResource() {
        HMODULE hModule = GetModuleHandle(NULL);
        HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_BACKGROUND_IMAGE), RT_RCDATA);

        if (hResource == NULL) {
            std::cerr << "Resource not found" << std::endl;
            return NULL;
        }

        DWORD imageSize = SizeofResource(hModule, hResource);
        HGLOBAL hMemory = LoadResource(hModule, hResource);

        if (hMemory == NULL) {
            std::cerr << "Failed to load resource" << std::endl;
            return NULL;
        }

        LPVOID pImageData = LockResource(hMemory);

        HGLOBAL hGlobalMem = GlobalAlloc(GMEM_MOVEABLE, imageSize);
        if (hGlobalMem == NULL) {
            std::cerr << "Memory allocation failed" << std::endl;
            return NULL;
        }

        LPVOID pGlobalMem = GlobalLock(hGlobalMem);
        memcpy(pGlobalMem, pImageData, imageSize);
        GlobalUnlock(hGlobalMem);

        return hGlobalMem;
    }

    __declspec(dllexport) void FreeSpaceImageResource(HGLOBAL hMem) {
        if (hMem != NULL) {
            GlobalFree(hMem);
        }
    }
}