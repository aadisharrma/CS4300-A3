#pragma once
#include <string>
#include <vector>

// This is a forward declaration of the visitor interface so we can reference it later.
class SGVisitor;

/**
 * This is the base node class for our scene graph.
 * Every node has a name and a method to accept visitors.
 */
class SGNode {
public:
    // This is the constructor that sets the node's name.
    SGNode(const std::string& name) : name(name) {}
    
    // This is the virtual destructor, in case we subclass.
    virtual ~SGNode() {}

    // This accepts a visitor and invokes the proper function depending on the node type.
    virtual void accept(SGVisitor* visitor) = 0;

    // This returns the name of the node.
    std::string getName() const { return name; }

protected:
    // This is the variable storing the node's name.
    std::string name;
};

/**
 * This is a TransformNode, which can have children
 * and typically includes a transformation (omitted for brevity).
 */
class TransformNode : public SGNode {
public:
    // This is the constructor taking a name, passing it to SGNode.
    TransformNode(const std::string& name)
        : SGNode(name) {}

    // This adds a child node to the list of children.
    void addChild(SGNode* child) {
        children.push_back(child);
    }

    // This returns a read-only reference to the children.
    const std::vector<SGNode*>& getChildren() const {
        return children;
    }

    // This calls the visitor's method specifically for a TransformNode.
    void accept(SGVisitor* visitor) override;

private:
    // This is the list of child SGNodes under this TransformNode.
    std::vector<SGNode*> children;
};

/**
 * This is a GeometryNode, typically a leaf node that might hold shape data in a real application.
 */
class GeometryNode : public SGNode {
public:
    // This is the constructor taking a name and passing it to the base class.
    GeometryNode(const std::string& name)
        : SGNode(name) {}

    // This calls the visitor's method specifically for a GeometryNode.
    void accept(SGVisitor* visitor) override;
};
