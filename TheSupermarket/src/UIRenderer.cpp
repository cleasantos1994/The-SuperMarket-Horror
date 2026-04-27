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
}

void UIRenderer::DrawMainMenu(int hoveredItem) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1);
}

void UIRenderer::DrawSettingsMenu(GameStateData& state) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0.1f, 0.1f, 0.1f, 1);
}

void UIRenderer::DrawHUD(int fear, const std::string& activeTask, bool nearInteractable, const std::string& interactHint, float playTime, bool hasKeys, bool antoniChasing) {
    RenderQuad(50, (float)screenH_ - 100, (float)fear * 2.0f, 20, 1, 0, 0, 0.8f);
}

void UIRenderer::DrawThreatIndicator(float intensity, float angle) {
    // Red borders or directional indicator
}

void UIRenderer::SetUIScale(float scale) {
    // Logic for dynamic scaling
}

void UIRenderer::DrawPauseMenu(int hoveredItem) {
    DrawFade(0.5f, 0, 0, 0);
}

void UIRenderer::DrawJumpscare(float intensity) {
    DrawFade(intensity, 1, 0, 0);
}

void UIRenderer::DrawGameOver(int deaths, float survivalTime) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1);
}

void UIRenderer::DrawWinScreen(float clearTime, int deaths) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 1, 1, 1, 1);
}

void UIRenderer::DrawCredits(float scrollY) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1);
}

void UIRenderer::DrawCutsceneDay1(float elapsed) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1);
}
void UIRenderer::DrawPoopEvent1(float elapsed) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0.36f, 0.23f, 0.12f, 1);
}
void UIRenderer::DrawCutsceneDay5(float elapsed) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1);
}
void UIRenderer::DrawPoopEvent2(float elapsed) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0.36f, 0.23f, 0.12f, 1);
}
void UIRenderer::DrawBabyOilCutscene(float elapsed) {
    DrawFade(0.5f, 1, 1, 0.8f);
}
void UIRenderer::DrawChaseCutscene(float elapsed) {
    DrawFade(1.0f, 0, 0, 0);
}

void UIRenderer::DrawFade(float alpha, float r, float g, float b) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, r, g, b, alpha);
}
void UIRenderer::DrawSubtitle(const std::string& text, float alpha, float y) {}
void UIRenderer::DrawTypewriter(const std::string& text, float elapsed, float charsPerSec, float x, float y, float scale) {}

void UIRenderer::RenderText(const std::string& text, float x, float y, float scale, float r, float g, float b, float a) {}
void UIRenderer::RenderQuad(float x, float y, float w, float h, float r, float g, float b, float a, unsigned int texID) {
    glBegin(GL_QUADS);
    glColor4f(r, g, b, a);
    glVertex2f(x / screenW_ * 2 - 1, y / screenH_ * 2 - 1);
    glVertex2f((x + w) / screenW_ * 2 - 1, y / screenH_ * 2 - 1);
    glVertex2f((x + w) / screenW_ * 2 - 1, (y + h) / screenH_ * 2 - 1);
    glVertex2f(x / screenW_ * 2 - 1, (y + h) / screenH_ * 2 - 1);
    glEnd();
}

void UIRenderer::InitQuad() {}
void UIRenderer::InitFontAtlas() {}
unsigned int UIRenderer::LoadTex(const char* path) { return 0; }
