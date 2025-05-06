#include "SDLWindow.h"
#include <iostream>


    static bool s_sdlInitialized = false;

    HC::Window::SDLWindow::SDLWindow(const glm::uvec2& windowSize, const std::string& windowName)
            : m_width(windowSize.x)
            , m_height(windowSize.y)
            , m_windowName(windowName)
    {
        if (!s_sdlInitialized) {
            if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
                return;
            }
            s_sdlInitialized = true;
        }

        Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
        m_window = SDL_CreateWindow(
                m_windowName.c_str(),
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                static_cast<int>(m_width), static_cast<int>(m_height),
                flags
        );
        if (!m_window) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            return;
        }

        recreateRenderer();
        m_open = (m_renderer != nullptr);
    }

    HC::Window::SDLWindow::~SDLWindow() {
        Close();
    }

    bool HC::Window::SDLWindow::IsOpen() const {
        // on considère la fenêtre ouverte tant que m_open est true
        return m_open;
    }

    bool HC::Window::SDLWindow::IsVSyncEnabled() const {
        return m_vsyncEnabled;
    }

    glm::uvec2 HC::Window::SDLWindow::GetSize() const {
        int w, h;
        SDL_GetWindowSize(m_window, &w, &h);
        return glm::uvec2(w, h);
    }

    std::string& HC::Window::SDLWindow::GetWindowName() {
        return m_windowName;
    }

    glm::uvec2 HC::Window::SDLWindow::GetPosition() const {
        int x, y;
        SDL_GetWindowPosition(m_window, &x, &y);
        return glm::uvec2(x, y);
    }

    HC::Window::WindowMode HC::Window::SDLWindow::GetWindowMode() const {
        return m_windowMode;
    }

    bool HC::Window::SDLWindow::IsFullscreen() const {
        return m_windowMode == WindowMode::Fullscreen;
    }

    HC::Window::CursorMode HC::Window::SDLWindow::GetCursorMode() const {
        return m_cursorMode;
    }

    WindowHandle HC::Window::SDLWindow::GetNativeHandle() const {
        return reinterpret_cast<WindowHandle>(m_window);
    }

    void HC::Window::SDLWindow::SetSize(const glm::uvec2& size) {
        SDL_SetWindowSize(m_window, size.x, size.y);
        m_width  = size.x;
        m_height = size.y;
    }

    void HC::Window::SDLWindow::SetVSync(bool enabled) {
        if (m_vsyncEnabled == enabled) return;
        m_vsyncEnabled = enabled;
        recreateRenderer();
    }

    void HC::Window::SDLWindow::SetWindowName(const std::string& name) {
        SDL_SetWindowTitle(m_window, name.c_str());
        m_windowName = name;
    }

    void HC::Window::SDLWindow::SetPosition(const glm::uvec2& position) {
        SDL_SetWindowPosition(m_window, position.x, position.y);
    }

    void HC::Window::SDLWindow::SetWindowMode(WindowMode mode) {
        Uint32 sdlFlag = 0;
        if (mode == WindowMode::Fullscreen) {
            sdlFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        SDL_SetWindowFullscreen(m_window, sdlFlag);
        m_windowMode = mode;
    }

    void HC::Window::SDLWindow::SetCursorMode(HC::Window::CursorMode mode) {
        switch (mode) {
            case CursorMode::Normal:
                SDL_ShowCursor(SDL_ENABLE);
                SDL_SetRelativeMouseMode(SDL_FALSE);
                break;
            case CursorMode::Hidden:
                SDL_ShowCursor(SDL_DISABLE);
                SDL_SetRelativeMouseMode(SDL_FALSE);
                break;
            case CursorMode::Disabled:
                SDL_ShowCursor(SDL_DISABLE);
                SDL_SetRelativeMouseMode(SDL_TRUE);
                break;
        }
        m_cursorMode = mode;
    }

    void HC::Window::SDLWindow::Close() {
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        m_open = false;
    }

    void HC::Window::SDLWindow::PollEvents() {
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            switch (evt.type) {
                case SDL_QUIT:
                    Close();
                    break;
                case SDL_WINDOWEVENT:
                    if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                        m_width  = evt.window.data1;
                        m_height = evt.window.data2;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    void HC::Window::SDLWindow::SwapBuffers() {
        if (m_renderer) {
            SDL_RenderPresent(m_renderer);
        }
    }

    void HC::Window::SDLWindow::MakeContextCurrent() {
        // NO Contexte needed
    }

    void HC::Window::SDLWindow::recreateRenderer() {
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }
        if (!m_window) return;

        Uint32 flags = SDL_RENDERER_ACCELERATED;
        if (m_vsyncEnabled) flags |= SDL_RENDERER_PRESENTVSYNC;
        m_renderer = SDL_CreateRenderer(m_window, -1, flags);
        if (!m_renderer) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        }
    }
