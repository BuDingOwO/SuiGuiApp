#include "imgui.h"
#include <iostream>
#include <GL/gl.h>
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "imgui_internal.h"
#include <string>
#include "Config.h"
#include "Animation.hpp"


struct Debug_{
    bool bDebug = false;
}Debug;

float fCatWidth = 15;

std::vector<ImVec2> BulletEntityList;
int BulletCounter = 0;
int EntityIndex = 0;


float iCatPosY = 560 - fCatWidth;
float iCatPosX = ((ScreenHeight / 2) - fCatWidth);

struct ResStruct{
    const char* Background1 = "../res/background/1.png";
    const char* Background2 = "../res/background/2.png";
    const char* Background3 = "../res/background/3.png";


    const char* Cat1 = "../res/cat/1.png";
    const char* Cat2 = "../res/cat/2.png";
    const char* Cat3 = "../res/cat/3.png";
    const char* Cat4 = "../res/cat/4.png";

    const char* Enemy = "../res/assets/assets/graphics/1x/sprite-50-0.png";
    const char* Block = "../res/assets/assets/graphics/1x/sprite-687-0.png";
    const char* Bullet = "../res/assets/assets/graphics/1x/sprite-513-0.png";
}Res;

struct Pos4{
    ImVec2 x;
    ImVec2 y;
    ImVec2 z;
    ImVec2 w;
};
struct iPosVec4{
    int x1;
    int y1;
    int x2;
    int y2;
};
struct iPosVec2{
    int x;
    int y;
};


//bool bCatFloating = TRUE;
float fDefaultCatDropSpeed = 0.5f;
bool bCatOnBlock = FALSE;
bool bCatCanGoLeft = TRUE;
bool bCatCanGoRight = TRUE;
bool bCatCanGoTop = TRUE;
bool bCatCanGoDown = TRUE;

float Velocity = 3; // 3px/s
float Gravity = 1; // 1px/s

int iTimes = 5;

bool bPointLineCollision(float px, float py, float x1, float y1, float x2, float y2){
    if (x2 >= x1){
        if (px <= x2 && px >= x1){
            if (y1 == y2){
                if (py == y1) return TRUE;
            }
        }
    }
    return FALSE;
}

bool bLineLineCollision(float px1, float py1, float px2, float py2, float x1, float y1, float x2, float y2){
    if (x2 >= x1){
        if (px1 <= x2 && px1 >= x1){
            if (y1 == y2){
                if (py1 == y1) return TRUE;
            }
        }
        if (px2 <= x2 && px2 >= x1){
            if (y1 == y2){
                if (py2 == y1) return TRUE;
            }
        }
    }
    return FALSE;
}

void WALK(float* x, bool mode){
    if (mode){
        // 向右
        if (bCatCanGoRight){
            *x += Velocity;
        }
    }else{
        // 向左
        if (bCatCanGoLeft){
            *x -= Velocity;
        }
    }
}
void FLY(float* y, bool mode){
    if (mode){
        //向下
        if (bCatCanGoDown) *y += Velocity;
    }else{
        // 向上
        if (bCatCanGoTop) *y -= Velocity;
    }
}

/*
void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
//    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
//        if (key == GLFW_KEY_A) WALK(&iCatPosX, FALSE);
//        if (key == GLFW_KEY_D) WALK(&iCatPosX, TRUE);
//    }
//    if (action == GLFW_PRESS) {
//        if (key == GLFW_KEY_SPACE) Jump();
//    }
//    if (action == GLFW_REPEAT) {
//        if (key == GLFW_KEY_SPACE){
//            if(bCatOnBlock){
//                Jump();
//                Gravity = 0.1f;
//            }else{
//                Gravity = 0.5f;
//            }
//        }
//    }
}
*/

void DrawCat(float *x, float *y, Pos4 MovingBox){
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow*  window = ImGui::GetCurrentWindow();

    style.WindowPadding = ImVec2(0.f, 0.f);
    style.ItemSpacing = ImVec2(0.f, 0.f);
    if (*x < MovingBox.x.x) bCatCanGoLeft = FALSE;
    else bCatCanGoLeft = TRUE; // |<

    if (*y <= MovingBox.x.y) bCatCanGoTop = FALSE;
    else bCatCanGoTop = TRUE; // -^-

    if (*x >= MovingBox.y.x - (fCatWidth * iScale)) bCatCanGoRight = FALSE;
    else bCatCanGoRight = TRUE; // >|

    if (*y >= MovingBox.z.y - (fCatWidth * iScale)) {bCatCanGoDown = FALSE;bCatOnBlock = TRUE;}
    else {bCatCanGoDown = TRUE;bCatOnBlock = FALSE;} // -v-


    if (!bCatOnBlock) {
        Gravity = fDefaultCatDropSpeed;
        *y += Gravity;
    }else Gravity = 0;

    Animation::NoAnimation::Update(x, y, Res.Cat1, 0);
}

void DrawEnemy(float x, float y) {
    ImVec2 Pos, Vel;
    Pos.x = x;
    Pos.y = y;

    Vel.x = 0.0f;
    Vel.y = 5.6f;

    int index = 0;

    Animation::MovingAnimation MA = Animation::MovingAnimation();
    MA.Update(Pos, Vel, index, Res.Enemy, 1);
//    Animation::MovingAnimation::Update(Pos, Vel, Res.Enemy, 1);
}

//void DrawBlock(int x, int y){
//    ImGuiWindow*  window = ImGui::GetCurrentWindow();
//    GLuint Image;
//    int weidth = 0;
//    int height = 0;
//
////    LoadTextureFromFile(Res.Block, &Image, &weidth, &height);
//
//    int BlockHead = x;
//    int BlockTail = x + weidth;
//    int BlockTopLine = y - height;
////    ImGui::Text("Block Y: %d, X: %d, FX: %d", y-height, x, x+weidth);
////    if(ImGui::Button("Goto Y 350")) iCatPosY = 350;
//
//    if (bLineLineCollision(iCatPosX, iCatPosY, iCatPosX + (fCatWidth * iScale), iCatPosY + (fCatWidth * iScale), (float)BlockHead, (float)BlockTopLine, (float)BlockTail, (float)BlockTopLine)){
//        bCatOnBlock = TRUE;
//        ImGui::Text("Collision");
//    }else bCatOnBlock = FALSE;
//
//    window -> DrawList ->AddImage((void*)(intptr_t)Image, ImVec2((float)x, (float)y), ImVec2((float)x + (float)weidth * iScale, (float)y + (float)height * iScale));
//}

void DrawMovingAreaSquare(){
    ImGuiWindow*  window = ImGui::GetCurrentWindow();
    int iAreaWidth = 1216;
    int iAreaHeight = 156;

    ImVec2 LeftTop = ImVec2(32, ScreenHeight - 32 - iAreaHeight);
    ImVec2 RightTop = ImVec2(32 + iAreaWidth, ScreenHeight - 32 - iAreaHeight);
    ImVec2 LeftBottom = ImVec2(32, ScreenHeight - 32);
    ImVec2 RightBottom = ImVec2(32 + iAreaWidth, ScreenHeight - 32);

//    iPosVec4 iLeftWALLPos = iPosVec4(LeftTop.x, LeftTop.y, LeftBottom.x, LeftBottom.y);
//    iPosVec4 iRightWALLPos = iPosVec4(RightTop.x, RightTop.y, RightBottom.x, RightBottom.y);
//    iPosVec4 iTopWALLPos = iPosVec4(LeftTop.x, LeftTop.y, RightTop.x, RightTop.y);
//    iPosVec4 iBottomWALLPos = iPosVec4(LeftBottom.x, LeftBottom.y, RightBottom.x, RightBottom.y);
    if (Debug.bDebug) {
        window->DrawList->AddQuad(LeftTop, RightTop, RightBottom, LeftBottom, IM_COL32(250, 250, 250, 250));
    }
    DrawCat(&iCatPosX, &iCatPosY, Pos4(LeftTop, RightTop, LeftBottom, RightBottom));
}

void DrawEnemyAreaSquare(){
    ImGuiWindow*  window = ImGui::GetCurrentWindow();
    int iAreaWidth = 1216;
    int iAreaHeight = 468;

    ImVec2 LeftTop = ImVec2(32,  32);
    ImVec2 RightTop = ImVec2(32 + iAreaWidth, 32);
    ImVec2 LeftBottom = ImVec2(32, iAreaHeight + 32);
    ImVec2 RightBottom = ImVec2(32 + iAreaWidth, iAreaHeight + 32);

//    iPosVec4 iLeftWALLPos = iPosVec4(LeftTop.x, LeftTop.y, LeftBottom.x, LeftBottom.y);
//    iPosVec4 iRightWALLPos = iPosVec4(RightTop.x, RightTop.y, RightBottom.x, RightBottom.y);
//    iPosVec4 iTopWALLPos = iPosVec4(LeftTop.x, LeftTop.y, RightTop.x, RightTop.y);
//    iPosVec4 iBottomWALLPos = iPosVec4(LeftBottom.x, LeftBottom.y, RightBottom.x, RightBottom.y);

    if (Debug.bDebug) {
        window -> DrawList ->AddQuad(LeftTop, RightTop, RightBottom, LeftBottom, IM_COL32(250, 250, 250, 250));
    }

    DrawEnemy(LeftTop.x + (RightTop.x / 2), LeftTop.y + (RightTop.y / 2));
}

void Fire(const float *x, const float *y){
    BulletCounter++;
    ImVec2 EmptyVec;
    for (size_t i; i < BulletCounter; i++){
        BulletEntityList.push_back(EmptyVec);
        ImVec2 Pos, Vel;
        Pos.x = *x;
        Pos.y = *y;

        Vel.x = 0.0f;
        Vel.y = -5.6f;

        int index = EntityIndex + BulletCounter + 2;

        Animation::MovingAnimation MA = Animation::MovingAnimation();
        MA.Update(Pos, Vel,index,  Res.Bullet, 0);
    }
}

void DrawBackground(){
    std::vector<std::string> l;
    l.emplace_back(Res.Background1);
    l.emplace_back(Res.Background2);
    l.emplace_back(Res.Background3);
    Animation::BackGroundAnimation::Update(l);
};

void MouseProc(){
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Mouse down:");
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++){
        if (ImGui::IsMouseDown(i)) {
            ImGui::SameLine();
            ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]);
        }
//        if (i == ImGuiMouseButton_Left){
//            Fire(&iCatPosX, &iCatPosY);
//        }
    }
}

void KeyProc(){
    struct funcs {
        static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; }
    }; // Hide Native<>ImGuiKey duplicates when both exists in the array
    auto start_key = (ImGuiKey)0;
    ImGui::Text("Keys down:");
    for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey) (key + 1)) {
        if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key))continue;
        ImGui::SameLine();
        ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key);
        if(ImGui::IsKeyDown(ImGuiKey_W)) FLY(&iCatPosY, FALSE);
        if(ImGui::IsKeyDown(ImGuiKey_S)) FLY(&iCatPosY, TRUE);
        if(ImGui::IsKeyDown(ImGuiKey_A)) WALK(&iCatPosX, FALSE);
        if(ImGui::IsKeyDown(ImGuiKey_D)) WALK(&iCatPosX, TRUE);
        if(ImGui::IsKeyDown(ImGuiKey_J)) Fire(&iCatPosX, &iCatPosY);
    }
}

void BeginFrame()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0;
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
//    ImGui::Text("bCatOnBlock: %s", bCatOnBlock ? "TRUE" : "FALSE");
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    ImGui::Checkbox("Debug", &Debug.bDebug);

//    if(ImGui::Button("Goto Y 200")) iCatPosY = 200;
//    DrawBlock(560, 460);
    MouseProc();
    KeyProc();


    DrawMovingAreaSquare();
    DrawEnemyAreaSquare();

    ImGui::End();

    ImGui::ShowDemoWindow();
}