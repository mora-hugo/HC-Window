#pragma once
#include "Window/Core.h"
#include <glm/vec2.hpp>
#include <string>

namespace HC::Window {

    enum class WindowMode {
        Fullscreen,
        Windowed,
    };

    enum class CursorMode {
        Normal,
        Hidden,
        Disabled,
    };
    class HC_WINDOW_API Window {
        friend class WindowFactory;
        public:

            Window() = default;
            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;
            virtual ~Window() = default;

            [[nodiscard]] virtual bool IsOpen() const = 0;
            [[nodiscard]] virtual bool IsVSyncEnabled() const = 0;
            [[nodiscard]] virtual glm::uvec2 GetSize() const = 0;
            [[nodiscard]] virtual std::string& GetWindowName() = 0;
            [[nodiscard]] virtual glm::uvec2 GetPosition() const = 0;
            [[nodiscard]] virtual WindowMode GetWindowMode() const = 0;
            [[nodiscard]] virtual bool IsFullscreen() const = 0;
            [[nodiscard]] virtual CursorMode GetCursorMode() const = 0;
            [[nodiscard]] virtual WindowHandle GetNativeHandle() const = 0;


            virtual void SetSize(const glm::uvec2& size) = 0;
            virtual void SetVSync(bool enabled) = 0;
            virtual void SetWindowName(const std::string& name) = 0;
            virtual void SetPosition(const glm::uvec2& position) = 0;
            virtual void SetWindowMode(WindowMode mode) = 0;
            virtual void SetCursorMode(CursorMode mode) = 0;

            virtual void Close() = 0;
            virtual void PollEvents() = 0;
            virtual void SwapBuffers() = 0;
            virtual void MakeContextCurrent() = 0;

#if HC_USE_IMGUI
        virtual void BeforeIMGUIRendering() = 0;
        virtual void AfterIMGUIRendering() = 0;
#endif

    private:
#if HC_USE_IMGUI
        virtual void InitializeIMGUI() = 0;
        virtual void DestroyIMGUI() = 0;
#endif


    };

} // nam