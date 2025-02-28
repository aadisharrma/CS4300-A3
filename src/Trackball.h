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
    glm::quat rotationQuat;

    glm::vec3 mapToSphere(const glm::vec2& point);

public:
    Trackball(float radius = 1.0f);

    void setRadius(float r);
    void startRotation(float x, float y);
    void updateRotation(float x, float y);
    void stopRotation();
    void resetRotation();
    glm::mat4 getRotationMatrix() const;
};

#endif // TRACKBALL_H
