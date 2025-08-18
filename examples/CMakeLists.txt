add_executable(simple_window simple_window.cpp)
target_link_libraries(simple_window PRIVATE HC::Window)

set(ALL_REQUIRED_DLLS "")

if(DEFINED HC_WINDOW_DLLS)
    list(APPEND ALL_REQUIRED_DLLS ${HC_WINDOW_DLLS})
endif()

if(WIN32 AND HC_WINDOW_LINKING STREQUAL "SHARED")
    foreach(DLL_PATH ${ALL_REQUIRED_DLLS})
        add_custom_command(TARGET simple_window POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${DLL_PATH}
                $<TARGET_FILE_DIR:simple_window>
                COMMENT "Copying ${DLL_PATH}"
        )
    endforeach()
endif()

