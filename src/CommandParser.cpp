#include <iostream>
#include <fstream>
#include <string>
#include "MySceneGraph.h"

bool parseCommands(const std::string& filename, SGNode*& rootOut) {
    // A minimal stub that just returns false, or implement your real parsing here
    std::cerr << "parseCommands stub: reading " << filename << "\n";

    // For now, create a dummy root if you like:
    rootOut = new TransformNode("sceneRoot");

    return true;  // Indicate success
}
