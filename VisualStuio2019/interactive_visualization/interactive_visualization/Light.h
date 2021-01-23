//
// Created by 이재현 on 2020/09/02.
//

#ifndef OPENGL_PLAYGROUND_LIGHT_H
#define OPENGL_PLAYGROUND_LIGHT_H
#include <glm/glm.hpp>


struct Light{
    glm::vec3 srcpos;
    glm::vec3 direction;
    glm::vec3 specColor;
    glm::vec3 ambColor;
    glm::vec3 diffColor;
};


#endif //OPENGL_PLAYGROUND_LIGHT_H
