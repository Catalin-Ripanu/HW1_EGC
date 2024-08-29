#pragma once

#include "components/simple_scene.h"
#include <chrono>


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        glm::mat3 modelMatrix1;
        glm::mat3 modelMatrix2;
        float translateX, translateY;
        float angularStepWing1;
        float angularStepWing0;
        bool verif0;
        int cont0;
        bool verif1;
        int cont1;
        int randX;
        int resX;
        int resY;
        float randAng;
        float neckX;
        float neckY;
        float wing0X;
        float wing0Y;
        float wing1X;
        float wing1Y;
        float scoreX;
        float scoreY;
        float scaleScoreX;
        float bulletX;
        float bulletY;
        bool firstTime;
        float dirX;
        float dirY;
        float inv;
        float speed;
        int bulCnt;
        int heartCnt;
        bool escape;
        bool killed;
        bool gameOver;
        int numKills;
        glm::vec3 recA;
        glm::vec3 recB;
        glm::vec3 recC;
        glm::vec3 recD;
        bool calc;
        bool helper;
        std::chrono::steady_clock::time_point begin;
        std::chrono::steady_clock::time_point end;
        int lucky;
    };
}
