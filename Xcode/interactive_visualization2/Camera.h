//
// Created by 이재현 on 2020/09/02.
//

#ifndef OPENGL_PLAYGROUND_CAMERA_H
#define OPENGL_PLAYGROUND_CAMERA_H


#include <glm/glm.hpp>

class Camera {

public:
    Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up,float fovy,float aspect,float zNear,float zFar);
    glm::mat4x4 getViewMatrix();
    glm::mat4x4 getProjectionMatrix();
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;

    float zNear;
    float zFar;
    float aspect;
    float fovy;
    glm::vec3 lookAtvec;
private:


    glm::mat4x4 viewTransform;
    glm::mat4x4 projectionTransform;


};


#endif //OPENGL_PLAYGROUND_CAMERA_H

