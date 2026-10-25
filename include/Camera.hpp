#pragma once

#include <glm/glm.hpp>

class Camera{
public:
    //position and orientation
    glm::vec3 position{4.0f, 3.0f, 5.0f};
    float yaw = -90.0f;
    float pitch = 0.0f;
    glm::vec3 front, right, up;

    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};

    //projection
    float fov = 90.0f;
    float aspect_ratio = 1.0f;
    float near_plane = 0.1f;
    float far_plane = 100.0f;

    //movement
    float movementSpeed = 10.0f;
    float rotationSensitivity = 0.2f;

    //get matrices for rendering
    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix();


    //move camera
    void moveForward(float amount);
    void moveRight(float amount);
    void moveUp(float amount);

    //other operations
    void rotate(float yaw, float pitch);
    void zoom(float amount);

private: 
    glm::vec3 getForward();
    glm::vec3 getRight();
    glm::vec3 getUp();

};