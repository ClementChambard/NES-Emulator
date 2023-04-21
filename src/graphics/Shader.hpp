#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

class Shader {
    public:
        Shader(std::string vertex, std::string fragment);
        ~Shader();

        void use() { glUseProgram(programId); }
        void unuse() { glUseProgram(0); }

        GLuint getUniformLocation(std::string uniformName) { return glGetUniformLocation(programId, uniformName.c_str()); }
        void loadMat4(GLuint location, const glm::mat4& value) { glUniformMatrix4fv(location, 1, GL_FALSE, &(value[0][0])); }

    private:

        GLuint programId = 0;

        const std::string vertexShaderPath;
        const std::string fragmentShaderPath;

        static GLuint loadShader(std::string file, GLenum type);
};

#endif // SHADER_H_
