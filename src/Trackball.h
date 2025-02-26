#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Trackball {
public:
    Trackball(float radius = 1.0f);  // Constructor

    void startRotation(float x, float y);  // Click
    void updateRotation(float x, float y); // Drag
    void resetRotation();  // Reset

    glm::mat4 getRotationMatrix() const;

private:
    float radius;
    glm::vec3 lastPosition;
    glm::quat rotationQuat;

    glm::vec3 mapToSphere(float x, float y);
};
