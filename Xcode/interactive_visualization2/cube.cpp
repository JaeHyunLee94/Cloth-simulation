//
//  Sphere.cpp
//  interactive_visualization2
//
//  Created by 이재현 on 2020/11/26.
//

#include "cube.hpp"


Cube::Cube(){
    
    init();
    
}

void Cube::init(){
    
    
    float pos[]={
        
        3.0f,0.0f,2.0f,
        3.0f,0.0f,4.0f,
        5.0f,0.0f,4.0f,
        5.0f,0.0f,2.0f,
        
        
        3.0f,2.0f,2.0f,
        3.0f,2.0f,4.0f,
        5.0f,2.0f,4.0f,
        5.0f,2.0f,2.0f
        
    };
    
    
    float color[]={
        
        1.0f,0.0f,1.0f,
        0.6f,0.2f,0.3f,
        0.9f,0.3f,0.0f,
        0.3f,0.9f,0.2f,
        
        
        1.0f,0.0f,1.0f,
        0.6f,0.2f,0.3f,
        0.9f,0.3f,0.0f,
        0.3f,0.9f,0.2f
        
    };
    
//    float normal[]={
//
//
//    };
    
   unsigned int indices[]{
        1,0,2,
        2,0,3,
        1,2,5,
        2,6,5,
        3,7,6,
        3,6,2,
        6,4,5,
        6,7,4,
        1,5,4,
        1,4,0,
        0,4,7,
        3,0,7
    };
    
    M=glm::mat4(1.0f);
    
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1,&VBO_POS);
    glBindBuffer(GL_ARRAY_BUFFER,VBO_POS);
    glBufferData(GL_ARRAY_BUFFER,sizeof(pos),&pos[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glGenBuffers(1,&VBO_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER,VBO_COLOR);
    glBufferData(GL_ARRAY_BUFFER,sizeof(color),&color[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    
    glGenBuffers(1,&EBO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),&indices[0],GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    

    
}

void Cube::render(Shader& shader, Light& light,Camera& camera){
    
    shader.use();
    shader.setLight(light);
    shader.setCamera(camera);
    shader.setUniform("worldMat", M);
    shader.setUniform("Ka", Ka);
    shader.setUniform("Ks", Ks);
    shader.setUniform("Ke", Ke);
    shader.setUniform("sh", sh);
    
    
    glBindVertexArray(VAO);
  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT , NULL);

    
}
