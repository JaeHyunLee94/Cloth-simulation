//
//  cloth.cpp
//  interactive_visualization2
//
//  Created by 이재현 on 2020/11/17.
//

#include "cloth.hpp"


Cloth::Cloth(){
    init();
}

void Cloth::init(){
    
    M=glm::mat4(1.0); // 이것도 깔끔하게
    startX=2;
    startY=4;
    startZ=1;
    
    width=6;
    height=4;
    
    numW=60;
    numH=40;
    nodeMass=0.5f;
    
    color=glm::vec3{0.3f,0.1f,0.7f};
    
    Kstruct=2000;
    Kshear=2000;
    Kbend=2400;
    Kd=18;
    
    dw=(float)width/(float)numW;
    dh=(float)height/(float)numH;
    
    //node init
    for(int h=0;h<numH;h++){
        for(int w=0;w<numW;w++){
            node.push_back(Node{
                nodeMass,
                false,
                glm::vec3{startX+w*dw,startY,startZ+h*dh}, //pos
                glm::vec3{0,1,0},//normal
                glm::vec3{0,0,0.2f}, //velocity
                glm::vec3{0,0,0} //force
            });
            
        }
    }
    
    node[0].isFixed=true;
    node[numW*(numH-1)].isFixed=true;
    
    //structural spring init
    for(int h=0;h<numH-1;h++){
        for(int w=0;w<numW-1;w++){
            
            Node* first_1= &node[numW*h+w];
            Node* second_1= &node[numW*h+w+1];
            float rest_len_1=glm::distance(first_1->pos, second_1->pos);
            
            spring.push_back(Spring{
                0, //type
                Kstruct, //type
                rest_len_1,//rest len
                first_1,
                second_1
            });
            
            Node* first_2= &node[numW*h+w];
            Node* second_2= &node[numW*(h+1)+w];
            float rest_len_2=glm::distance(first_2->pos,second_2->pos);
            
            spring.push_back(Spring{
                0, //type
                Kstruct, //type
                rest_len_2,//rest len
                first_2,
                second_2
            });
            
        }
    }
    //shear spring init
    for(int h=0;h<numH-1;h++){
        for(int w=0;w<numW-1;w++){
            
            Node* first_1= &node[numW*h+w];
            Node* second_1= &node[numW*(h+1)+w+1];
            float rest_len_1=glm::distance(first_1->pos,second_1->pos);
            
            spring.push_back(Spring{
                1, //type
                Kshear, //type
                rest_len_1,//rest len
                first_1,
                second_1
            });
            
        }
    }
    
    for(int h=0;h<numH-1;h++){
        for(int w=1;w<numW;w++){
            
            Node* first_1= &node[numW*h+w];
            Node* second_1= &node[numW*(h+1)+w-1];
            float rest_len_1=glm::distance(first_1->pos,second_1->pos);
            
            spring.push_back(Spring{
                1, //type
                Kshear, //type
                rest_len_1,//rest len
                first_1,
                second_1
            });
            
        }
    }
    
    //bend spring init
    
    for(int h=0;h<numH;h++){
        for(int w=0;w<numW-2;w++){
            
            Node* first_1= &node[numW*h+w];
            Node* second_1= &node[numW*h+w+2];
            float rest_len_1=glm::distance(first_1->pos,second_1->pos);
            
            spring.push_back(Spring{
                2, //type
                Kbend, //type
                rest_len_1,//rest len
                first_1,
                second_1
            });
            
        }
    }
    
    for(int h=0;h<numH-2;h++){
        for(int w=0;w<numW;w++){
            
            Node* first_1= &node[numW*h+w];
            Node* second_1= &node[numW*(h+2)+w];
            float rest_len_1=glm::distance(first_1->pos,second_1->pos);
            
            spring.push_back(Spring{
                2, //type
                Kbend, //type
                rest_len_1,//rest len
                first_1,
                second_1
            });
            
        }
    }
    
    //make node indices
    for(int h=0;h<numH-1;h++){
        for(int w=0;w<numW-1;w++){
            
            indices.push_back(numW*h+w);
            indices.push_back(numW*(h+1)+w+1);
            indices.push_back(numW*h+w+1);
            
            indices.push_back(numW*h+w);
            indices.push_back(numW*(h+1)+w);
            indices.push_back(numW*(h+1)+w+1);
            
            
        }
    }
    
    //is fixed 설정
    
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, node.size()*sizeof(Node), &node[0], GL_DYNAMIC_DRAW); //size ?
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Node), (void*)offsetof(Node, pos) );
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof(Node), (void*)offsetof(Node,normal) );
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(2);
    glCheckError_(195);
    
    
    glGenBuffers( 1, &EBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    
    
    
}

void Cloth::render(Shader& shader,Light& light, Camera& camera){
    
    
    
    shader.use();
    shader.setLight(light);
    shader.setCamera(camera);
    shader.setUniform("worldMat", M);
    shader.setUniform("Ka", Ka);glCheckError_(137);
    shader.setUniform("Ks", Ks);glCheckError_(138);
    shader.setUniform("Ke", Ke);glCheckError_(139);
    shader.setUniform("sh", sh);glCheckError_(140);
    
    
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    
    glCheckError_(230);
    
    glEnableVertexAttribArray(0);
    glVertexAttrib3f(1,0.7f,0.4f,0.6f); //color
    glEnableVertexAttribArray(2);
    
//        glVertexAttrib3f(2,0.0f,0.0f,1.0f); //normal
    glCheckError_(235);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
    glCheckError_(237);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(2);
    
    
    glBindVertexArray(0);
    
    
}

void Cloth::dump(){
    
    for(int i=0;i<node.size();i++){
        printf("node[%d]: x = %f , y = %f , z = %f \n",i, node[i].pos.x,node[i].pos.y, node[i].pos.z);
    }

    for(int i=0;i<indices.size()/3;i+=6){
        printf("indices1: %d, %d, %d -- indices2:  %d, %d, %d\n", indices[i],indices[i+1],indices[i+2],indices[i+3],indices[i+4],indices[i+5]);
    }
    
//    printf("node[2] x = %f , y = %f , z = %f \n", node[2].pos.x,node[2].pos.y, node[2].pos.z);
    
    
}

void Cloth::update(float dt){
    
  
    
    computeInternalForce();
    applyExternalForce();
    
    for(auto& node_: node){
        

        if(!node_.isFixed){
            glm::vec3 new_velocity=node_.velocity+dt*node_.force/node_.mass;
            glm::vec3 new_pos=node_.pos+dt*node_.velocity;
            
            
            node_.velocity=new_velocity;
            node_.pos=new_pos;
        }
        node_.force=glm::vec3(0);
    }
    collisionCheck();
    computeNormal();
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO);glCheckError_(285);
    glBufferSubData(GL_ARRAY_BUFFER, 0, node.size()*sizeof(Node), &node[0]);
    
    
    
    
}


void Cloth::computeInternalForce(){
    
    for(int i=0;i<spring.size();i++){
        
        glm::vec3 pos1= spring[i].firstNode->pos;
        glm::vec3 pos2= spring[i].secondNode->pos;
        
        glm::vec3 v1= spring[i].firstNode->velocity;
        glm::vec3 v2= spring[i].secondNode->velocity;
 
        glm::vec3 posDiff=pos1-pos2;
        glm::vec3 velDiff=v1-v2;


        float dist = glm::length(posDiff);
        
        

        glm::vec3 spring_force = -spring[i].K * (dist-spring[i].rest_len)*glm::normalize(posDiff);
        glm::vec3 damping_force= -Kd*velDiff;

        
        spring[i].firstNode->force += spring_force + damping_force;
        spring[i].secondNode->force -= spring_force + damping_force;

        
        
        
    }
    
    
    
}

void Cloth::applyExternalForce(){
    
    for(auto& n : node){ //중력
        n.force+=glm::vec3{0.0f,-9.81f,0.0f}*n.mass;
    }
    
    
}

void Cloth::computeNormal(){
    vector<vector<glm::vec3>> normals(node.size());
    for(int i=0;i<indices.size();i+=6){
        
        glm::vec3 v1= node[indices[i+1]].pos-node[indices[i]].pos;
        glm::vec3 v2= node[indices[i+2]].pos-node[indices[i]].pos;
        
        glm::vec3 v3=node[indices[i+4]].pos-node[indices[i+3]].pos;
        glm::vec3 v4=node[indices[i+5]].pos-node[indices[i+3]].pos;
        
        glm::vec3 normal1= glm::cross(v1, v2);
        normal1=glm::normalize(normal1);
        
        glm::vec3 normal2= glm::cross(v3, v4);
        normal2=glm::normalize(normal2);
        
        normals[indices[i]].push_back(normal1);
        normals[indices[i+1]].push_back(normal1);
        normals[indices[i+2]].push_back(normal1);
        normals[indices[i+3]].push_back(normal2);
        normals[indices[i+4]].push_back(normal2);
        normals[indices[i+5]].push_back(normal2);
        
    }
    
    for(int i=0;i<normals.size();i++){
        glm::vec3 avg{0,0,1.f};
       
        for(int j=0;j<normals[i].size();j++){
            avg+=normals[i][j];
           
        }
        avg=glm::normalize(avg);
        node[i].normal=avg; // why minus??
         
    }
    
    
    
    
    
}

void Cloth::collisionCheck(){
    vector<float> sorted(4,0.0f);
    
    for(int i=0;i<node.size();i++){
        //ground check
        if(node[i].pos.y<0.2f){
            node[i].pos.y=0.2f;
            node[i].velocity.y=0.0f;
            
        }
        
        //cube upper face check
        if(node[i].pos.y<2.2f && node[i].pos.x<5.2f && node[i].pos.x>2.8f && node[i].pos.z>1.8f && node[i].pos.z<4.2f){

            
            
            float dis_front= abs(node[i].pos.x- 5.2f) ;
            float dis_upper=abs(node[i].pos.y- 2.2f);
            float dis_left=abs(node[i].pos.z- 4.2f) ;
            float dis_right=abs(node[i].pos.z- 1.8f) ;
            sorted[0]=dis_front;
            sorted[1]=dis_upper;
            sorted[2]=dis_left;
            sorted[3]=dis_right;
            
            sort(sorted.begin(),sorted.end());
            
            if(sorted[0]==dis_front){
                node[i].velocity.x=0.0f;
            }else if(sorted[0]==dis_upper){
                node[i].velocity.y=0.0f;
            }else if(sorted[0]==dis_left){
                node[i].velocity.z=0.0f;
            }else{
                node[i].velocity.z=0.0f;
            }
            
            
            
            
            
            
            
            
        }
        
        
        
 
        
        
        
    }
    
    
    
    
    
    
}

