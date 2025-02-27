#include <iostream>
#include <string>
#include "MySceneGraph.h"
#include "PrintVisitor.h"
#include "Trackball.h"
// #include "CommandParser.h"
#include "Render.h"

/**
 * Function to parse command file and build scene graph
 */
bool parseCommands(const std::string& filename, SGNode*& rootOut);

int main(int argc, char** argv) {
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
