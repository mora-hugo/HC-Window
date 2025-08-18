#include <imgui_internal.h>
#include <Window/WindowFactory.h>
#include <iostream>
int main() {
    auto window = HC::Window::WindowFactory::CreateWindow(
            HC::Window::WindowParams{ {800, 600}, "HC-Window Example2" }
    );

    while (window->IsOpen()) {
        window->PollEvents();
#if HC_USE_IMGUI
    window->BeforeIMGUIRendering();

    ImGui::ShowDemoWindow();

    window->AfterIMGUIRendering();
#endif



        window->SwapBuffers();
    }
    return 0;
}