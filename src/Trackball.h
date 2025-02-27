#ifndef TRACKBALL_H
#define TRACKBALL_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Trackball {
private:
    float radius;
    bool isRotating;
    glm::vec2 lastPosition;
    glm::quat currentRotation;
    glm::quat initialRotation;

    // Maps window coordinates to a point on the trackball sphere
    glm::vec3 mapToSphere(const glm::vec2& point) {
        glm::vec2 p = point / radius;
        float d = glm::length(p);
        
        // Clamp d to the range [0, 1]
        if (d > 1.0f) {
            d = 1.0f;
        }
        
        // Calculate z-coordinate on the sphere
        float z = std::sqrt(1.0f - d * d);
        
        // Return the normalized 3D point
        return glm::normalize(glm::vec3(p.x, p.y, z));
    }

public:
    Trackball() : radius(1.0f), isRotating(false), currentRotation(1.0f, 0.0f, 0.0f, 0.0f) {}

    void setRadius(float r) {
        radius = r;
    }

    void startRotation(float x, float y) {
        isRotating = true;
        lastPosition = glm::vec2(x, y);
        initialRotation = currentRotation;
    }

    void updateRotation(float x, float y) {
        if (!isRotating) return;
        
        glm::vec2 newPosition(x, y);
        
        // Calculate rotation between last position and new position
        glm::vec3 from = mapToSphere(lastPosition);
        glm::vec3 to = mapToSphere(newPosition);
        
        // Create quaternion from the rotation
        float dot = glm::dot(from, to);
        if (dot > 0.9999f) {
            // Points are too close, no significant rotation
            return;
        }
        
        // Cross product gives axis of rotation
        glm::vec3 axis = glm::cross(from, to);
        
        // Angle between the points
        float angle = std::acos(std::min(dot, 1.0f));
        
        // Create rotation quaternion
        glm::quat newRotation = glm::angleAxis(angle, glm::normalize(axis));
        
        // Update current rotation
        currentRotation = newRotation * initialRotation;
        
        // Update last position
        lastPosition = newPosition;
    }

    void stopRotation() {
        isRotating = false;
    }

    void resetRotation() {
        currentRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }

    glm::mat4 getRotationMatrix() const {
        return glm::toMat4(currentRotation);
    }
};

#endif // TRACKBALL_H
