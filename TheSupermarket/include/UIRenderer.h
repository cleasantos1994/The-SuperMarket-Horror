#pragma once
#include <string>
#include "GameState.h"

class UIRenderer {
public:
    bool Init(int screenW, int screenH);
    void Shutdown();

    void DrawLoadingScreen(float progress, const std::string& statusLine);
    void DrawMainMenu(int hoveredItem);
    void DrawSettingsMenu(GameStateData& state);

    void DrawCutsceneDay1(float elapsed);
    void DrawPoopEvent1(float elapsed);
    void DrawCutsceneDay5(float elapsed);
    void DrawPoopEvent2(float elapsed);
    void DrawBabyOilCutscene(float elapsed);
    void DrawChaseCutscene(float elapsed);

    void DrawHUD(int fear, const std::string& activeTask,
                 bool nearInteractable, const std::string& interactHint,
                 float playTime, bool hasKeys, bool antoniChasing);
    void DrawThreatIndicator(float intensity, float angle);
    void SetUIScale(float scale);
    void DrawPauseMenu(int hoveredItem);

    void DrawJumpscare(float intensity);
    void DrawGameOver(int deaths, float survivalTime);
    void DrawWinScreen(float clearTime, int deaths);
    void DrawCredits(float scrollY);

    void DrawFade(float alpha, float r=0,float g=0,float b=0);
    void DrawSubtitle(const std::string& text, float alpha, float y = 0.12f);
    void DrawTypewriter(const std::string& text, float elapsed,
                        float charsPerSec, float x, float y, float scale);
    void RenderText(const std::string& text, float x, float y,
                    float scale, float r, float g, float b, float a = 1.0f);
    void RenderQuad(float x, float y, float w, float h,
                    float r, float g, float b, float a, unsigned int texID = 0);

private:
    unsigned int quadVAO_, quadVBO_, shaderID_, textShaderID_, fontAtlasID_;
    unsigned int loadBgID_, menuBgID_, crosshairID_, fearBarID_, warningIconID_;
    int          screenW_, screenH_;
    unsigned int LoadTex(const char* path);
    void         InitQuad();
    void         InitFontAtlas();
};
