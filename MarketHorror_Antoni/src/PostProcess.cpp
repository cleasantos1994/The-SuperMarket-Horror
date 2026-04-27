#include "PostProcess.h"
#include <iostream>

PostProcess::PostProcess() : shader_(nullptr) {}

bool PostProcess::Init(int w, int h) {
    w_ = w; h_ = h;
    InitFBO(w, h);
    InitQuad();
    shader_ = new Shader("assets/shaders/post/post.vert", "assets/shaders/post/post.frag");
    return true;
}

void PostProcess::Resize(int w, int h) {
    w_ = w; h_ = h;
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &colorTex_);
    glDeleteRenderbuffers(1, &rbo_);
    InitFBO(w, h);
}

void PostProcess::BeginCapture() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcess::EndCapture() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcess::Render(int fear, float time, bool isChase, bool isSlipping, bool isJumpscare, float jumpscareT) {
    shader_->Use();
    shader_->SetInt("screenTexture", 0);
    shader_->SetFloat("fearFactor", fear / 100.0f);
    shader_->SetFloat("time", time);
    shader_->SetFloat("chaseFactor", isChase ? 1.0f : 0.0f);
    shader_->SetFloat("slipFactor", isSlipping ? 1.0f : 0.0f);
    shader_->SetFloat("jumpscareT", jumpscareT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTex_);
    glBindVertexArray(screenVAO_);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

void PostProcess::InitFBO(int w, int h) {
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glGenTextures(1, &colorTex_);
    glBindTexture(GL_TEXTURE_2D, colorTex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex_, 0);
    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcess::InitQuad() {
    float quadVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &screenVAO_);
    glGenBuffers(1, &screenVBO_);
    glBindVertexArray(screenVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}