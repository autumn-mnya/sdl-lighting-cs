#include <windows.h>
#include <iostream>
#include "AutPI.h"
#include "SDL_API.h"
#include "mod_loader.h"
#include "cave_story.h"
#include <SDL.h>
#include <vector>

HMODULE sdlDLL = nullptr;  // Global variable

void LoadSdlDll()
{
    sdlDLL = LoadLibrary("sdl.dll");
    if (sdlDLL == nullptr) {
        std::cerr << "Failed to load sdl.dll\n";
        // You might want to handle the error appropriately, e.g., throw an exception or return early.
    }
}

typedef SDL_Window* (*Get_SDL_WindowFunc)();

typedef SDL_Renderer* (*Get_SDL_RendererFunc)();

typedef void* (*Get_SurfaceFunc)(int);

typedef int* (*Get_MagFunc)();

SDL_Window* Get_SDL_Window()
{
    Get_SDL_WindowFunc func = reinterpret_cast<Get_SDL_WindowFunc>(
        GetProcAddress(sdlDLL, "Get_SDL_Window"));

    if (func == nullptr) {
        std::cerr << "Failed to get the function pointer for Get_SDL_Window\n";
        return nullptr;
    }

    SDL_Window* win = func();
    return win;
}

SDL_Renderer* Get_SDL_Renderer()
{
    Get_SDL_RendererFunc func = reinterpret_cast<Get_SDL_RendererFunc>(
        GetProcAddress(sdlDLL, "Get_SDL_Renderer"));

    if (func == nullptr) {
        std::cerr << "Failed to get the function pointer for Get_SDL_Renderer\n";
        return nullptr;
    }
    SDL_Renderer* ren = func();
    return ren;
}

void* Get_Surface(int surface_id)
{
    Get_SurfaceFunc func = reinterpret_cast<Get_SurfaceFunc>(
        GetProcAddress(sdlDLL, "Get_Surface"));

    if (func == nullptr) {
        std::cerr << "Failed to get the function pointer for Get_Surface\n";
        return nullptr;
    }
    void* surf = func(surface_id);
    return surf;
}

typedef void (*SDL_RegisterElementFunc)(void (*)());
void SDL_RegisterElement(std::vector<void (*)()>& handlers, const char* functionName, void (*handler)())
{
    if (sdlDLL != nullptr) {
        SDL_RegisterElementFunc registerFunc =
            reinterpret_cast<SDL_RegisterElementFunc>(
                GetProcAddress(sdlDLL, functionName));

        if (registerFunc != nullptr) {
            registerFunc(handler);
            handlers.push_back(handler);
        }
        else {
            std::cerr << "Failed to get the function pointer for " << functionName << "\n";
            // You might want to handle the error appropriately.
        }
    }
}

std::vector<SdlStartDirectDrawElementHandler> sdlstartdirectdrawElementHandlers;

void RegisterSDLStartDirectDrawElement(SdlStartDirectDrawElementHandler handler)
{
    SDL_RegisterElement(sdlstartdirectdrawElementHandlers, "RegisterSDLStartDirectDrawElement", reinterpret_cast<void (*)()>(handler));
}