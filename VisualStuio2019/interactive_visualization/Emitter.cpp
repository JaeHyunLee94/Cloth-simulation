

#include "Emitter.hpp"

Emitter::Emitter(){
    
};
Emitter::Emitter(Camera camera,Shader& shader){
    emitterPos=glm::vec3{0,2,2};
    ParticlesContainer.reserve(MaxParticles);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    programID = shader.getProgramId();
    
    // Vertex shader
    CameraRight_worldspace_ID  = glGetUniformLocation(programID, "CameraRight_worldspace");
    CameraUp_worldspace_ID  = glGetUniformLocation(programID, "CameraUp_worldspace");
    ViewProjMatrixID = glGetUniformLocation(programID, "VP");
    
    // fragment shader
    TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    g_particule_position_size_data = new GLfloat[MaxParticles * 4];
    g_particule_color_data         = new GLubyte[MaxParticles * 4];
    
    for(int i=0; i<MaxParticles; i++){
        ParticlesContainer[i].life = -1.0f;
        ParticlesContainer[i].cameradistance = -1.0f;
    }
    
    ProjectionMatrix = camera.getProjectionMatrix();
    ViewMatrix = camera.getViewMatrix();

    // We will need the camera's position in order to sort the particles
    // w.r.t the camera's distance.
    // There should be a getCameraPosition() function in common/controls.cpp,
    // but this works too.
    CameraPosition= glm::vec3(glm::inverse(ViewMatrix)[3]);

    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    
    
    Texture = loadDDS("./texture/particle.DDS");
    
    // The VBO containing the 4 vertices of the particles.
    // Thanks to instancing, they will be shared by all particles.
    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
    };
    
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    // The VBO containing the positions and sizes of the particles
    
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    
    // The VBO containing the colors of the particles
    
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    glCheckError_(69);
}

void Emitter::set(Camera camera,Shader& shader){
    emitterPos=glm::vec3{0,2,2};
    ParticlesContainer.reserve(MaxParticles);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    programID = shader.getProgramId();
    
    // Vertex shader
    CameraRight_worldspace_ID  = glGetUniformLocation(programID, "CameraRight_worldspace");
    CameraUp_worldspace_ID  = glGetUniformLocation(programID, "CameraUp_worldspace");
    ViewProjMatrixID = glGetUniformLocation(programID, "VP");
    
    // fragment shader
    TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    g_particule_position_size_data = new GLfloat[MaxParticles * 4];
    g_particule_color_data         = new GLubyte[MaxParticles * 4];
    
    for(int i=0; i<MaxParticles; i++){
        ParticlesContainer[i].life = -1.0f;
        ParticlesContainer[i].cameradistance = -1.0f;
    }
    
    ProjectionMatrix = camera.getProjectionMatrix();
    ViewMatrix = camera.getViewMatrix();

    // We will need the camera's position in order to sort the particles
    // w.r.t the camera's distance.
    // There should be a getCameraPosition() function in common/controls.cpp,
    // but this works too.
    CameraPosition= glm::vec3(glm::inverse(ViewMatrix)[3]);

    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    
    
    Texture = loadDDS("../../../interactive_visualization2/texture/particle.DDS");
    
    // The VBO containing the 4 vertices of the particles.
    // Thanks to instancing, they will be shared by all particles.
    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
    };
    
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    // The VBO containing the positions and sizes of the particles
    
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    
    // The VBO containing the colors of the particles
    
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    glCheckError_(69);
    
}

int Emitter::FindUnusedParticle(){
    
    for(int i=LastUsedParticle; i<MaxParticles; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }
    
    for(int i=0; i<LastUsedParticle; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }
    
    return 0; // All particles are taken, override the first one
}

void Emitter::SortParticles(){
    std::sort(ParticlesContainer.begin(), ParticlesContainer.end());
}


void Emitter::update(float dt){
    
    int newparticles = (int)(dt*10000.0);
    if (newparticles > (int)(0.016f*10000.0))
        newparticles = (int)(0.016f*10000.0);
    
    for(int i=0; i<newparticles; i++){
        int particleIndex = FindUnusedParticle();
        ParticlesContainer[particleIndex].life = MaxLifeTime; // This particle will live 5 seconds.
        ParticlesContainer[particleIndex].pos = emitterPos;

        float spread = 1.5f;
        glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
        // Very bad way to generate a random direction;
        // See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
        // combined with some user-controlled parameters (main direction, spread, etc)
        glm::vec3 randomdir = glm::vec3(
            (rand()%2000 - 1000.0f)/1000.0f,
            (rand()%2000 - 1000.0f)/1000.0f,
            (rand()%2000 - 1000.0f)/1000.0f
        );
        
        ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;


        // Very bad way to generate a random color
        ParticlesContainer[particleIndex].r = 255;// rand() % 256;
        ParticlesContainer[particleIndex].g = 255;// rand() % 256;
        ParticlesContainer[particleIndex].b = 255;// rand() % 256;
        ParticlesContainer[particleIndex].a = 255;//(rand() % 256)/3;

        ParticlesContainer[particleIndex].size = (rand()%1000)/20000.0f + 0.005f;
        
    }
    // Simulate all particles
    ParticlesCount = 0;
    for(int i=0; i<MaxParticles; i++){

        Particle& p = ParticlesContainer[i]; // shortcut

        if(p.life > 0.0f){

            // Decrease life
            p.life -= dt;
            if (p.life > 0.0f){

                // Simulate simple physics : gravity only, no collisions
                p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)dt * 0.5f;
                p.pos += p.speed * (float)dt;
                p.cameradistance = glm::length2( p.pos - CameraPosition );
                //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                // Fill the GPU buffer
                g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
                g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
                g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;
                                               
                g_particule_position_size_data[4*ParticlesCount+3] = p.size;
                                               
                g_particule_color_data[4*ParticlesCount+0] = p.r;
                g_particule_color_data[4*ParticlesCount+1] = p.g;
                g_particule_color_data[4*ParticlesCount+2] = p.b;
                g_particule_color_data[4*ParticlesCount+3] = p.a;

            }else{
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p.cameradistance = -1.0f;
            }

            ParticlesCount++;

        }
    }

    SortParticles();
}


void Emitter::render(){
    glCheckError_(175);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);
    glCheckError_(185);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCheckError_(189);
    // Use our shader
    glUseProgram(programID);
    glCheckError_(192);
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(TextureID, 0);
    glCheckError_(198);
    // Same as the billboards tutorial
    glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
    glUniform3f(CameraUp_worldspace_ID   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

    glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
    glCheckError_(204);
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    
    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        4,                                // size : x + y + z + size => 4
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : particles' colors
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glVertexAttribPointer(
        2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        4,                                // size : r + g + b + a => 4
        GL_UNSIGNED_BYTE,                 // type
        GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // These functions are specific to glDrawArrays*Instanced*.
    // The first parameter is the attribute buffer we're talking about.
    // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
    // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

    // Draw the particules !
    // This draws many times a small triangle_strip (which looks like a quad).
    // This is equivalent to :
    // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
    // but faster.
    glCheckError_(254);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}


void Emitter::setEmitterPos(glm::vec3 pos){
    emitterPos=pos;
}




////
////  Emitter.cpp
////  interactive_visualization2
////
////  Created by 이재현 on 2020/12/01.
////
//
//#include "Emitter.hpp"
//
//
//
//Emitter::Emitter(){
//    init();
//}
//
//void Emitter::init(){
//
//    srand((unsigned int )time(0));
//
//    centerPosition=glm::vec3{-2,1,3};
//    MAX_LIFE_TIME=2.0f;
//    MAX_PARTICE_NUM=100;
//
//    for (unsigned int i = 0; i < MAX_PARTICE_NUM; ++i)
//    particles.push_back(Particle{
//        centerPosition,
//        glm::vec3{(rand()%2000 - 1000.0f)/1000.0f,(rand()%2000)/1000.0f,(rand()%2000 - 1000.0f)/1000.0f}, //random 하게 바꾸기
//        glm::vec3{(rand()%100)/100.0f,(rand()%100)/100.0f,(rand()%100)/100.0f},
//        MAX_LIFE_TIME
//    });
//
//    glCheckError_(32);
//    glGenVertexArrays(1,&VAO);
//    glBindVertexArray(VAO);
//
//    glGenBuffers(1,&VBO);
//    glBindBuffer(GL_ARRAY_BUFFER,VBO);
//    glBufferData(GL_ARRAY_BUFFER,sizeof(Particle)*particles.size(),&particles[0],GL_DYNAMIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Particle),(void*) offsetof(Particle,pos));
//
//    glCheckError_(42);
//}
//
//unsigned int Emitter::FirstUnusedParticle()
//{
//    // search from last used particle, this will usually return almost instantly
//    for (unsigned int i = lastUsedParticle; i < MAX_PARTICE_NUM; ++i) {
//        if (particles[i].life <= 0.0f){
//            lastUsedParticle = i;
//            return i;
//        }
//    }
//    // otherwise, do a linear search
//    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
//        if (particles[i].life <= 0.0f){
//            lastUsedParticle = i;
//            return i;
//        }
//    }
//    // override first particle if all others are alive
//    lastUsedParticle = 0;
//    return 0;
//}
//
//void Emitter::update(float dt){
//
//    // add new particles
//    for (unsigned int i = 0; i < new_particles_num; ++i)
//    {
//        int unusedParticle = FirstUnusedParticle();
//        RespawnParticle(unusedParticle);
//    }
//    // update all particles
//    for (unsigned int i = 0; i < MAX_PARTICE_NUM; ++i)
//    {
//        Particle &p = particles[i];
//        p.life -= dt; // reduce life
//        if (p.life > 0.0f)
//        {    // particle is alive, thus update
//            p.pos -= p.velocity * dt;
//            p.uva.z -= dt * 2.5f;
//        }
//    }
//
//    //glsubdata
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER,VBO);
//    glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size()*sizeof(Particle), &particles[0]);
//
//
//}
//
//void Emitter::RespawnParticle(int index){
//    particles[index].pos=centerPosition;
//    particles[index].velocity=glm::vec3 {0,1,0};
//    particles[index].uva= glm::vec3{(rand()%100)/100.0f,(rand()%100)/100.0f,(rand()%100)/100.0f};
//    particles[index].life=MAX_LIFE_TIME;
//
//
//}
//
//
//void Emitter::render(Shader& shader)
//{
//    // use additive blending to give it a 'glow' effect
//    glEnable( GL_PROGRAM_POINT_SIZE );
//    glPointSize(10);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//    shader.use();
//
//    glDrawArrays(GL_POINTS, 0, particles.size());
////    for (Particle particle : this->particles)
////    {
////        if (particle.life > 0.0f)
////        {
////            glCheckError_(110);
////            glBindVertexArray(VAO);
////            glBindBuffer(GL_ARRAY_BUFFER,VBO);
////            glEnableVertexAttribArray(0);
////            glCheckError_(114);
////            glDrawArrays(GL_POINTS, 0, 1);
////            glCheckError_(116);
////            glBindVertexArray(0);
////
////        }
////    }
////    // don't forget to reset to default blending mode
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDisable(GL_PROGRAM_POINT_SIZE);
//}
