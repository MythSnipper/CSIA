#include <Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::get_view_matrix(){
    
    return glm::lookAt(position, position + getForward(), worldUp);
}
glm::mat4 Camera::get_projection_matrix(){
    return glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);
}
glm::vec3 Camera::getForward(){
    glm::vec3 forward;
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(forward);
}
glm::vec3 Camera::getRight(){
    return glm::normalize(
        glm::cross(getForward(), worldUp)
    );
}
glm::vec3 Camera::getUp(){
    return glm::normalize(
        glm::cross(getRight(), getForward())
    );
}
void Camera::moveForward(float amount){
    position += getForward() * amount;
}
void Camera::moveRight(float amount){
    position += getRight() * amount;
}
void Camera::moveUp(float amount){
    position += worldUp * amount;
}
void Camera::rotate(float yaw, float pitch){
    this->yaw += yaw;
    this->pitch += pitch;
    if(this->yaw > 360){
        this->yaw -= 360;
    }
    if(this->yaw < 360){
        this->yaw += 360;
    }
    if(this->pitch > 89.9){
        this->pitch = 89.9;
    }
    if(this->pitch < -89.9){
        this->pitch = -89.9;
    }
}
void Camera::zoom(float amount){

}
