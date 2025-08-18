cmake_minimum_required(VERSION 3.25)

project(HC-Window VERSION 0.1.0)

set(HC_WINDOW_LINKING "STATIC" CACHE STRING "Linking type (STATIC ou SHARED)")
set_property(CACHE HC_WINDOW_LINKING PROPERTY STRINGS STATIC SHARED)

set(HC_WINDOW_BACKEND "GLFW" CACHE STRING "Backend to use (GLFW, SDL, WIN32)")
set_property(CACHE HC_WINDOW_BACKEND PROPERTY STRINGS GLFW SDL WIN32)

set(HC_WINDOW_BUILD_EXAMPLES "" CACHE BOOL "Build examples")

set(HC_USE_IMGUI "" CACHE BOOL "Use IMGUI")

set(HC_GRAPHIC_API "" CACHE STRING "Graphic API to use (VULKAN, OPENGL, DX12)")
# DLLS
set(HC_WINDOW_DLLS "" CACHE INTERNAL "Dlls list required")

set(libraries "")

# Commons headers
set(HC_WINDOW_HEADERS
        include/Window/Core.h
        include/Window/Window.h
        include/Window/WindowFactory.h
)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Sources
set(HC_WINDOW_SOURCES
        src/WindowFactory.cpp
)

# Setting the library type
if(HC_WINDOW_LINKING STREQUAL "SHARED")
    set(HC_WINDOW_LIB_TYPE SHARED)
else()
    set(HC_WINDOW_LIB_TYPE STATIC)
endif()

# Setting the sources according to the backend
if(HC_WINDOW_BACKEND STREQUAL "GLFW")
    list(APPEND HC_WINDOW_SOURCES src/GLFW/GLFWWindow.cpp)
elseif(HC_WINDOW_BACKEND STREQUAL "SDL")
    list(APPEND HC_WINDOW_SOURCES src/SDL/SDLWindow.cpp)
elseif(HC_WINDOW_BACKEND STREQUAL "WIN32")
    list(APPEND HC_WINDOW_SOURCES src/Win32/Win32Window.cpp)
else()
    message(FATAL_ERROR "invalid backend: ${HC_WINDOW_BACKEND}")
endif()

# Library creation
add_library(HC-Window ${HC_WINDOW_LIB_TYPE}
        ${HC_WINDOW_SOURCES}
        ${HC_WINDOW_HEADERS}
)
add_library(HC::Window ALIAS HC-Window)

if(HC_WINDOW_LINKING STREQUAL "SHARED" AND WIN32)
    list(APPEND HC_WINDOW_DLLS $<TARGET_FILE:HC-Window>)
    set(HC_WINDOW_DLLS ${HC_WINDOW_DLLS} CACHE INTERNAL "Dlls used by HC-Window" FORCE)
endif()

if(HC_WINDOW_LINKING STREQUAL "SHARED")
    target_compile_definitions(HC-Window PRIVATE HC_WINDOW_BUILD_DLL)
else()
    target_compile_definitions(HC-Window PUBLIC HC_WINDOW_STATIC)
endif()

# Backend
include(FetchContent)

if(HC_WINDOW_BACKEND STREQUAL "GLFW")
    target_compile_definitions(HC-Window PUBLIC HC_WINDOW_BACKEND_GLFW)
    FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG 3.3.8
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
    list(APPEND libraries glfw)

elseif(HC_WINDOW_BACKEND STREQUAL "SDL")
    target_compile_definitions(HC-Window PUBLIC HC_WINDOW_BACKEND_SDL)
    set(SDL_TEST FALSE CACHE BOOL "Build SDL tests" FORCE)
    set(SDL_SHARED TRUE CACHE BOOL "Build shared library" FORCE)
    FetchContent_Declare(
            SDL2
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-2.26.3
    )
    FetchContent_MakeAvailable(SDL2)
    if(WIN32)
        list(APPEND libraries SDL2::SDL2 SDL2::SDL2main)
        add_custom_command(TARGET HC-Window POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2::SDL2>
                $<TARGET_FILE_DIR:HC-Window>
        )
    else()
        list(APPEND libraries SDL2::SDL2)
    endif()

elseif(HC_WINDOW_BACKEND STREQUAL "WIN32")
    # TODO
endif()

# Graphic API
if(HC_GRAPHIC_API STREQUAL "OPENGL")
    list(APPEND libraries glad)
    target_compile_definitions(HC-Window PUBLIC HC_GRAPHIC_API_OPENGL)
else()
    message(FATAL_ERROR "Graphic API not supported: ${HC_GRAPHIC_API}")
endif()

# math lib
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e
)
FetchContent_MakeAvailable(glm)
list(APPEND libraries glm::glm)

# IMGUI
if(HC_USE_IMGUI)
    add_subdirectory(thirdparty/imgui-docking)
    list(APPEND libraries imgui)
    target_compile_definitions(HC-Window PUBLIC HC_USE_IMGUI)
endif()

# HC-Asserts
list(APPEND libraries HC::Assert)

list(APPEND libraries HC::Graphics)

# Link everything
target_link_libraries(HC-Window PUBLIC ${libraries})

# include paths
target_include_directories(HC-Window
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Installation
install(TARGETS HC-Window
        EXPORT HC-WindowTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

if(HC_WINDOW_BACKEND STREQUAL "SDL" AND WIN32)
    list(APPEND HC_WINDOW_DLLS $<TARGET_FILE:SDL2::SDL2>)
    set(HC_WINDOW_DLLS ${HC_WINDOW_DLLS} CACHE INTERNAL "Dlls list" FORCE)
endif()

set(HC_WINDOW_DLLS ${HC_WINDOW_DLLS} PARENT_SCOPE)

# Examples
if(HC_WINDOW_BUILD_EXAMPLES)
    add_custom_command(TARGET HC-Window POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/examples
    )
    if(HC_WINDOW_BACKEND STREQUAL "SDL" AND WIN32)
        add_custom_command(TARGET HC-Window POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2::SDL2>
                ${CMAKE_BINARY_DIR}/examples
        )
    endif()
    add_subdirectory(examples)
endif()
