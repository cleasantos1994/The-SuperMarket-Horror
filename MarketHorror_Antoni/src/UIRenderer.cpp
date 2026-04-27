#include "UIRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

bool UIRenderer::Init(int screenW, int screenH) {
    screenW_ = screenW; screenH_ = screenH;
    InitQuad();
    return true;
}

void UIRenderer::Shutdown() {}

void UIRenderer::DrawLoadingScreen(float progress, const std::string& statusLine) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0.05f, 0.05f, 0.05f, 1.0f);
    RenderText("SUPERMEME MARKET", (float)screenW_/2 - 100, (float)screenH_/2, 1.0f, 1, 1, 1);
    RenderQuad((float)screenW_/4, (float)screenH_/4, (screenW_/2) * progress, 20, 0, 1, 0, 1.0f);
    RenderText(statusLine, (float)screenW_/4, (float)screenH_/4 - 30, 0.5f, 0.8f, 0.8f, 0.8f);
}

void UIRenderer::DrawMainMenu(int hoveredItem) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1);
    RenderText("MARKET HORROR", 100, (float)screenH_ - 150, 2.0f, 1, 0, 0);
    RenderText("The Legend of Antoni", 120, (float)screenH_ - 200, 1.0f, 0.8f, 0, 0);
    
    std::string options[] = {"NEW GAME", "CONTINUE", "SETTINGS", "QUIT"};
    for (int i = 0; i < 4; i++) {
        float r = (i == hoveredItem) ? 1.0f : 0.5f;
        RenderText(options[i], 150, 400.0f - i * 50.0f, 1.0f, r, r, r);
    }
}

void UIRenderer::DrawSettingsMenu(GameStateData& state) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0.1f, 0.1f, 0.1f, 1);
    RenderText("SETTINGS", 100, (float)screenH_ - 100, 1.5f, 1, 1, 1);
}

void UIRenderer::DrawHUD(int fear, const std::string& activeTask, bool nearInteractable, const std::string& interactHint, float playTime, bool hasKeys, bool antoniChasing) {
    RenderText("FEAR", 50, (float)screenH_ - 50, 0.7f, 1, 1, 1);
    RenderQuad(120, (float)screenH_ - 55, fear * 2.0f, 20, 1, 0, 0, 0.8f);
    RenderText(activeTask, (float)screenW_ - 400, (float)screenH_ - 50, 0.7f, 1, 1, 1);
    RenderQuad((float)screenW_/2 - 2, (float)screenH_/2 - 2, 4, 4, 1, 1, 1, 1);
    if (nearInteractable) {
        RenderText(interactHint, (float)screenW_/2 - 100, (float)screenH_/2 - 50, 0.6f, 1, 1, 0);
    }
}

void UIRenderer::DrawPauseMenu(int hoveredItem) {
    DrawFade(0.5f, 0, 0, 0);
    RenderText("PAUSED", (float)screenW_/2 - 50, (float)screenH_/2 + 100, 1.5f, 1, 1, 1);
}

void UIRenderer::DrawJumpscare(float intensity) {
    DrawFade(intensity, 1, 0, 0);
}

void UIRenderer::DrawGameOver(int deaths, float survivalTime) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1);
    RenderText("HE GOT YOU", (float)screenW_/2 - 150, (float)screenH_/2 + 50, 2.0f, 1, 0, 0);
}

void UIRenderer::DrawWinScreen(float clearTime, int deaths) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 1, 1, 1, 1);
    RenderText("YOU ESCAPED", (float)screenW_/2 - 150, (float)screenH_/2 + 50, 2.0f, 1, 0.8f, 0);
}

void UIRenderer::DrawCredits(float scrollY) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1);
    RenderText("MARKET HORROR", (float)screenW_/2 - 100, scrollY, 1.5f, 1, 0, 0);
}

void UIRenderer::DrawCutsceneDay1(float elapsed) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1);
    RenderText("Day 1. A normal Tuesday.", (float)screenW_/2 - 200, (float)screenH_/2, 1.0f, 1, 1, 1);
}
void UIRenderer::DrawPoopEvent1(float elapsed) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0.36f, 0.23f, 0.12f, 1);
    RenderText("IT HAPPENED.", (float)screenW_/2 - 100, (float)screenH_/2, 1.5f, 1, 1, 1);
}
void UIRenderer::DrawCutsceneDay5(float elapsed) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1);
    RenderText("4 DAYS LATER", (float)screenW_/2 - 100, (float)screenH_/2, 1.2f, 1, 1, 1);
}
void UIRenderer::DrawPoopEvent2(float elapsed) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0.36f, 0.23f, 0.12f, 1);
    RenderText("NOT THE REGISTER!", (float)screenW_/2 - 150, (float)screenH_/2, 1.5f, 1, 1, 1);
}
void UIRenderer::DrawBabyOilCutscene(float elapsed) {
    DrawFade(0.5f, 1, 1, 0.8f);
    RenderText("BABY OIL...", (float)screenW_/2 - 100, (float)screenH_/2, 1.5f, 0, 0, 0);
}
void UIRenderer::DrawChaseCutscene(float elapsed) {
    DrawFade(1.0f, 0, 0, 0);
    RenderText("RUN", (float)screenW_/2 - 50, (float)screenH_/2, 3.0f, 1, 0, 0);
}

void UIRenderer::DrawFade(float alpha, float r, float g, float b) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, r, g, b, alpha);
}
void UIRenderer::DrawSubtitle(const std::string& text, float alpha, float y) {
    RenderText(text, (float)screenW_/2 - 100, (float)screenH_ * y, 0.8f, 1, 1, 1, alpha);
}
void UIRenderer::DrawTypewriter(const std::string& text, float elapsed, float charsPerSec, float x, float y, float scale) {
    size_t count = (size_t)(elapsed * charsPerSec);
    if (count > text.length()) count = text.length();
    RenderText(text.substr(0, count), x, y, scale, 1, 1, 1);
}

void UIRenderer::RenderText(const std::string& text, float x, float y, float scale, float r, float g, float b, float a) {
    // Basic text rendering placeholder
}
void UIRenderer::RenderQuad(float x, float y, float w, float h, float r, float g, float b, float a, unsigned int texID) {
    glBegin(GL_QUADS);
    glColor4f(r, g, b, a);
    glVertex2f(x / screenW_ * 2 - 1, y / screenH_ * 2 - 1);
    glVertex2f((x + w) / screenW_ * 2 - 1, y / screenH_ * 2 - 1);
    glVertex2f((x + w) / screenW_ * 2 - 1, (y + h) / screenH_ * 2 - 1);
    glVertex2f(x / screenW_ * 2 - 1, (y + h) / screenH_ * 2 - 1);
    glEnd();
}

void UIRenderer::InitQuad() {
    glGenVertexArrays(1, &quadVAO_);
    glGenBuffers(1, &quadVBO_);
}
void UIRenderer::InitFontAtlas() {}
unsigned int UIRenderer::LoadTex(const char* path) { return 0; }