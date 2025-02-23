#include "PrintVisitor.h"
#include "MySceneGraph.h"

// This is the constructor that sets indentLevel to 0.
PrintVisitor::PrintVisitor() : indentLevel(0) {}

/**
 * This prints the correct number of spaces based on 'indentLevel'.
 */
void PrintVisitor::printIndent() const {
    // This loops from 0 to indentLevel, printing 2 spaces per level.
    for (int i = 0; i < indentLevel; i++) {
        std::cout << "  ";
    }
}

/**
 * This handles a TransformNode by:
 * 1) Printing its name
 * 2) Increasing indentLevel
 * 3) Visiting all children
 * 4) Decreasing indentLevel
 */
void PrintVisitor::visitTransformNode(TransformNode* node) {
    printIndent();
    std::cout << "TransformNode (" << node->getName() << ")\n";

    // This increases the indentation for the children.
    indentLevel++;

    // This iterates over each child and accepts the visitor.
    for (auto child : node->getChildren()) {
        child->accept(this);
    }

    // This decreases the indentation after finishing child visits.
    indentLevel--;
}

/**
 * This handles a GeometryNode by printing the node's name.
 * A GeometryNode is usually a leaf, so there are no children to visit.
 */
void PrintVisitor::visitGeometryNode(GeometryNode* node) {
    printIndent();
    std::cout << "GeometryNode (" << node->getName() << ")\n";
}
