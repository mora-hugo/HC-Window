#pragma once

#include "Window/Window.h"
#include <SDL.h>
#include <string>
#include <glm/vec2.hpp>

namespace HC::Window::SDL {

    class SDLWindow : public Window {
    public:
        SDLWindow(const glm::uvec2& windowSize, const std::string& windowName);
        ~SDLWindow() override;

        [[nodiscard]] bool IsOpen() const override;
        [[nodiscard]] bool IsVSyncEnabled() const override;
        [[nodiscard]] glm::uvec2 GetSize() const override;
        std::string& GetWindowName() override;
        [[nodiscard]] glm::uvec2 GetPosition() const override;
        [[nodiscard]] WindowMode GetWindowMode() const override;
        [[nodiscard]] bool IsFullscreen() const override;
        [[nodiscard]] CursorMode GetCursorMode() const override;
        [[nodiscard]] WindowHandle GetNativeHandle() const override;

        void SetSize(const glm::uvec2& size) override;
        void SetVSync(bool enabled) override;
        void SetWindowName(const std::string& name) override;
        void SetPosition(const glm::uvec2& position) override;
        void SetWindowMode(WindowMode mode) override;
        void SetCursorMode(CursorMode mode) override;
        void Close() override;
        void PollEvents() override;
        void SwapBuffers() override;
        void MakeContextCurrent() override;

    private:
        SDL_Window*   m_window       = nullptr;
        SDL_Renderer* m_renderer     = nullptr;
        uint32_t      m_width        = 0;
        uint32_t      m_height       = 0;
        std::string   m_windowName;
        bool          m_open         = false;
        bool          m_vsyncEnabled = false;
        WindowMode    m_windowMode   = WindowMode::Windowed;
        CursorMode    m_cursorMode   = CursorMode::Normal;

        void recreateRenderer();  // helper pour toggler le VSync
    };

} // namespace HC::Window::SDL
