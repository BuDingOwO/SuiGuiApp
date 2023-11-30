#ifndef SUIGUIAPP_ANIMATION_HPP
#define SUIGUIAPP_ANIMATION_HPP

#include "GraphicsSystem.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

class Animation {
public:
    class NoAnimation {
    public:
        static void Update(float* x, float* y, const char* imagePath, int iOffset = 0) {
            GraphicsSystem::DrawImage(*x, *y, imagePath, iOffset);
        }
    };
    class CatAnimation {
    public:
        static void Update(float* x, float* y, const char* imagePath, int iOffset = 0) {
            GraphicsSystem::DrawImage(*x, *y, imagePath, iOffset);
        }
    };
    class BackGroundAnimation {
    public:
        static void Update(std::vector<std::string> FileList, int iOffset = 0) {
            GraphicsSystem::DrawBackground(&FileList, iOffset);
        }
    };

    class [[maybe_unused]] MovingAnimation {
    public:
        float startX, startY;  // 起始位置
        float endX, endY;      // 终点位置
        float duration;        // 动画持续时间
        float currentTime;     // 当前动画时间
        char* FilePath;        // 图片文件路径
        int iOffset;          // 图片缩放偏移

        void Update(float deltaTime) {
            currentTime += deltaTime;

            // 计算插值因子（通常是一个介于 0 到 1 之间的值）
            float t = currentTime / duration;
            t = (t > 1.0f) ? 1.0f : t;  // 确保 t 不会超过 1

            // 使用缓动函数（比如线性插值）计算当前位置
            float interpolatedX = Lerp(startX, endX, t);
            float interpolatedY = Lerp(startY, endY, t);

            // 更新物体的位置
            UpdateObjectPosition(interpolatedX, interpolatedY, FilePath, iOffset);

            // 判断动画是否完成
            if (currentTime >= duration) {
                // 动画完成后的处理
                HandleAnimationComplete();
            }
        }

        // 线性插值函数
        static float Lerp(float a, float b, float t) {
            return a + t * (b - a);
        }

        // 更新物体的位置的函数，可以根据具体情况自定义
        static void UpdateObjectPosition(float x, float y, const char* FilePath, int iOffset) {
            // 更新物体的位置
            // 例如：object.SetPosition(x, y);
            GraphicsSystem::DrawImage(x, y, FilePath, iOffset);
        }

        // 动画完成后的处理，可以根据具体情况自定义
        void HandleAnimationComplete() {
            // 动画完成后的操作
            // 例如：object.OnAnimationComplete();
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

#endif //SUIGUIAPP_ANIMATION_HPP