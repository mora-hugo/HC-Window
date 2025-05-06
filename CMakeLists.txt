cmake_minimum_required(VERSION 3.5)

project(HC-Window VERSION 0.1.0 LANGUAGES CXX)

set(HC_WINDOW_LINKING "STATIC" CACHE STRING "Linking type (STATIC ou SHARED)")
set_property(CACHE HC_WINDOW_LINKING PROPERTY STRINGS STATIC SHARED)

set(HC_WINDOW_BACKEND "GLFW" CACHE STRING "Backend to use (GLFW, SDL, WIN32)")
set_property(CACHE HC_WINDOW_BACKEND PROPERTY STRINGS GLFW SDL WIN32)

set(HC_WINDOW_BUILD_EXAMPLES "" CACHE BOOL "Build examples")

# DLLS
set(HC_WINDOW_DLLS "" CACHE INTERNAL "Dlls list required")

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
    message(FATAL_ERROR "Backend invalide: ${HC_WINDOW_BACKEND}")
endif()

# Library creation
add_library(HC-Window ${HC_WINDOW_LIB_TYPE} ${HC_WINDOW_SOURCES} ${HC_WINDOW_HEADERS})
add_library(HC::Window ALIAS HC-Window)


if(HC_WINDOW_LINKING STREQUAL "SHARED" AND WIN32)
    # Adding the DLL to the list of required DLLs
    list(APPEND HC_WINDOW_DLLS $<TARGET_FILE:HC-Window>)
    set(HC_WINDOW_DLLS ${HC_WINDOW_DLLS} CACHE INTERNAL "Dlls used by HC-Window" FORCE)
endif()

# Linking the library
if(HC_WINDOW_LINKING STREQUAL "SHARED")
    target_compile_definitions(HC-Window
            PRIVATE HC_WINDOW_BUILD_DLL
    )
else()
    target_compile_definitions(HC-Window
            PUBLIC HC_WINDOW_STATIC
    )
endif()


set(libraries "")

# Linking the backend
include(FetchContent)


if(HC_WINDOW_BACKEND STREQUAL "GLFW")
    target_compile_definitions(HC-Window PUBLIC HC_WINDOW_BACKEND_GLFW)

    # Fetch GLFW from GitHub
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

    # Fetch SDL2
    set(SDL_TEST FALSE CACHE BOOL "Build SDL tests" FORCE)
    set(SDL_SHARED TRUE CACHE BOOL "Build shared library" FORCE)

    FetchContent_Declare(
            SDL2
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-2.26.3
    )

    FetchContent_MakeAvailable(SDL2)

    # SDL dependencies
    if(WIN32)
        list(APPEND libraries SDL2::SDL2 SDL2::SDL2main)
        # Copy SDL2.dll to the output directory
        add_custom_command(TARGET HC-Window POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2::SDL2>
                $<TARGET_FILE_DIR:HC-Window>
        )
    else()
        list(APPEND libraries SDL2::SDL2)
    endif()

elseif(HC_WINDOW_BACKEND STREQUAL "WIN32")
   #TODO
endif()

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)
list(APPEND libraries glm::glm)


target_link_libraries(HC-Window
        PUBLIC
        ${libraries})

# includes paths
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
    # add SDL2.dll to the list of required DLLs
    list(APPEND HC_WINDOW_DLLS $<TARGET_FILE:SDL2::SDL2>)
    set(HC_WINDOW_DLLS ${HC_WINDOW_DLLS} CACHE INTERNAL "Dlls list" FORCE)

endif()

# According to the parent scope
set(HC_WINDOW_DLLS ${HC_WINDOW_DLLS} PARENT_SCOPE)

# Examples
if(HC_WINDOW_BUILD_EXAMPLES)
    if(HC_WINDOW_BACKEND STREQUAL "SDL" AND WIN32)
        add_custom_command(TARGET HC-Window POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                ${CMAKE_BINARY_DIR}/examples
        )
        add_custom_command(TARGET HC-Window POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2::SDL2>
                ${CMAKE_BINARY_DIR}/examples
        )
    endif()

    add_subdirectory(examples)
endif()