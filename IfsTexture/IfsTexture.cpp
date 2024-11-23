#include "pch.h"
#include <windows.h>
#include <iostream>
#include "resource.h"

extern "C" {
    __declspec(dllexport) HGLOBAL LoadSpaceImageResource() {
        HMODULE hModule = GetModuleHandle(NULL); // Saame käivitatud mooduli käitleja

        // Muutke IDR_BACKGROUND_IMAGE IDR_PHOTO1-ks
        HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_PHOTO1), RT_RCDATA);
        if (hResource == NULL) {
            std::cerr << "Could not find resource. Error: " << GetLastError() << std::endl;
            return NULL;
        }

        DWORD imageSize = SizeofResource(hModule, hResource);
        if (imageSize == 0) {
            std::cerr << "Could not get resource size. Error: " << GetLastError() << std::endl;
            return NULL;
        }

        HGLOBAL hLoadedResource = LoadResource(hModule, hResource);
        if (hLoadedResource == NULL) {
            std::cerr << "Could not load resource. Error: " << GetLastError() << std::endl;
            return NULL;
        }

        LPVOID pResourceData = LockResource(hLoadedResource);
        if (pResourceData == NULL) {
            std::cerr << "Could not lock resource. Error: " << GetLastError() << std::endl;
            return NULL;
        }

        HGLOBAL hGlobalMem = GlobalAlloc(GMEM_MOVEABLE, imageSize);
        if (hGlobalMem == NULL) {
            std::cerr << "Failed to allocate global memory. Error: " << GetLastError() << std::endl;
            return NULL;
        }

        LPVOID pGlobalMem = GlobalLock(hGlobalMem);
        if (pGlobalMem == NULL) {
            std::cerr << "Failed to lock global memory. Error: " << GetLastError() << std::endl;
            GlobalFree(hGlobalMem);
            return NULL;
        }

        memcpy(pGlobalMem, pResourceData, imageSize);
        GlobalUnlock(hGlobalMem);

        return hGlobalMem;
    }
}