#pragma once
#include "Window/Window.h"
#include "glm/ext/vector_uint2.hpp"
#include <GLFW/glfw3.h>
#include <string>

namespace HC::Window {

    class GLFWWindow : public Window {
    public:
        GLFWWindow(const glm::uvec2 & windowSize, const std::string & windowName);
        ~GLFWWindow() override;

        void Close() override;
        [[nodiscard]] bool IsOpen() const override;
        void PollEvents() override;
        void SwapBuffers() override;


        [[nodiscard]] WindowHandle GetNativeHandle() const override;

        bool IsVSyncEnabled() const override;

        glm::uvec2 GetSize() const override;

        std::string &GetWindowName() override;

        glm::uvec2 GetPosition() const override;

        WindowMode GetWindowMode() const override;

        bool IsFullscreen() const override;

        CursorMode GetCursorMode() const override;

        void SetSize(const glm::uvec2 &size) override;

        void SetVSync(bool enabled) override;

        void SetWindowName(const std::string &name) override;

        void SetPosition(const glm::uvec2 &position) override;

        void SetWindowMode(WindowMode mode) override;

        void SetCursorMode(CursorMode mode) override;

        void MakeContextCurrent() override;

    private:
        GLFWwindow* m_window;
        glm::uvec2 m_windowSize;
        std::string m_windowName;
        CursorMode m_cursorMode;
        WindowMode m_windowMode;
        bool m_vsyncEnabled;
    };

} // namespace HC::Window
