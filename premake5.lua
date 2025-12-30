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
