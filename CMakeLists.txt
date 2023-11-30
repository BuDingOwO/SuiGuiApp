cmake_minimum_required(VERSION 3.24)
project(SuiGuiApp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        libs/glfw/include
        libs/imgui
        libs/imgui/backends
#        libs/libpng/inc
        libs/stbimage/lib
        libs/hanim
)
link_directories(
        libs
        libs/gcc
        libs/glfw/lib
#        libs/libpng/lib

)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

file(GLOB_RECURSE IMGUI_SRCS libs/imgui/*.cpp)
file(GLOB_RECURSE LIBPNG_SRCS libs/libpng/inc/*.c)


add_executable(${PROJECT_NAME} # WIN32
        src/App.cpp
        src/Utils.cpp
        src/Begin.cpp
#        src/logo.rc
        ${IMGUI_SRCS}
#        ${LIBPNG_SRCS}
        src/Animation.hpp
        src/GraphicsSystem.hpp
        src/Config.h
        src/Config.cpp
)

# 添加WIN32参数就不会显示控制台界面了
target_link_libraries(${PROJECT_NAME} PRIVATE
#        libpng16.lib
        glfw3.a
#        zlib
        opengl32
        -static-libgcc
        -static-libstdc++
        libwinpthread.a
        -lpthread
        -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
)

# https://blog.csdn.net/soralaro/article/details/88708353
# https://blog.csdn.net/qq_29022265/article/details/117509682

#if (WIN32)
#    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
#    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ./res/icon.ico)
#endif()