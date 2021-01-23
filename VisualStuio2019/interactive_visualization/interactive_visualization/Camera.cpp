//
// Created by 이재현 on 2020/09/02.
//
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up,float fovy,float aspect,float zNear,float zFar) {

    this->eye=eye;
    this->center=center;
    this->up=up;
    this->aspect=aspect;
    this->fovy=fovy;
    this->zFar=zFar;
    this->zNear=zNear;

    viewTransform=glm::lookAt(eye,center,up);
    projectionTransform=glm::perspective(fovy,aspect,zNear,zFar);



}

glm::mat4x4 Camera::getViewMatrix() {
    return viewTransform;
}

glm::mat4x4 Camera::getProjectionMatrix() {
    return projectionTransform;
}
