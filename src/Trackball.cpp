#define GLM_ENABLE_EXPERIMENTAL
#include "Trackball.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtx/component_wise.hpp>
#include <cmath>

Trackball::Trackball(float radius) : radius(radius), isRotating(false), rotationQuat(1, 0, 0, 0) {}

void Trackball::setRadius(float r) {
    radius = r;
}

void Trackball::startRotation(float x, float y) {
    isRotating = true;
    lastPosition = glm::vec2(x, y);
}

void Trackball::updateRotation(float x, float y) {
    if (!isRotating) return;
    
    glm::vec3 newPosition = mapToSphere(glm::vec2(x, y));
    glm::vec3 from = mapToSphere(lastPosition);
    glm::vec3 axis = glm::cross(from, newPosition);
    float angle = glm::acos(glm::min(glm::dot(from, newPosition), 1.0f));
    
    if (glm::length(axis) > 0.0001f) {
        axis = glm::normalize(axis);
        glm::quat deltaRotation = glm::angleAxis(angle, axis);
        rotationQuat = deltaRotation * rotationQuat;
    }
    
    lastPosition = glm::vec2(x, y);
}

void Trackball::stopRotation() {
    isRotating = false;
}

void Trackball::resetRotation() {
    rotationQuat = glm::quat(1, 0, 0, 0);
}

glm::mat4 Trackball::getRotationMatrix() const {
    return glm::toMat4(rotationQuat);
}

glm::vec3 Trackball::mapToSphere(const glm::vec2& point) {
    glm::vec3 result(point.x, point.y, 0);
    float squaredLength = glm::dot(point, point);
    
    if (squaredLength > 1.0f) {
        result = glm::normalize(result);
    } else {
        result.z = std::sqrt(1.0f - squaredLength);
    }
    
    return result;
}
