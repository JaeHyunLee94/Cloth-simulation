//
//  cloth.hpp
//  interactive_visualization2
//
//  Created by 이재현 on 2020/11/17.
//

#ifndef cloth_hpp
#define cloth_hpp

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include "error_check.h"

using namespace std;

struct Node{
    float mass;
    bool isFixed;
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 velocity;
    glm::vec3 force;
    
};


struct Spring{
    int type;
    float K; //coeff
    float rest_len;
    Node* firstNode;
    Node* secondNode;
    
};


class Cloth{
    
public:
    
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    
    float sh=5.f;
    glm::vec3 Ka{0.0f,0.0f,0.0f};
    glm::vec3 Ks{0.2f,0.2f,0.2f};
    glm::vec3 Ke{0.1f,0.1f,0.1f};
    glm::mat4x4 M;
    
    int width,height;
    float nodeMass;
    int numW,numH;
    int startX,startY,startZ;
    float dw,dh;
    glm::vec3 color;// 일단은 단색 ㅎㅎ.
    float fall_time=2.0f;
    float now_time=0.0f;
    
    float Kshear,Kstruct,Kbend;
    float Kd; //damping
    
    

    vector<unsigned int> indices;
    
    vector<Node> node;
    vector<Spring> spring;
    
    
    void computeInternalForce();
    Cloth(); 
    void init();
    void collisionCheck();
    void applyExternalForce(); //외부에서 호출 가능
    
    void computeNormal();
    void render(Shader& shader,Light& light,Camera& camera);
    
    void dump();
   
    void update(float dt); //glsubdata 로 바꾸기
    
private:
  
    
    
    
};

#endif /* cloth_hpp */
