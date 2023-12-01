#ifndef SUIGUIAPP_ANIMATION_HPP
#define SUIGUIAPP_ANIMATION_HPP

#include "GraphicsSystem.hpp"
#include "Config.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

std::vector<ImVec2> EnemyPreviousPosList;

int CatPicIndex = 0;
auto CatTimmer = 0;

class Animation {
private:

public:
    class NoAnimation {
    public:
        static void Update(const float* x, const float* y, const char* imagePath, int iOffset = 0) {
            GraphicsSystem::DrawImage(*x, *y, imagePath, iOffset);
        }
    };
    class CatAnimation {
    public:
        float dt = 1.f / 60.f;
        void Update(const float* x, const float* y, std::vector<std::string> image_list, int iOffset = 0) {
            auto CatTimmerStart = std::chrono::system_clock::now();
            GraphicsSystem::DrawImage(*x, *y, image_list.at(CatPicIndex).c_str(), iOffset);
            auto CatTimmerEnd = std::chrono::system_clock::now();
            auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(CatTimmerEnd - CatTimmerStart);
            CatTimmer = CatTimmer + Duration.count();
            if (CatTimmer >= 2500) {CatPicIndex += 1; CatTimmer = 0;}
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

};

Animation AnimationObj;

#endif //SUIGUIAPP_ANIMATION_HPP