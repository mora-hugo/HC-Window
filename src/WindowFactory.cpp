#include "Window/WindowFactory.h"
#include <memory>
#ifdef HC_WINDOW_BACKEND_GLFW
#include "GLFW/GLFWWindow.h"
#endif

#ifdef HC_WINDOW_BACKEND_SDL
#include "SDL/SDLWindow.h"
#endif

#ifdef HC_WINDOW_BACKEND_WIN32
#include "Win32/Win32Window.h"
#endif

namespace HC::Window {

    Ref<Window> WindowFactory::CreateWindow(const WindowParams& params) {
#ifdef HC_WINDOW_BACKEND_GLFW
        return CreateRef<GLFW::GLFWWindow>(params.size, params.name);
#elif defined(HC_WINDOW_BACKEND_SDL)
        return CreateRef<SDL::SDLWindow>(params.size, params.name);
#elif defined(HC_WINDOW_BACKEND_WIN32)
        return CreateRef<Win32::Win32Window>(params.size, params.name);
#else
#error "No window backend defined!"
#endif
    }

} // namespace HC::Window
