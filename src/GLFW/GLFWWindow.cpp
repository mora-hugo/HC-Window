#include "GLFWWindow.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "Core.h"
HC::Window::GLFWWindow::GLFWWindow(const glm::uvec2 & windowSize, const std::string & windowName)
                : Window(), m_windowSize(windowSize), m_windowName(windowName), m_window(nullptr), m_cursorMode(CursorMode::Normal),
                  m_windowMode(WindowMode::Windowed), m_vsyncEnabled(false) {
            Initialize();
        }

        HC::Window::GLFWWindow::~GLFWWindow() {
            GLFWWindow::Close();
        }

        void HC::Window::GLFWWindow::Initialize() {
            HC_ASSERT(glfwInit(), "Failed to initialize GLFW");

        #if HC_GRAPHIC_API_OPENGL
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef DEBUG_MODE
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        #endif
        #endif

            m_window = glfwCreateWindow(static_cast<int>(m_windowSize.x), static_cast<int>(m_windowSize.y), m_windowName.c_str(), nullptr, nullptr);

            if (!m_window) {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
            }

            GLFWWindow::MakeContextCurrent();
            
            glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
                #if HC_GRAPHIC_API_OPENGL
                glViewport(0, 0, width, height);
                #endif
            });
            
            SetVSync(true);
            SetCursorMode(CursorMode::Normal);
        }


        void HC::Window::GLFWWindow::Close() {
#if HC_USE_IMGUI
    DestroyIMGUI();
#endif

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
            return {width, height};
        }

        std::string& HC::Window::GLFWWindow::GetWindowName() {
            return m_windowName;
        }

        glm::uvec2 HC::Window::GLFWWindow::GetPosition() const {
            int x, y;
            glfwGetWindowPos(m_window, &x, &y);
            return {x, y};
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
            glfwSetWindowSize(m_window, static_cast<int>(size.x), static_cast<int>(size.y));
#if HC_GRAPHIC_API_OPENGL
            glViewport(0, 0, static_cast<int>(size.x), static_cast<int>(size.y));
#endif
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
            glfwSetWindowPos(m_window, static_cast<int>(position.x), static_cast<int>(position.y));
        }

        void HC::Window::GLFWWindow::SetWindowMode(HC::Window::WindowMode mode) {
            if (mode == WindowMode::Fullscreen) {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(m_window, monitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
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
#if HC_USE_IMGUI
    void HC::Window::GLFWWindow::InitializeIMGUI() {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

#if HC_GRAPHIC_API_OPENGL
            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(m_window, true);
            ImGui_ImplOpenGL3_Init();
#elif HC_GRAPHIC_API_VULKAN
            // Do Vulkan initialization here
#endif
    }


void HC::Window::GLFWWindow::DestroyIMGUI() {
#if HC_GRAPHIC_API_OPENGL
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
#elif HC_GRAPHIC_API_VULKAN
            // Do Vulkan cleaning here
#endif
            ImGui::DestroyContext();

    }

    void HC::Window::GLFWWindow::BeforeIMGUIRendering() {

#if HC_USE_IMGUI
    if (!m_imguiInitialized)
    {
        InitializeIMGUI();
        m_imguiInitialized = true;
    }
#endif
#if HC_GRAPHIC_API_OPENGL
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
#elif HC_GRAPHIC_API_VULKAN
            // Do Vulkan stuff here
#endif
            ImGui::NewFrame();
    }

    void HC::Window::GLFWWindow::AfterIMGUIRendering() {
            ImGui::Render();
#if HC_GRAPHIC_API_OPENGL
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#elif HC_GRAPHIC_API_VULKAN
            // Do Vulkan stuff here
#endif
    }
#endif
