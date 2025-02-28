#include "PrintVisitor.h"
#include "MySceneGraph.h"
#include <iostream>

PrintVisitor::PrintVisitor() : indentLevel(0) {}

void PrintVisitor::printIndent() const {
    for (int i = 0; i < indentLevel; i++) {
        std::cout << "  "; // Two spaces per indent level
    }
}

void PrintVisitor::visit(TransformNode* node) {
    printIndent();
    std::cout << "TransformNode: " << node->getName() << std::endl;
    
    // Increase indent level for children
    indentLevel++;
    
    // Visit all children
    for (auto child : node->getChildren()) {
        child->accept(this);
    }
    
    // Decrease indent level after visiting children
    indentLevel--;
}

void PrintVisitor::visit(GeometryNode* node) {
    printIndent();
    std::cout << "GeometryNode: " << node->getName() << std::endl;
}
