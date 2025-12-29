#pragma once

#if HC_GRAPHIC_API_OPENGL
#define GLFW_INCLUDE_NONE
       #include<glad/glad.h>
#elif HC_GRAPHIC_API_VULKAN
    // import vulkan stuff
#endif
#if HC_USE_IMGUI
    #if HC_GRAPHIC_API_OPENGL

       #include <backends/imgui_impl_opengl3.h>
    #elif HC_GRAPHIC_API_VULKAN

    #elif HC_GRAPHIC_API_DX12

    #endif
    #include <imgui.h>
    #include <backends/imgui_impl_glfw.h>

#endif

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

        [[nodiscard]] bool IsVSyncEnabled() const override;

        [[nodiscard]] glm::uvec2 GetSize() const override;

        std::string &GetWindowName() override;

        [[nodiscard]] glm::uvec2 GetPosition() const override;

        [[nodiscard]] WindowMode GetWindowMode() const override;

        [[nodiscard]] bool IsFullscreen() const override;

        [[nodiscard]] CursorMode GetCursorMode() const override;

        void SetSize(const glm::uvec2 &size) override;

        void SetVSync(bool enabled) override;

        void SetWindowName(const std::string &name) override;

        void SetPosition(const glm::uvec2 &position) override;

        void SetWindowMode(WindowMode mode) override;

        void SetCursorMode(CursorMode mode) override;

        void MakeContextCurrent() override;

#if HC_USE_IMGUI
        void BeforeIMGUIRendering() override;
        void AfterIMGUIRendering() override;

#endif

    private:
    void Initialize();
#if HC_USE_IMGUI
        void InitializeIMGUI() override;
        void DestroyIMGUI() override;
#endif
    private:
        GLFWwindow* m_window;
        glm::uvec2 m_windowSize;
        std::string m_windowName;
        CursorMode m_cursorMode;
        WindowMode m_windowMode;
        bool m_vsyncEnabled{};

#if HC_USE_IMGUI
    bool m_imguiInitialized = false;
#endif
    };

} // namespace HC::Window
