#include <iostream>
#include <string>
#include "MySceneGraph.h"
#include "PrintVisitor.h"

/**
 * This is a forward declaration of your command parser.
 * It should take a filename and an SGNode* reference to fill with the root node.
 * The actual implementation depends on your assignment's code or starter files.
 */
bool parseCommands(const std::string& filename, SGNode*& rootOut);

int main(int argc, char** argv) {
    // This checks if we have at least one command-line argument (the commands file).
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <commands-file.txt>\n";
        return 1;
    }

    // This stores the name/path of the commands file (e.g., "scene-commands.txt").
    std::string commandsFile = argv[1];

    // This is a pointer to the root node of our scene graph; we'll set it via parseCommands.
    SGNode* root = nullptr;

    // This calls parseCommands(...) to build the scene graph from the commands file.
    // If it succeeds, 'root' will point to the top-level node (e.g. a TransformNode).
    bool success = parseCommands(commandsFile, root);
    if (!success || !root) {
        std::cerr << "Error: Could not build scene from " << commandsFile << "\n";
        return 1;
    }

    // This constructs a PrintVisitor to traverse the resulting scene graph.
    PrintVisitor printer;

    // This calls accept(...) on the root, which will recursively print each node's hierarchy.
    root->accept(&printer);

    // This is where you'd normally run your rendering code (e.g. an OpenGL loop) if you want
    // to visualize the scene. For Part 2, we're just printing the hierarchy.

    // This deletes 'root' to clean up memory. In a full solution, you might use smart pointers
    // or ensure each node's destructor also deletes its children.
    delete root;

    // This indicates successful execution.
    return 0;
}
