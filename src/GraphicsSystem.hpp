#ifndef SUIGUIAPP_GRAPHICSSYSTEM_HPP
#define SUIGUIAPP_GRAPHICSSYSTEM_HPP


#include <GL/gl.h>
#include "imgui_internal.h"
#include "stb_image.h"
#include "Config.h"
#include <filesystem>
#include <vector>
#include <random>

//std::mt19937 rng;

std::filesystem::path ResourcePath = std::filesystem::current_path();

short back[10][20];
bool GeneralRandomBacklist = TRUE;

class GraphicsSystem {
public:
    static bool LoadTextureFromFile(const char* filename, GLuint *out_texture, int *out_width, int *out_height, float scale = 1)
    {
        // Load from file
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, nullptr, 4);
        if (image_data == nullptr)
            return false;
        image_width =(int)((float)image_width * scale);
        image_height = (int)((float)image_height * scale);

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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

    static void DrawImage(float x, float y, const char* File, int iOffset = 1){
        ImGuiWindow*  window = ImGui::GetCurrentWindow();
        GLuint Image;
        int weidth = 0;
        int height = 0;

        LoadTextureFromFile(File, &Image, &weidth, &height);
        window -> DrawList ->AddImage((void*)(intptr_t)Image, ImVec2(x, y), ImVec2(x + (weidth * (iScale + iOffset)), y + (height * (iScale + iOffset))));

    }

    static void DrawBackground(std::vector<std::string> *FileList, int iOffset = 1){
        ImGuiStyle& style = ImGui::GetStyle();
        ImGuiWindow*  window = ImGui::GetCurrentWindow();
        std::vector<GLuint> Images;
        std::vector<int> ImageHeights;
        std::vector<int> ImageWidths;

        style.WindowPadding = ImVec2(0.f, 0.f);
        style.ItemSpacing = ImVec2(0.f, 0.f);

        for (size_t i = 0; i < FileList->size(); i ++){
            if (i != FileList->size()){
                Images.push_back(0);
                ImageWidths.push_back(0);
                ImageHeights.push_back(0);
            }

            LoadTextureFromFile(FileList->at(i).c_str(), &Images.at(i), &ImageWidths.at(i), &ImageHeights.at(i));
        }

//        if (ImGui::Button("ReGeneralBackground")) GeneralRandomBacklist = !GeneralRandomBacklist;

        if (GeneralRandomBacklist){
            for (int y = 0; y < 10; y += 1){
                for (int x = 0; x < 20; x += 1){
                    back[y][x] = std::rand() % 3;
                }
            }
            GeneralRandomBacklist = FALSE;
        }
        int Display_x = 0;
        int Display_y = 0;

        for (int y = 0; y < ScreenHeight; y += 160){
            for (int x = 0; x < ScreenWidth; x += 96){
                for (size_t i = 0; i < Images.size(); i ++){
                    window -> DrawList ->AddImage((void*)(intptr_t)Images.at(back[Display_y][Display_x]), ImVec2((float)x, (float)y), ImVec2((float)x + (float)ImageWidths.at(i), (float)y + (float)ImageHeights.at(i)));
                }
                Display_x++;
            }
            Display_y++;
        }
    }
};


#endif //SUIGUIAPP_GRAPHICSSYSTEM_HPP