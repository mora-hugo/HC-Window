#pragma once

#ifdef _WIN32
    /* On windows we need to define if we are using a DLL or a static library */
    #ifdef HC_WINDOW_STATIC
        /* Static linking : no exportation/importation needed */
        #define HC_WINDOW_API
    #else
        /* Dynamic linking linking (DLL) */
        #ifdef HC_WINDOW_BUILD_DLL
            #define HC_WINDOW_API __declspec(dllexport)
        #else
            #define HC_WINDOW_API __declspec(dllimport)
        #endif
    #endif
#else
    /* On Linux/macOS, there is no need to define anything */
    #define HC_WINDOW_API
#endif

#include <cstdint>
using WindowHandle = uintptr_t;