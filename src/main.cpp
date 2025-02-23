#include <iostream>
#include "MySceneGraph.h"
#include "PrintVisitor.h"

/**
 * This is our entry point, where we build a small scene graph,
 * then use PrintVisitor to display its structure.
 */
int main() {
    // This creates a root transform node.
    TransformNode* root = new TransformNode("RootNode");

    // This creates a sub-tree for a building.
    TransformNode* buildingRoot = new TransformNode("BuildingRoot");
    GeometryNode* buildingBase = new GeometryNode("BuildingBase");

    // This attaches buildingBase as a child of buildingRoot.
    buildingRoot->addChild(buildingBase);

    // This attaches buildingRoot as a child of root.
    root->addChild(buildingRoot);

    // This creates another geometry node called ground.
    GeometryNode* ground = new GeometryNode("GroundPlane");
    root->addChild(ground);

    // This constructs a PrintVisitor object that will print out the hierarchy.
    PrintVisitor printer;

    // This calls accept(...) on the root, which recursively visits all nodes.
    root->accept(&printer);

    // This is where you'd normally do cleanup; in a real app, you'd use smart pointers
    // or properly delete children in the destructor.
    delete root;

    return 0;
}
