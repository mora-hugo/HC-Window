#pragma once
#include <Core.h>
#include <memory>
#include <glm/vec2.hpp>
#include <string>

#include "Window/Window.h"

namespace HC::Window {
    struct HC_WINDOW_API WindowParams {
        const glm::uvec2 size;
        const std::string name;
    };

    class HC_WINDOW_API WindowFactory {
        public:
            static Ref<Window> CreateWindow(const WindowParams& params);

            WindowFactory() = delete;
            WindowFactory(const WindowFactory&) = delete;
            WindowFactory& operator=(const WindowFactory&) = delete;
    };
} // namespace HC::Window