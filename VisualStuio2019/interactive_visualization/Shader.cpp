//
// Created by 이재현 on 2020/08/28.
//

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Shader.h"
#include <string>


Shader::Shader(){
    
}

Shader::Shader(std::string vertexShaderPath_arg, std::string fragmentShaderPath_arg) {

    vertexShaderPath = vertexShaderPath_arg;
    fragmentShaderPath = fragmentShaderPath_arg;

    setVertexShader(vertexShaderPath);
    setFragmentShader(fragmentShaderPath);
    makeProgram();
}

void Shader::set(std::string vertexShaderPath_arg, std::string fragmentShaderPath_arg){
    vertexShaderPath = vertexShaderPath_arg;
    fragmentShaderPath = fragmentShaderPath_arg;

    setVertexShader(vertexShaderPath);
    setFragmentShader(fragmentShaderPath);
    makeProgram();
}

int Shader::setVertexShader(std::string vertexShaderPath) {

    this->vertexShaderPath = vertexShaderPath;
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);


    return 0;
}

int Shader::setFragmentShader(std::string fragmentShaderPath) {

    this->fragmentShaderPath = fragmentShaderPath;
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    return 0;
}

int Shader::loadSource() {
    // Read the Vertex Shader code from the file

    std::ifstream VertexShaderStream(vertexShaderPath.c_str(), std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        this->vertexShaderCode = sstr.str();
        VertexShaderStream.close();
    } else {

        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n",
               vertexShaderPath.c_str());
        getchar();
        return 0;
    }


    // Read the Fragment Shader code from the file

    std::ifstream FragmentShaderStream(fragmentShaderPath, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        this->fragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    } else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n",
               fragmentShaderPath.c_str());
        getchar();
        return 0;
    }
    return 1;
}

int Shader::compileShader() {
    GLint Result = GL_FALSE;
    int InfoLogLength;
    // Compile Vertex Shader
    std::cout << "Compiling shader : " << vertexShaderPath << "\n";

    char const *VertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(vertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
        return 0;
    }



    // Compile Fragment Shader
    std::cout << "Compiling shader : " << fragmentShaderPath << "\n";
    char const *FragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(fragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
        return 0;
    }
    return 1;
}

int Shader::makeProgram() {

    loadSource();
    compileShader();
    programID=glCreateProgram();
    // Link the program
    std::cout << "Linking program...\n";
    GLint Result = GL_FALSE;;
    int InfoLogLength;

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
        return 0;
    }


    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return 1;
}

Shader::~Shader() {
    glDeleteProgram(programID);
}


void Shader::use() {
    glUseProgram(programID);

}

GLuint Shader::getProgramId() {
    return programID;
}



void Shader::setLight(Light& light) {
    /*
     * light possition , 광원 색깔 정의
     */
    
    setUniform("lightdir",light.direction);
    glCheckError_(1);
    setUniform("Sd",light.diffColor);
    glCheckError_(2);
    setUniform("Sa",light.ambColor);
    glCheckError_(3);
    setUniform("Ss",light.specColor);
    glCheckError_(4);

}

void Shader::setCamera(Camera& camera) {
    /*
     * eye loc, V, P transform 전달
     * worldMat,viewMat,projMat;
     */
    setUniform("eyepos",camera.eye);
    setUniform("viewMat",camera.getViewMatrix());
    setUniform("projMat",camera.getProjectionMatrix());
    

}

void Shader::setUniform(std::string name, float f) {
    GLint loc= glGetUniformLocation(programID,name.c_str());
    glUniform1f(loc,f);
}

void Shader::setUniform(std::string name, glm::vec3 v3) {
    GLint loc= glGetUniformLocation(programID,name.c_str());
    glUniform3f(loc,v3.x,v3.y,v3.z);
}

void Shader::setUniform(std::string name, glm::mat4x4 m4) {
    GLint loc= glGetUniformLocation(programID,name.c_str());
    glUniformMatrix4fv(loc,1,GL_FALSE,&m4[0][0]);
}

void Shader::setUniform(std::string name, glm::mat3x3 m3) {
    GLint loc= glGetUniformLocation(programID,name.c_str());
    glUniformMatrix3fv(loc,1,GL_FALSE,&m3[0][0]);
}

void Shader::setTexture(std::string name, GLuint textureID, int TEXTURE_UNIT) {
    glActiveTexture(GL_TEXTURE0+TEXTURE_UNIT);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(glGetUniformLocation(programID,name.c_str()), TEXTURE_UNIT);
}

