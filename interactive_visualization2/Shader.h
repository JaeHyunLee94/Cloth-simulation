//
// Created by 이재현 on 2020/08/28.
//

#ifndef OPENGL_PLAYGROUND_SHADER_H
#define OPENGL_PLAYGROUND_SHADER_H


#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include "Light.h"
#include "Camera.h"


class Shader {

public:

    //uniform 얻는 함수 만들, texture load 함수 만들
    //setTexture, setfloat, set vec3...

    Shader(std::string vertexShaderPath_arg, std::string fragmentShaderPath_arg);

    void setUniform(std::string name,glm::vec3 v3);
    void setUniform(std::string name,glm::mat4x4 m4);
    void setUniform(std::string name,glm::mat3x3 m3);
    void setUniform(std::string name,float f);
    
    void setLight(Light& light);

    void setCamera(Camera& camera);

    void setTexture(std::string name,GLuint textureID, int TEXTURE_UNIT);
    int setVertexShader(std::string vertexShaderPath);
    int setFragmentShader(std::string fragmentShaderPath);

    int loadSource();

    int compileShader();

    GLuint getProgramId();

    int makeProgram();


    void use();

    ~Shader();


private:

    GLuint programID;
    GLuint fragmentShaderID;
    GLuint vertexShaderID;
    std::string vertexShaderCode;
    std::string fragmentShaderCode;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;


};


#endif //OPENGL_PLAYGROUND_SHADER_H

