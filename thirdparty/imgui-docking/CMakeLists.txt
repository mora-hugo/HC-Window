cmake_minimum_required(VERSION 3.1)
project(imgui)

add_library(imgui)
target_sources(imgui PRIVATE
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
)



if(HC_WINDOW_BACKEND STREQUAL "GLFW")
    target_sources(imgui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp")
elseif(HC_WINDOW_BACKEND STREQUAL "SDL")
    target_sources(imgui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl2.cpp")
elseif(HC_WINDOW_BACKEND STREQUAL "WIN32")
    target_sources(imgui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_win32.cpp")
else()
    message(FATAL_ERROR "invalid backend: ${HC_WINDOW_BACKEND}")
endif()

if(HC_GRAPHIC_API STREQUAL "OPENGL")
    target_sources(imgui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp")
elseif(HC_GRAPHIC_API STREQUAL "VULKAN")
    message(FATAL_ERROR "VULKAN is not supported yet")
endif ()
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui")


target_link_libraries(imgui PUBLIC glfw glad)