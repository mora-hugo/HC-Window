-- HC-Window Module
ThirdPartyLibCpp("HC-Window", {
    files = {
        "include/**.h",
        "src/WindowFactory.cpp",
        "src/GLFW/GLFWWindow.cpp",
        "src/GLFW/GLFWWindow.h"
    },
    includedirs = {
        "include",
        "src"
    },
    defines = {
        "HC_WINDOW_STATIC",
        "HC_WINDOW_BACKEND_GLFW",
        "HC_GRAPHIC_API_OPENGL",
        "HC_USE_IMGUI",
        "IMGUI_IMPL_OPENGL_LOADER_GLAD"
    },
    dependencies = {
        UseCommon,
        UseGlad,
        UseGLFW,
        UseGLM,
        UseHCAsserts,
        UseImGui,
        UseOpenGL
    }
})
