#pragma once

#ifndef TB_GL_RENDERAPP
#define TB_GL_RENDERAPP

#include "common.h"
TB_GL_NAMESPACE_BEGIN 

class RenderApp 
{
public:
    RenderApp();
    ~RenderApp();
    bool Init();
    bool Run();
    bool ShouldClose();
    bool CompileVertexShader(const char* path);
    bool CompileFragmentShader(const char* path);
    bool LinkProgram();
    bool CreateGeometry();
    void SetUniforms();
    void LoadTexture();
private:
    GLFWwindow* window = NULL;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int shaderProgram;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int texture;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void readShaderSource(const char* path, char* pdest);


TB_GL_NAMESPACE_END

#endif