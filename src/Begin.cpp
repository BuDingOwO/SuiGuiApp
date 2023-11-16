#include "imgui.h"
#include <iostream>
#include <GL/gl.h>
#include "png.h"
#include "imgui_internal.h"

#define IMGUI_DEFINE_MATH_OPERATORS

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "GLFW/glfw3.h"

float iCatPosY = (float)720/2;
float iCatPosX = (float)1280/2;
//bool bCatFloating = TRUE;
float fCatDropSpeed = 0.5f;
float fDefaultCatDropSpeed = 0.5f;
bool bCatOnBlock = TRUE;
bool bCatCanGoLeft = TRUE;
bool bCatCanGoRight = TRUE;
bool bCatCanGoDown = FALSE;

// 点与直线的碰撞检测函数
bool pointLineCollision(float px, float py, float x1, float y1, float x2, float y2) {
    // 计算直线上的投影点
    float qx = x1 + ((px - x1) * (x2 - x1) + (py - y1) * (y2 - y1)) /
                    ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) * (x2 - x1);

    float qy = y1 + ((px - x1) * (x2 - x1) + (py - y1) * (y2 - y1)) /
                    ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) * (y2 - y1);

    // 检测点是否在直线上（投影点在直线上，且投影点在直线段的范围内）
    if ((qx >= x1 && qx <= x2) || (qx >= x2 && qx <= x1)) {
        if ((qy >= y1 && qy <= y2) || (qy >= y2 && qy <= y1)) {
            return true;
        }
    }

    return false;
}

void Jump(){
    if (bCatOnBlock){
        iCatPosY -= 50;
    }
}

void GoLeft(float* x){
    if (bCatCanGoLeft){
        *x -= 5;
    }
}


void GoRight(float* x){
    if (bCatCanGoRight){
        *x += 5;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_A:
                GoLeft(&iCatPosX);
                break;
            case GLFW_KEY_S:
                iCatPosY+=5;
                break;
            case GLFW_KEY_D:
                GoRight(&iCatPosX);
                break;
            default:
                break;
        }
    }
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_SPACE){
            Jump();
        }
    }
    if (action == GLFW_REPEAT) {
        if (key == GLFW_KEY_SPACE){
            if(iCatPosY==700.5){
                Jump();
                fCatDropSpeed = 0.1f;
            }else{
                fCatDropSpeed = 0.5f;
            }
        }
    }
}

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

void DrawBackground() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow*  window = ImGui::GetCurrentWindow();
    GLuint Image;
    int weidth = 0;
    int height = 0;

    style.WindowPadding = ImVec2(0.f, 0.f);
    style.ItemSpacing = ImVec2(0.f, 0.f);

    LoadTextureFromFile("../res/assets/assets/graphics/1x/background-299-0.png", &Image, &weidth, &height);

    for (short y = 0; y < 720; y += 160){
        for (short x = 0; x < 1280; x += 96){

//            ImGui::Image((void*)(intptr_t)Image, ImVec2((float)weidth, (float)height), ImVec2(x, y));ImGui::SameLine();
            window -> DrawList ->AddImage((void*)(intptr_t)Image, ImVec2(x, y), ImVec2((float)x + (float)weidth, (float)y + (float)height));
        }
        ImGui::Text("");
    }
}

void DrawCatBird(){
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow*  window = ImGui::GetCurrentWindow();
    GLuint Image;
    int width = 0;
    int height = 0;
    style.WindowPadding = ImVec2(0.f, 0.f);
    style.ItemSpacing = ImVec2(0.f, 0.f);
    LoadTextureFromFile("../res/assets/assets/graphics/1x/sprite-4-3.png", &Image, &width, &height);


    ImVec2 image_min(iCatPosX, iCatPosY);
    ImVec2 image_max(((float)width / 3 + iCatPosX), ((float)height / 2 + iCatPosY));
    ImVec2 uv_min(0.0f, 0.0f);
    ImVec2 uv_max(0.33f, 0.5f);
    if (iCatPosX < 0) bCatCanGoLeft = FALSE;
    if (iCatPosY < 0) iCatPosY = 0;
    if (iCatPosX >= 1280) iCatPosX = 1280;
    if (iCatPosY >= 720){
        iCatPosY = 720;
        bCatOnBlock = TRUE;
    }else bCatOnBlock = FALSE;
    if (!bCatOnBlock) {
        fCatDropSpeed = fDefaultCatDropSpeed;
        iCatPosY += fCatDropSpeed;
    }else fCatDropSpeed = 0;
    window -> DrawList ->AddImage((void*)(intptr_t)Image, image_min, image_max, uv_min, uv_max);
}

void DrawBlock(int x, int y){
    ImGuiWindow*  window = ImGui::GetCurrentWindow();
    GLuint Image;
    int weidth = 0;
    int height = 0;

    LoadTextureFromFile("../res/assets/assets/graphics/1x/sprite-687-0.png", &Image, &weidth, &height);

    int BlockHead = x;
    int BlockTail = x + height;
    ImGui::Text("Block Y: %d, X: %d, FX: %d", y-height, x, x+weidth);
    if(ImGui::Button("Goto Y 350")) iCatPosY = 350;

    if (pointLineCollision(iCatPosX, iCatPosY, (float)x, (float)y - (float)height, (float)x + (float)weidth, (float)y - (float)height)){
        bCatOnBlock = TRUE;
    }else bCatOnBlock = FALSE;

    window -> DrawList ->AddImage((void*)(intptr_t)Image, ImVec2((float)x, (float)y), ImVec2((float)x + (float)weidth, (float)y + (float)height));
}

void BeginFrame()
{
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoResize;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowPos(viewport->Pos);
    bool p_open = true;

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 pos = ImGui::GetCursorScreenPos();


    ImGui::Begin("Window", &p_open, flags);
    DrawBackground();

//    ImGuiWindow*  window = ImGui::GetCurrentWindow();
//    window -> DrawList ->AddImage(&Image, ImVec2(x, y), ImVec2((float)weidth, (float)height));

    // WindowWidth = 1280
    // WindowHeight = 720

    ImGui::Text("CatX: %.6f", iCatPosX);
    ImGui::Text("CatY: %.6f", iCatPosY);
    ImGui::Text("bCatOnBlock: %s", bCatOnBlock ? "TRUE" : "FALSE");
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

    DrawBlock(1280/2, 720/2+50);

    DrawCatBird();



//    ImGui::Image((void*)(intptr_t)Image, ImVec2((float)weidth, (float)height), ImVec2(x, y));


    ImGui::End();

//    ImGui::ShowDemoWindow();
}