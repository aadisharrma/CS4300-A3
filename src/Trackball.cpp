#include "Trackball.h"
#include <glm/gtx/quaternion.hpp>
#include <cmath>

Trackball::Trackball(float radius) : radius(radius), rotationQuat(1, 0, 0, 0) {}

// When the user presses the mouse
void Trackball::startRotation(float x, float y) {
    lastPosition = mapToSphere(x, y);
}

// When the user draags the pressed mouse
void Trackball::updateRotation(float x, float y) {
    glm::vec3 newPosition = mapToSphere(x, y);

    glm::vec3 axis = glm::cross(lastPosition, newPosition);
    float angle = glm::length(axis);

    if (angle > 0.0001f) {
        axis = glm::normalize(axis);
        glm::quat deltaRotation = glm::angleAxis(angle, axis);
        rotationQuat = deltaRotation * rotationQuat;
    }

    lastPosition = newPosition;
}

void Trackball::resetRotation() {
    rotationQuat = glm::quat(1, 0, 0, 0);
}

glm::mat4 Trackball::getRotationMatrix() const {
    return glm::toMat4(rotationQuat);
}

// Converts the coordinats to the 3d sphere
glm::vec3 Trackball::mapToSphere(float x, float y) {
    glm::vec3 result(x, y, 0);
    float squaredLength = x * x + y * y;
    
    if (squaredLength > 1.0f) {
        result = glm::normalize(result);
    } else {
        result.z = std::sqrt(1.0f - squaredLength);
    }

    return result;
}

