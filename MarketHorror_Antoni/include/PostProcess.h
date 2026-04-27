#pragma once
#include <glad/glad.h>
#include "Shader.h"

class PostProcess {
public:
    PostProcess();
    bool Init(int w, int h);
    void Resize(int w, int h);
    void BeginCapture();
    void EndCapture();

    void Render(int fear, float time,
                bool isChase, bool isSlipping,
                bool isJumpscare, float jumpscareT);

private:
    unsigned int fbo_, colorTex_, rbo_;
    unsigned int screenVAO_, screenVBO_;
    Shader*      shader_;
    int          w_, h_;
    void InitQuad();
    void InitFBO(int w, int h);
};
