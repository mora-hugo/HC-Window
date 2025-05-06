# HC-Window

A lightweight C++17 windowing library with selectable backends (GLFW, SDL2 or probably more later...).

Quick start:

```bash
git clone https://github.com/mora-hugo/HC-Window.git
cd HC-Window
mkdir build && cd build
cmake .. 
  -DHC_WINDOW_LINKING=STATIC    # Choose with STATIC or SHARED linking
  -DHC_WINDOW_BACKEND=GLFW      # Choose your backend (GLFW or SDL)
  -DHC_WINDOW_BUILD_EXAMPLES=ON # Build with the simple_window example if you need

cmake --build .
```