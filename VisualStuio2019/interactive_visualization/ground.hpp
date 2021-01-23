//
//  ground.hpp
//  interactive_visualization2
//
//  Created by 이재현 on 2020/11/18.
//

#ifndef ground_hpp
#define ground_hpp

#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "error_check.h"

using namespace std;
//class Ground {
//
//public:
//
//    float slice;
//    unsigned int GRID_SIZE;
//    GLuint VAO,VBO,EBO;
//    std::vector<glm::vec3> vertices;
//    std::vector<glm::uvec4> indices;
//    Ground(float slice, unsigned int GRID_SIZE);
//    void draw(Shader& shader,Light& light, Camera& camera);
//
//    GLuint lenght;
//};
class Ground{
    
public:
    
    
    //model matrix 그리기
    
    int LENGTH;
    
    //Ka,Ks,Kd,Ke; , sh 그리기
    
    glm::vec3 Ka{0.0f,0.0f,0.0f};
    glm::vec3 Ks{0.8f,0.8f,0.8f};
    glm::vec3 Ke{0.1f,0.1f,0.1f};
    float sh = 200.f;
    glm::mat4x4 M;
   

    
    GLuint VAO_BLACK;
    GLuint VAO_WHITE;
    
    GLuint VBO_BLACK;
    GLuint VBO_WHITE;
    
    GLuint EBO_BLACK;
    GLuint EBO_WHITE;
    
    
    vector<glm::vec3> vertice_pos_black;
    vector<glm::vec3> vertice_pos_white;
    
    std::vector<glm::uvec3> indices_black;
    std::vector<glm::uvec3> indices_white;
    
    

    
    Ground();
    void init();
    
    void render(Shader& shader, Light& light, Camera& camera);
    
private:
    
    void renderBlack();
    void renderWhite();
    
    
    
    
    
    
};

#endif /* ground_hpp */
