#include "Shader.hpp"
#include <iostream>
#include <vector>
#include <fstream>

Shader::Shader(std::string vertex, std::string fragment) : vertexShaderPath(vertex), fragmentShaderPath(fragment)
{
    GLuint vertexShaderId = loadShader(vertex, GL_VERTEX_SHADER);
    GLuint fragmentShaderId = loadShader(fragment, GL_FRAGMENT_SHADER);

    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glBindAttribLocation(programId, 0, "vertexPosition");
    glBindAttribLocation(programId, 1, "vertexColor");
    glBindAttribLocation(programId, 2, "vertexTexCoords");

    glLinkProgram(programId);
    glValidateProgram(programId);

    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

Shader::~Shader()
{
    unuse();
    glDeleteProgram(programId);
}

GLuint Shader::loadShader(std::string fileName, GLuint type)
{
    std::string shaderSource;
    std::ifstream f(fileName, std::ios::binary);
    if (f.fail()) {
        std::cout << "Error reading file " << fileName << "\n";
        exit(1);
    }
    f.seekg(0, std::ios::end);
    std::streamoff fileSize = f.tellg();
    f.seekg(0, std::ios::beg);
    fileSize -= f.tellg();
    shaderSource.resize((size_t) fileSize);
    f.read(&(shaderSource[0]), fileSize);
    f.close();
    GLuint id = glCreateShader(type);
    const char* shaderSourceCstr = shaderSource.c_str();
    glShaderSource(id, 1, &shaderSourceCstr, nullptr);
    glCompileShader(id);
    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
        glDeleteShader(id); //Don't leak the shader.
        std::printf("%s\n", &(errorLog[0]));
        std::cout << "Error : Shader failed to compile\n";
        exit(1);
    }
    return id;
}
