#pragma once
#include <iostream>
#include <string>
#include "MySceneGraph.h"

// This is a forward declaration of node classes to avoid circular includes.
class TransformNode;
class GeometryNode;

///**
// * This is the base visitor interface that all visitors implement.
// * Each node type calls the matching method (TransformNode calls visitTransformNode, etc.).
// */
//class SGVisitor {
//public:
//    // This is a virtual destructor, letting us delete derived visitors via SGVisitor* if needed.
//    virtual ~SGVisitor() {}
//
//    // This handles TransformNode visits.
//    virtual void visitTransformNode(TransformNode* node) = 0;
//
//    // This handles GeometryNode visits.
//    virtual void visitGeometryNode(GeometryNode* node) = 0;
//};

/**
 * This is our concrete PrintVisitor, which traverses the scene graph
 * and prints a text hierarchy to the console.
 */
class PrintVisitor : public SGVisitor {
public:
    // This initializes our indent level to 0.
    PrintVisitor();

    // These implement the SGVisitor interface.
    void visit(TransformNode* node) override;
    void visit(GeometryNode* node) override;

private:
    // This is the current indentation level, controlling how many spaces we print.
    int indentLevel;

    // This actually prints the indentation in the console.
    void printIndent() const;
};
