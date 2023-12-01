#ifndef SUIGUIAPP_ANIMATION_HPP
#define SUIGUIAPP_ANIMATION_HPP

#include "GraphicsSystem.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

std::vector<ImVec2> EnemyPreviousPosList;

int CatPicIndex = 0;

class Animation {
private:

public:
    class NoAnimation {
    public:
        static void Update(float* x, float* y, const char* imagePath, int iOffset = 0) {
            GraphicsSystem::DrawImage(*x, *y, imagePath, iOffset);
        }
    };
    class CatAnimation {
    public:
        float dt = 1.f / 60.f;
        void Update(const float* x, const float* y, std::vector<std::string> image_list, int iOffset = 0) {
            GraphicsSystem::DrawImage(*x, *y, image_list.at(CatPicIndex).c_str(), iOffset);
            CatPicIndex += 1;
            if (CatPicIndex == image_list.size()) CatPicIndex = 0;
        }
    };
    class BackGroundAnimation {
    public:
        static void Update(std::vector<std::string> FileList, int iOffset = 0) {
            GraphicsSystem::DrawBackground(&FileList, iOffset);
        }
    };

    class MovingAnimation {
    private:
        ImVec2 CurrentPos;

    public:
//        Animation& animation; // 引用外部类的实例
//        explicit MovingAnimation(Animation& anim) : animation(anim) {}
        float dt = 1.f / 5.0f;
        ImVec2 EmptyVec;
        void Update(ImVec2 Pos, ImVec2 Vel,int index , const char* Image, int iScaleOffset = 0) {
            if (EnemyPreviousPosList.size() <= index){
                for (short i; i <= index + 1; i++)
                EnemyPreviousPosList.push_back(EmptyVec);
            }
            if (EnemyPreviousPosList[index].x == 0 && EnemyPreviousPosList[index].y == 0){
                CurrentPos = Pos;
                CurrentPos.x = Pos.x + Vel.x * dt;
                CurrentPos.y = Pos.y + Vel.y * dt;
            }else{
                CurrentPos.x = EnemyPreviousPosList[index].x + Vel.x * dt;
                CurrentPos.y = EnemyPreviousPosList[index].y + Vel.y * dt;
            }
            EnemyPreviousPosList[index] = CurrentPos;
            GraphicsSystem::DrawImage(CurrentPos.x, CurrentPos.y, Image, iScaleOffset);
        }

        void DefEntityList(int Counter){
            if (EnemyPreviousPosList.size() < Counter){
                for (short i; i <= Counter; i++)
                    EnemyPreviousPosList.push_back(EmptyVec);
            }
        }
    };

    class SwitcherAnimation {
    public:
        SwitcherAnimation(const std::vector<std::string>& imagePaths, float in_x = 0, float in_y = 0, int in_iOffset = 0) : images(imagePaths), currentIndex(0), isRunning(false) {
            this->y = in_y;
            this->iOffset = in_iOffset;
            this->x = in_x;
        }

        ~SwitcherAnimation() {
            Stop();
        }

        void Start() {
            if (!isRunning) {
                isRunning = true;
                animationThread = std::thread(&SwitcherAnimation::Run, this);

//                std::cout << "Start animation " << images[currentIndex] << " at " << x << ", " << y << "\n";
//                animationThread = std::thread([this]() { Run(); });
            }
        }

        void Stop() {
            if (isRunning) {
                isRunning = false;
                if (animationThread.joinable()) {
                    animationThread.join();
                }
            }
        }

    private:
        std::vector<std::string> images;
        size_t currentIndex;
        std::thread animationThread;
        bool isRunning;
        float x, y;
        int iOffset;

        void Run() {
            while (isRunning) {
                // 渲染当前图片，这里用输出代替真实的渲染操作
                RenderImage(images[currentIndex]);

                // 切换到下一张图片
                currentIndex = (currentIndex + 1) % images.size();

                // 等待1秒
                std::this_thread::sleep_for(std::chrono::seconds (1));
            }
        }

        void RenderImage(const std::string& imagePath) const {
            // 这里用输出代替真实的渲染操作
            std::cout << "Rendering image: " << imagePath << " at " << x << ", " << y << "\n";
            GraphicsSystem::DrawImage(x, y, imagePath.c_str(), iOffset);
        }


    };

};

Animation AnimationObj;

#endif //SUIGUIAPP_ANIMATION_HPP