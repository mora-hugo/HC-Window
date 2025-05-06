#include <Window/WindowFactory.h>
#include <iostream>

int main() {
    auto window = HC::Window::WindowFactory::CreateWindow(
            HC::Window::WindowParams{ {800, 600}, "HC-Window Example2" }
    );

    while (window->IsOpen()) {
        window->PollEvents();


        window->SwapBuffers();
    }
    return 0;
}