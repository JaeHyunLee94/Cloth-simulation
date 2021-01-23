//
//  Sphere.hpp
//  interactive_visualization2
//
//  Created by 이재현 on 2020/11/26.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Light.h"
#include "Shader.h"
#include "Camera.h"


class Cube{
    
public:
    GLuint VAO;
    GLuint VBO_POS;
    GLuint VBO_COLOR;
    GLuint EBO;
    
    int lats, longs;
    int numsToDraw;
    float radius;
    
    
    
    glm::vec3 pos;
    glm::vec3 velocity;
    
    
    float sh=100.f;
    glm::vec3 Ka{0.0f,0.0f,0.0f};
    glm::vec3 Ks{0.8f,0.8f,0.8f};
    glm::vec3 Ke{0.1f,0.1f,0.1f};
    glm::mat4x4 M;
    
    
    Cube();
    void init();
    void update();
    void render(Shader& shader, Light& light,Camera& camera);

    
    
private:
    
    
    
    
};

#endif /* Sphere_hpp */
