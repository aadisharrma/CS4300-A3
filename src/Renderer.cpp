#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
#include <iostream>
#include <map>
#include "Shader.h"
#include "MySceneGraph.h"
#include "Trackball.h"

// Forward declarations - ensure this matches your actual class structure
class SGVisitor;

// Define Material class if not already defined elsewhere
class Material {
public:
    virtual glm::vec4 getColor() const { return glm::vec4(1.0f); }
};

class Renderer {
public:
    SDL_Window* window;
    SDL_GLContext glContext;
    GLuint shaderProgram;
    SGNode* root;
    Trackball* trackball;
    
    // Camera properties
    glm::vec3 cameraPosition;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    int windowWidth, windowHeight;
    
    // Uniform locations
    GLint modelMatrixLoc;
    GLint viewMatrixLoc;
    GLint projectionMatrixLoc;
    GLint materialColorLoc;
    
    // GLScenegraphRenderer class - A visitor for initializing OpenGL resources
    class GLScenegraphRenderer : public SGVisitor {
    private:
        // Shader-related data
        GLuint shaderProgram;
        
        // Keep track of VAOs and VBOs
        std::map<std::string, GLuint> vaos;
        std::map<std::string, GLuint> vbos;
        std::map<std::string, GLuint> ebos;
        std::map<std::string, int> elementCounts;
        
    public:
        GLScenegraphRenderer(GLuint program) : shaderProgram(program) {}
        
        void visit(TransformNode* node) override {
            // Visitor pattern: visit all children
            for (SGNode* child : node->getChildren()) {
                child->accept(this);
            }
        }
        
        void visit(GeometryNode* node) override {
            // Create VAO, VBO, EBO for this geometry node
            std::string name = node->getName();
            
            // Skip if already processed
            if (vaos.find(name) != vaos.end()) {
                return;
            }
            
            GLuint vao, vbo, ebo;
            
            // Create and bind VAO
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            
            // Create and bind VBO for vertex data
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            
            // Assume node has getVertices() method returning a vector<float>
            std::vector<float> vertices = node->getVertices();
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
            
            // Position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            
            // Normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            
            // Element buffer for indices
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            
            // Assume node has getIndices() method returning a vector<unsigned int>
            std::vector<unsigned int> indices = node->getIndices();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                         indices.data(), GL_STATIC_DRAW);
            
            // Store for later use
            vaos[name] = vao;
            vbos[name] = vbo;
            ebos[name] = ebo;
            elementCounts[name] = indices.size();
            
            // Unbind
            glBindVertexArray(0);
            
            // Store VAO and element count in the node for rendering
            // This assumes your GeometryNode class has these methods
            // node->setVAO(vao);
            // node->setElementCount(indices.size());
        }
        
        ~GLScenegraphRenderer() {
            // Clean up OpenGL resources
            for (auto& pair : vaos) {
                glDeleteVertexArrays(1, &pair.second);
            }
            for (auto& pair : vbos) {
                glDeleteBuffers(1, &pair.second);
            }
            for (auto& pair : ebos) {
                glDeleteBuffers(1, &pair.second);
            }
        }
    };

    
    void InitializeSceneGraph() {
        if (root) {
            // Initialize renderer - use appropriate class based on your codebase
            GLScenegraphRenderer renderer(shaderProgram);
            
            // Visit the entire scene graph to initialize OpenGL objects
            root->accept(&renderer);
        }
    }

    Renderer(int width, int height, SGNode* sceneRoot, Trackball* tb = nullptr)
        : root(sceneRoot), trackball(tb), windowWidth(width), windowHeight(height) {
        
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            exit(1);
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        window = SDL_CreateWindow("Scene Graph Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (!window) {
            std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
            exit(1);
        }

        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
            exit(1);
        }

        // Initialize GLAD with SDL
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            exit(1);
        }

        // Setup default camera and projection
        cameraPosition = glm::vec3(0.0f, 3.0f, 10.0f);
        SetupCamera();
        
        // Create default trackball if none provided
        if (!trackball) {
            trackball = new Trackball();
            trackball->setRadius(5.0f);
        }

        // Setup OpenGL state
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        
        // Load and setup shaders
        SetupShaders();
        
        // Initialize VAOs and VBOs for the scene graph
        InitializeSceneGraph();
    }

    void SetupCamera() {
        // View matrix
        viewMatrix = glm::lookAt(
            cameraPosition,             // Camera position
            glm::vec3(0.0f, 0.0f, 0.0f), // Look at the center
            glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
        );
        
        // Perspective projection
        float aspectRatio = static_cast<float>(windowWidth) / windowHeight;
        projectionMatrix = glm::perspective(
            glm::radians(45.0f), // FOV
            aspectRatio,         // Aspect ratio
            0.1f,                // Near plane
            100.0f               // Far plane
        );
    }

    void SetupShaders() {
        shaderProgram = Shader::Load("shaders/default.vert", "shaders/default.frag");
        
        // Get uniform locations
        modelMatrixLoc = glGetUniformLocation(shaderProgram, "modelMatrix");
        viewMatrixLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
        projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
        materialColorLoc = glGetUniformLocation(shaderProgram, "materialColor");
    }

    void HandleInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    exit(0);
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        trackball->startRotation(event.button.x, event.button.y);
                    }
                    break;
                
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                    }
                    break;
                
                case SDL_MOUSEMOTION:
                    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                        trackball->updateRotation(event.motion.x, event.motion.y);
                    }
                    break;
                
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_r) {
                        trackball->resetRotation();
                    }
                    break;
                
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        windowWidth = event.window.data1;
                        windowHeight = event.window.data2;
                        glViewport(0, 0, windowWidth, windowHeight);
                        SetupCamera();
                    }
                    break;
            }
        }
    }

    void RenderLoop() {
        bool running = true;
        
        while (running) {
            HandleInput();
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(shaderProgram);
            
            // Update view and projection uniforms
            glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
            glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
            
            // Apply trackball rotation to the model
            glm::mat4 trackballMatrix = trackball->getRotationMatrix();
            
            if (root) {
                RenderSceneGraph(root, trackballMatrix);
            }
            
            SDL_GL_SwapWindow(window);
            SDL_Delay(16); // ~60 fps
        }
    }

    void RenderSceneGraph(SGNode* node, const glm::mat4& parentTransform) {
        if (!node) return;
        
        // Current transform starts with parent transform
        glm::mat4 currentTransform = parentTransform;
        
        // Apply node's transformation if it's a transform node
        TransformNode* transformNode = dynamic_cast<TransformNode*>(node);
        if (transformNode) {
            // Apply the node's transformation
            // Note: You need to implement getTransformationMatrix() in your TransformNode class
            currentTransform = currentTransform * transformNode->getTransformation();
        }
        
        // Render geometry if it's a geometry node
        GeometryNode* geometryNode = dynamic_cast<GeometryNode*>(node);
        if (geometryNode) {
            // Set the model matrix uniform
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(currentTransform));
            
            // Set material color - adjust based on your actual class implementation
            // Material* material = geometryNode->getMaterial();
            // if (material) {
            //    glm::vec4 color = material->getColor();
            //    glUniform4fv(materialColorLoc, 1, glm::value_ptr(color));
            // }
            
            // Get the VAO and element count - adjust based on your implementation
            // GLuint vao = geometryNode->getVAO();
            // int elementCount = geometryNode->getElementCount();
            
            // Using placeholder values - replace with actual implementation
            GLuint vao = 0; // Replace with actual implementation
            int elementCount = 0; // Replace with actual implementation
            
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        
        // Process all children with the current transform
        if (transformNode) {
            for (SGNode* child : transformNode->getChildren()) {
                RenderSceneGraph(child, currentTransform);
            }
        }
    }

    ~Renderer() {
        if (trackball && trackball != nullptr) {
            delete trackball;
        }
        
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

//// GLScenegraphRenderer class - A visitor for initializing OpenGL resources
//class GLScenegraphRenderer : public SGVisitor {
//private:
//    // Shader-related data
//    // GLuint shaderProgram;
//    
//    // Keep track of VAOs and VBOs
//    std::map<std::string, GLuint> vaos;
//    std::map<std::string, GLuint> vbos;
//    std::map<std::string, GLuint> ebos;
//    std::map<std::string, int> elementCounts;
//    
//public:
//    GLScenegraphRenderer(GLuint program) : shaderProgram(program) {}
//    
//    void visit(TransformNode* node) override {
//        // Visitor pattern: visit all children
//        for (SGNode* child : node->getChildren()) {
//            child->accept(this);
//        }
//    }
//    
//    void visit(GeometryNode* node) override {
//        // Create VAO, VBO, EBO for this geometry node
//        std::string name = node->getName();
//        
//        // Skip if already processed
//        if (vaos.find(name) != vaos.end()) {
//            return;
//        }
//        
//        GLuint vao, vbo, ebo;
//        
//        // Create and bind VAO
//        glGenVertexArrays(1, &vao);
//        glBindVertexArray(vao);
//        
//        // Create and bind VBO for vertex data
//        glGenBuffers(1, &vbo);
//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        
//        // Assume node has getVertices() method returning a vector<float>
//        std::vector<float> vertices = node->getVertices();
//        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
//        
//        // Position attribute
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//        
//        // Normal attribute
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(1);
//        
//        // Element buffer for indices
//        glGenBuffers(1, &ebo);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//        
//        // Assume node has getIndices() method returning a vector<unsigned int>
//        std::vector<unsigned int> indices = node->getIndices();
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
//                     indices.data(), GL_STATIC_DRAW);
//        
//        // Store for later use
//        vaos[name] = vao;
//        vbos[name] = vbo;
//        ebos[name] = ebo;
//        elementCounts[name] = indices.size();
//        
//        // Unbind
//        glBindVertexArray(0);
//        
//        // Store VAO and element count in the node for rendering
//        // This assumes your GeometryNode class has these methods
//        // node->setVAO(vao);
//        // node->setElementCount(indices.size());
//    }
//    
//    ~GLScenegraphRenderer() {
//        // Clean up OpenGL resources
//        for (auto& pair : vaos) {
//            glDeleteVertexArrays(1, &pair.second);
//        }
//        for (auto& pair : vbos) {
//            glDeleteBuffers(1, &pair.second);
//        }
//        for (auto& pair : ebos) {
//            glDeleteBuffers(1, &pair.second);
//        }
//    }
//};
