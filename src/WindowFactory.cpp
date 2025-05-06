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

    std::unique_ptr<Window> WindowFactory::CreateWindow(const WindowParams& params) {
#ifdef HC_WINDOW_BACKEND_GLFW
        return std::make_unique<GLFWWindow>(params.size, params.name);
#elif defined(HC_WINDOW_BACKEND_SDL)
        return std::make_unique<SDLWindow>(params.size, params.name);
#elif defined(HC_WINDOW_BACKEND_WIN32)
        return std::make_unique<Win32Window>(params.size, params.name);
#else
#error "No window backend defined!"
#endif
    }

} // namespace HC::Window
