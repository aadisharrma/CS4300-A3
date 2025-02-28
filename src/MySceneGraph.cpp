#include "MySceneGraph.h"
#include "PrintVisitor.h"

// This invokes visit(...) when a visitor visits a TransformNode.
void TransformNode::accept(SGVisitor* visitor) {
    visitor->visit(this);
}

// This invokes visit(...) when a visitor visits a GeometryNode.
void GeometryNode::accept(SGVisitor* visitor) {
    visitor->visit(this);
}
