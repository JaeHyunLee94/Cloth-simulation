////
////  ground.cpp
////  interactive_visualization2
////
////  Created by 이재현 on 2020/11/18.
////

#include <GL/glew.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "ground.hpp"




Ground::Ground(){
    init();
}



void Ground::init(){
  
    LENGTH=100;
    int num_black=0;
    int num_white=0;
    M=glm::mat4(1.0);
    
    for(int x=-LENGTH/2;x<LENGTH/2-1;x++){
        for(int z=LENGTH/2;z>-LENGTH/2+1;z--){
            if((x+z)%2==0){
                vertice_pos_black.push_back(glm::vec3(x,0,z));
                vertice_pos_black.push_back(glm::vec3(x-1,0,z));
                vertice_pos_black.push_back(glm::vec3(x-1,0,z+1));
                vertice_pos_black.push_back(glm::vec3(x,0,z+1));
                
                indices_black.push_back(glm::uvec3{num_black,num_black+1,num_black+2});
                indices_black.push_back(glm::uvec3{num_black,num_black+2,num_black+3});
                
                num_black+=4;
            }else{
                vertice_pos_white.push_back(glm::vec3(x,0,z));
                vertice_pos_white.push_back(glm::vec3(x-1,0,z));
                vertice_pos_white.push_back(glm::vec3(x-1,0,z+1));
                vertice_pos_white.push_back(glm::vec3(x,0,z+1));
                
                indices_white.push_back(glm::uvec3{num_white,num_white+1,num_white+2});
                indices_white.push_back(glm::uvec3{num_white,num_white+2,num_white+3});
                
                num_white+=4;
            }
            
            
        }
        
        
    }
    
    glGenVertexArrays(1,&VAO_BLACK);
    glBindVertexArray(VAO_BLACK);
    
    glGenBuffers(1,&VBO_BLACK);
    glBindBuffer(GL_ARRAY_BUFFER,VBO_BLACK);
    glBufferData(GL_ARRAY_BUFFER, vertice_pos_black.size()*sizeof(glm::vec3), &vertice_pos_black[0], GL_STATIC_DRAW); //size ?
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
    glDisableVertexAttribArray(0);

    
    glGenBuffers( 1, &EBO_BLACK );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO_BLACK);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices_black.size()*sizeof(glm::uvec3), &indices_black[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    
    
    
    glGenVertexArrays(1,&VAO_WHITE);
    glBindVertexArray(VAO_WHITE);
    
    glGenBuffers(1,&VBO_WHITE);
    glBindBuffer(GL_ARRAY_BUFFER,VBO_WHITE);
    glBufferData(GL_ARRAY_BUFFER, vertice_pos_white.size()*sizeof(glm::vec3), &vertice_pos_white[0], GL_STATIC_DRAW); //size ?
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
    glDisableVertexAttribArray(0);

    glGenBuffers( 1, &EBO_WHITE );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO_WHITE);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices_white.size()*sizeof(glm::uvec3), &indices_white[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    

    
}
void Ground::render(Shader& shader,Light& light,Camera& camera){
    shader.use();
    shader.setLight(light);
    shader.setCamera(camera);
    shader.setUniform("worldMat", M);
    shader.setUniform("Ka", Ka);
    shader.setUniform("Ks", Ks);
    shader.setUniform("Ke", Ke);
    shader.setUniform("sh", sh);

    
    
    glBindVertexArray(VAO_BLACK);
    glEnableVertexAttribArray(0);
    glVertexAttrib3f(1,0.0f,0.f,0.f);
    glVertexAttrib3f(2,0.0f,1.0f,0.0f);
    
    glDrawElements(GL_TRIANGLES, indices_black.size()*3, GL_UNSIGNED_INT, NULL);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    

    glBindVertexArray(VAO_WHITE);
    glEnableVertexAttribArray(0);
    glVertexAttrib3f(1,0.2f,1.f,1.f);
    glVertexAttrib3f(2,0.0f,1.0f,0.0f);
    glDrawElements(GL_TRIANGLES, indices_white.size()*3, GL_UNSIGNED_INT, NULL);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    
    
    
}
