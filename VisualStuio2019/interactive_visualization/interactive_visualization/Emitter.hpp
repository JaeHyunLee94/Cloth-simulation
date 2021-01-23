#include <GL/glew.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include "texture.hpp"
#include "Shader.h"
#include "Camera.h"
#include "error_check.h"
using namespace glm;



struct Particle{
    glm::vec3 pos, speed;
    unsigned char r,g,b,a; // Color
    float size, angle, weight;
    float life; // Remaining life of the particle. if <0 : dead and unused.
    float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

    bool operator<(const Particle& that) const {
        // Sort in reverse order : far particles drawn first.
        return this->cameradistance > that.cameradistance;
    }
};



class Emitter{
    
public:
    Emitter();
    Emitter(Camera camera ,Shader& shader);
    int MaxParticles = 100000;
    
    GLuint VAO;
    GLuint VBO;
    GLuint programID;
    GLuint CameraRight_worldspace_ID ;
    GLuint CameraUp_worldspace_ID ;
    GLuint ViewProjMatrixID ;
    GLuint TextureID;
    GLfloat* g_particule_position_size_data ;
    GLubyte* g_particule_color_data;
    GLuint billboard_vertex_buffer;
    GLuint particles_position_buffer;
    GLuint particles_color_buffer;
    GLuint Texture;
    glm::mat4 ProjectionMatrix ;
    glm::mat4 ViewMatrix;
    int ParticlesCount;
    float MaxLifeTime=5.0f;
    glm::vec3 emitterPos;
    

    // We will need the camera's position in order to sort the particles
    // w.r.t the camera's distance.
    // There should be a getCameraPosition() function in common/controls.cpp,
    // but this works too.
    glm::vec3 CameraPosition;

    glm::mat4 ViewProjectionMatrix;
    
    std::vector<Particle> ParticlesContainer;
   
    int LastUsedParticle = 0;

    
    int FindUnusedParticle();
    void SortParticles();
    void update(float dt);
    void render();
    void setEmitterPos(glm::vec3 pos);
    void set(Camera camera,Shader& shader);
    
    
};


























////
////  Emitter.hpp
////  interactive_visualization2
////
////  Created by 이재현 on 2020/12/01.
////
//
//#ifndef Emitter_hpp
//#define Emitter_hpp
//
//#include <GL/glew.h>
//#include <glm/glm.hpp>
//#include <vector>
//#include <algorithm>
//#include <stdio.h>
//#include <iostream>
//#include <ctime>
//#include "Shader.h"
//#include "Camera.h"
//#include "Light.h"
//#include "error_check.h"
//
//using namespace std;
//
//struct Particle{
//
//
//
//    glm::vec3 pos;
//    glm::vec3 velocity;
//    glm::vec3 uva;
//    float life;
//
//
//};
//
//
//class Emitter{
//
//
//public:
//
//
//    GLuint VAO;
//    GLuint VBO;
//
//    float MAX_LIFE_TIME;
//    unsigned int MAX_PARTICE_NUM;
//    glm::vec3 centerPosition;
//    unsigned int lastUsedParticle = 0;
//    unsigned int new_particles_num = 2;
//    vector<Particle> particles;
//
//
//    Emitter();
//    void update(float dt);
//    void render(Shader& shader);
//    void init();
//    void RespawnParticle(int index);
//    unsigned int FirstUnusedParticle();
//
//
//
//};
//
//#endif /* Emitter_hpp */
