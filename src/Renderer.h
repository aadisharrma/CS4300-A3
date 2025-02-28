#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include "MySceneGraph.h"
#include "Trackball.h"

class Renderer {
public:
    SDL_Window* window;
    SDL_GLContext glContext;
    GLuint shaderProgram;
    SGNode* root;
    Trackball* trackball;
    
    Renderer(int width, int height, SGNode* sceneRoot, Trackball* tb = nullptr);
    void SetupCamera();
    void SetupShaders();
    void InitializeSceneGraph();
    void HandleInput();
    void RenderLoop();
    void RenderSceneGraph(SGNode* node, const glm::mat4& parentTransform);
    ~Renderer();
};

#endif // RENDERER_H
