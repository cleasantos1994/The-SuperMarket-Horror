#include "UIRenderer.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

static Shader* uiShader = nullptr;

bool UIRenderer::Init(int screenW, int screenH) {
    screenW_ = screenW; screenH_ = screenH;
    uiShader = new Shader("assets/shaders/ui/ui.vert", "assets/shaders/ui/ui.frag");
    InitQuad();
    return true;
}

void UIRenderer::Shutdown() {
    delete uiShader;
    glDeleteVertexArrays(1, &quadVAO_);
    glDeleteBuffers(1, &quadVBO_);
}

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

void UIRenderer::DrawThreatIndicator(float intensity, float angle) {}
void UIRenderer::SetUIScale(float scale) {}
void UIRenderer::DrawPauseMenu(int hoveredItem) { DrawFade(0.5f, 0, 0, 0); }
void UIRenderer::DrawJumpscare(float intensity) { DrawFade(intensity, 1, 0, 0); }
void UIRenderer::DrawGameOver(int deaths, float survivalTime) { RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1); }
void UIRenderer::DrawWinScreen(float clearTime, int deaths) { RenderQuad(0, 0, (float)screenW_, (float)screenH_, 1, 1, 1, 1); }
void UIRenderer::DrawCredits(float scrollY) { RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1); }

void UIRenderer::DrawCutsceneDay1(float elapsed) { RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1); }
void UIRenderer::DrawPoopEvent1(float elapsed) { RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0.36f, 0.23f, 0.12f, 1); }
void UIRenderer::DrawCutsceneDay5(float elapsed) { RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0, 0, 0, 1); }
void UIRenderer::DrawPoopEvent2(float elapsed) { RenderQuad(0, 0, (float)screenW_, (float)screenH_, 0.36f, 0.23f, 0.12f, 1); }
void UIRenderer::DrawBabyOilCutscene(float elapsed) { DrawFade(0.5f, 1, 1, 0.8f); }
void UIRenderer::DrawChaseCutscene(float elapsed) { DrawFade(1.0f, 0, 0, 0); }

void UIRenderer::DrawFade(float alpha, float r, float g, float b) {
    RenderQuad(0, 0, (float)screenW_, (float)screenH_, r, g, b, alpha);
}

void UIRenderer::DrawSubtitle(const std::string& text, float alpha, float y) {}
void UIRenderer::DrawTypewriter(const std::string& text, float elapsed, float charsPerSec, float x, float y, float scale) {}
void UIRenderer::RenderText(const std::string& text, float x, float y, float scale, float r, float g, float b, float a) {}

void UIRenderer::RenderQuad(float x, float y, float w, float h, float r, float g, float b, float a, unsigned int texID) {
    uiShader->Use();
    glm::mat4 projection = glm::ortho(0.0f, (float)screenW_, (float)screenH_, 0.0f, -1.0f, 1.0f);
    uiShader->SetMat4("projection", projection);
    uiShader->SetVec3("spriteColor", glm::vec3(r, g, b));
    uiShader->SetFloat("alpha", a);
    
    if (texID != 0) {
        uiShader->SetBool("useTexture", true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
    } else {
        uiShader->SetBool("useTexture", false);
    }

    float vertices[] = { 
        x,     y + h,   0.0f, 1.0f,
        x + w, y,       1.0f, 0.0f,
        x,     y,       0.0f, 0.0f, 

        x,     y + h,   0.0f, 1.0f,
        x + w, y + h,   1.0f, 1.0f,
        x + w, y,       1.0f, 0.0f
    };

    glBindVertexArray(quadVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void UIRenderer::InitQuad() {
    glGenVertexArrays(1, &quadVAO_);
    glGenBuffers(1, &quadVBO_);
    glBindVertexArray(quadVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void UIRenderer::InitFontAtlas() {}
unsigned int UIRenderer::LoadTex(const char* path) { return 0; }
