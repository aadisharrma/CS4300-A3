#include <iostream>
#include <string>
#define SDL_MAIN_HANDLED
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SDL.h>
#include "MySceneGraph.h"
#include "PrintVisitor.h"
#include "Trackball.h"
#include "Renderer.h"

/**
 * Function to parse command file and build scene graph
 */
bool parseCommands(const std::string& filename, SGNode*& rootOut);

int main(int argc, char** argv) {
    
    // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return -1;
    }

    // Create OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // DELETE THIS BEFORE SUBMISSION
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }


    // Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }
    
    // Check for command line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <commands-file.txt>\n";
        return 1;
    }

    // Get the commands file name
    std::string commandsFile = argv[1];

    // Root node for our scene graph
    SGNode* root = nullptr;

    // Parse the commands file to build the scene graph
    bool success = parseCommands(commandsFile, root);
    if (!success || !root) {
        std::cerr << "Error: Could not build scene from " << commandsFile << "\n";
        return 1;
    }

    // Print the scene graph hierarchy using the PrintVisitor
    std::cout << "Scene Graph Hierarchy:" << std::endl;
    PrintVisitor printer;
    root->accept(&printer);
    
    // Create a trackball for scene rotation
    Trackball* trackball = new Trackball();
    trackball->setRadius(5.0f); // Adjust based on your scene size
    
    // Create renderer and start rendering loop
    // This will handle the OpenGL setup, trackball interaction, and rendering
    Renderer renderer(800, 600, root, trackball);
    renderer.RenderLoop();

    // Clean up
    delete root;
    
    return 0;
}
