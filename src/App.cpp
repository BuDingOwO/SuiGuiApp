#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include <cstdio>
#include <math.h>  
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>
#include <chrono>
#include "Begin.h"
#include "stb_image.h"
#include "Config.h"

#define PI 3.141592

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


// Main code
int main( int argc, char** argv )
{
    if ( !glfwInit() )
        return 1;

    const char * glsl_version = "#version 130";

    // 隐藏标题栏和边框
//    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    GLFWwindow * window = glfwCreateWindow(ScreenWidth, ScreenHeight, "SuiApp", NULL, NULL);

    if ( window == NULL )
        // 创建窗口失败
        return 1;

    // 获取屏幕分辨率
    int cx, cy;

    int monitorCount;
    GLFWmonitor** pMonitor = glfwGetMonitors(&monitorCount);
    for (int i = 0; i < monitorCount; i++){
        const GLFWvidmode * mode = glfwGetVideoMode(pMonitor[i]);
        cx = mode -> width;
        cy = mode -> height;
    }
    // 设置窗口居中
    glfwSetWindowPos(window, cx / 2 - ScreenWidth / 2, cy /2 - ScreenHeight / 2);


//    glfwSetKeyCallback(window, KeyCallBack);
    glfwMakeContextCurrent( window );
    glfwSwapInterval(1); // Enable vsync

    ImGui::CreateContext();


    
    ImGui_ImplGlfw_InitForOpenGL( window, true );
    ImGui_ImplOpenGL3_Init(glsl_version);

    FrameInit();
    // Init

    while ( !glfwWindowShouldClose( window ) )
    {


        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();



        ImGui::NewFrame();

        FrameRender();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}