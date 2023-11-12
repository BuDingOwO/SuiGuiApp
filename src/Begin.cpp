#include "imgui.h"
#include <iostream>
#include <GL/gl.h>
#include "png.h"
#include "imgui_internal.h"

#define IMGUI_DEFINE_MATH_OPERATORS

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint *out_texture, int *out_width, int *out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, nullptr, 4);
    if (image_data == nullptr)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Same

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

GLuint Image;
int weidth = 0;
int height = 0;

bool bLoaded;

void Init(){
    bLoaded =  LoadTextureFromFile(R"(C:\Users\ConBl\Desktop\SuiGuiApp\res\cat.png)", &Image, &weidth, &height);
}

void BeginFrame()
{
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoResize;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowPos(viewport->Pos);
    bool p_open = true;

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::Begin("Window", &p_open, flags);

    float x = 0, y = 0;

    ImGuiWindow*  window = ImGui::GetCurrentWindow();
    window -> DrawList ->AddImage(&Image, ImVec2(x, y), ImVec2((float)weidth, (float)height));

    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

    if (!bLoaded){
        ImGui::Text("Load Image Failed");
        if (ImGui::Button("Reload")){
            bLoaded =  LoadTextureFromFile(R"(C:\Users\ConBl\Desktop\SuiGuiApp\res\cat.png)", &Image, &weidth, &height);
        }
    }

    ImGui::Image((void*)(intptr_t)Image, ImVec2(510, 300));


    ImGui::End();

    ImGui::ShowDemoWindow();
}