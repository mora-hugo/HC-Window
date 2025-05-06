#include "GLFWWindow.h"
#include <iostream>
#include <GLFW/glfw3.h>

        HC::Window::GLFWWindow::GLFWWindow(const glm::uvec2 & windowSize, const std::string & windowName)
                : Window(), m_windowSize(windowSize), m_windowName(windowName), m_window(nullptr) {
            if (!glfwInit()) {
                std::cerr << "Failed to initialize GLFW" << std::endl;
            }

            m_window = glfwCreateWindow(m_windowSize.x, m_windowSize.y, m_windowName.c_str(), nullptr, nullptr);

            if (!m_window) {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
            }
        }

        HC::Window::GLFWWindow::~GLFWWindow() {
            Close();
        }

        void HC::Window::GLFWWindow::Close() {
            if (m_window) {
                glfwDestroyWindow(m_window);
                m_window = nullptr;
                glfwTerminate();
            }
        }

        bool HC::Window::GLFWWindow::IsOpen() const {
            return m_window != nullptr && !glfwWindowShouldClose(m_window);
        }

        void HC::Window::GLFWWindow::PollEvents() {
            glfwPollEvents();
        }

        void HC::Window::GLFWWindow::SwapBuffers() {
            if (m_window) {
                glfwSwapBuffers(m_window);
            }
        }

        WindowHandle HC::Window::GLFWWindow::GetNativeHandle() const {
            return reinterpret_cast<WindowHandle>(m_window);
        }

        bool HC::Window::GLFWWindow::IsVSyncEnabled() const {
            return m_vsyncEnabled;
        }

        glm::uvec2 HC::Window::GLFWWindow::GetSize() const {
            int width, height;
            glfwGetWindowSize(m_window, &width, &height);
            return glm::uvec2(width, height);
        }

        std::string& HC::Window::GLFWWindow::GetWindowName() {
            return m_windowName;
        }

        glm::uvec2 HC::Window::GLFWWindow::GetPosition() const {
            int x, y;
            glfwGetWindowPos(m_window, &x, &y);
            return glm::uvec2(x, y);
        }

        HC::Window::WindowMode HC::Window::GLFWWindow::GetWindowMode() const {
            return m_windowMode;
        }

        bool HC::Window::GLFWWindow::IsFullscreen() const {
            return glfwGetWindowMonitor(m_window) != nullptr;
        }

        HC::Window::CursorMode HC::Window::GLFWWindow::GetCursorMode() const {
            return m_cursorMode;
        }

        void HC::Window::GLFWWindow::SetSize(const glm::uvec2 &size) {
            glfwSetWindowSize(m_window, size.x, size.y);
            m_windowSize = size;
        }

        void HC::Window::GLFWWindow::SetVSync(bool enabled) {
            glfwSwapInterval(enabled ? 1 : 0);
            m_vsyncEnabled = enabled;
        }

        void HC::Window::GLFWWindow::SetWindowName(const std::string &name) {
            glfwSetWindowTitle(m_window, name.c_str());
            m_windowName = name;
        }

        void HC::Window::GLFWWindow::SetPosition(const glm::uvec2 &position) {
            glfwSetWindowPos(m_window, position.x, position.y);
        }

        void HC::Window::GLFWWindow::SetWindowMode(HC::Window::WindowMode mode) {
            if (mode == WindowMode::Fullscreen) {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            } else if (mode == WindowMode::Windowed) {
                glfwSetWindowMonitor(m_window, nullptr, 0, 0, m_windowSize.x, m_windowSize.y, 0);
            }
            m_windowMode = mode;
        }

        void HC::Window::GLFWWindow::SetCursorMode(HC::Window::CursorMode mode) {
            if (mode == CursorMode::Hidden) {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            } else if (mode == CursorMode::Normal) {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else if (mode == CursorMode::Disabled) {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            m_cursorMode = mode;
        }

        void HC::Window::GLFWWindow::MakeContextCurrent() {
            glfwMakeContextCurrent(m_window);
        }
