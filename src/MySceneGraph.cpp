#include "MySceneGraph.h"
#include "PrintVisitor.h" // This is where SGVisitor is defined

// This invokes visitTransformNode(...) when a visitor visits a TransformNode.
void TransformNode::accept(SGVisitor* visitor) {
    visitor->visitTransformNode(this);
}

// This invokes visitGeometryNode(...) when a visitor visits a GeometryNode.
void GeometryNode::accept(SGVisitor* visitor) {
    visitor->visitGeometryNode(this);
}
