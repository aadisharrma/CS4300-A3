#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

// Forward declarations for classes we'll need
class TransformNode;
class GeometryNode;

/**
 * This is the visitor interface for the scene graph.
 */
class SGVisitor {
public:
    virtual ~SGVisitor() {}
    virtual void visit(TransformNode* node) = 0;
    virtual void visit(GeometryNode* node) = 0;
};

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
 * and typically includes a transformation.
 */
class TransformNode : public SGNode {
public:
    // This is the constructor taking a name, passing it to SGNode.
    TransformNode(const std::string& name)
        : SGNode(name), transformationMatrix(1.0f) {} // Initialize to identity matrix

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

    // Set the transformation matrix
    void setTransformation(const glm::mat4& matrix) {
        transformationMatrix = matrix;
    }

    // Get the transformation matrix
    glm::mat4 getTransformation() const {
        return transformationMatrix;
    }

private:
    // This is the list of child SGNodes under this TransformNode.
    std::vector<SGNode*> children;
    
    // This is the transformation matrix for this node.
    glm::mat4 transformationMatrix;
};

/**
 * This is a GeometryNode, typically a leaf node that holds shape data.
 */
class GeometryNode : public SGNode {
public:
    // This is the constructor taking a name and passing it to the base class.
    GeometryNode(const std::string& name)
        : SGNode(name), vao(0), elementCount(0) {}

    // This calls the visitor's method specifically for a GeometryNode.
    void accept(SGVisitor* visitor) override;
    
    // Methods for vertices and indices
    std::vector<float> getVertices() const {
        return vertices;
    }
    
    std::vector<unsigned int> getIndices() const {
        return indices;
    }
    
    // Setters for vertices and indices
    void setVertices(const std::vector<float>& v) {
        vertices = v;
    }
    
    void setIndices(const std::vector<unsigned int>& i) {
        indices = i;
        elementCount = i.size();
    }
    
    // OpenGL-specific getters and setters
    void setVAO(unsigned int v) {
        vao = v;
    }
    
    unsigned int getVAO() const {
        return vao;
    }
    
    void setElementCount(int count) {
        elementCount = count;
    }
    
    int getElementCount() const {
        return elementCount;
    }

private:
    // Geometry data
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // OpenGL handles
    unsigned int vao;
    int elementCount;
};
