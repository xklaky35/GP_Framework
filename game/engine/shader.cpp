#include "shader.h"
#include "logmanager/logmanager.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

namespace Engine {
    Shader::Shader()
        : m_vertexShader(0)
          , m_pixelShader(0)
          , m_shaderProgram(0) {
    }

    Shader::~Shader() {
        Unload();
    }

    bool Shader::Load(const char *vertexFile, const char *pixelFile) {
        const bool vertexCompiled = CompileShader(vertexFile, GL_VERTEX_SHADER, m_vertexShader);
        const bool pixelCompiled = CompileShader(pixelFile, GL_FRAGMENT_SHADER, m_pixelShader);
        if (vertexCompiled == false || pixelCompiled == false) {
            LogManager::GetInstance().Log(ERROR, "Shaders failed to compile!");
            return false;
        }
        m_shaderProgram = glCreateProgram();
        glAttachShader(m_shaderProgram, m_vertexShader);
        glAttachShader(m_shaderProgram, m_pixelShader);
        glLinkProgram(m_shaderProgram);
        return IsValidProgram();
    }

    void Shader::Unload() const {
        glDeleteProgram(m_shaderProgram);
        glDeleteShader(m_vertexShader);
        glDeleteShader(m_pixelShader);
    }

    void Shader::SetActive() const {
        assert(m_shaderProgram);
        glUseProgram(m_shaderProgram);
    }

    void Shader::SetMatrixUniform(const char *name, const Matrix4 &matrix) const {
        GLuint location = glGetUniformLocation(m_shaderProgram, name);
        glUniformMatrix4fv(static_cast<int>(location), 1, GL_TRUE, (float*)(&matrix));
    }

    void Shader::SetVector4Uniform(const char *name, float x, float y, float z, float w) const {
        GLuint location = glGetUniformLocation(m_shaderProgram, name);
        float vec4[4];
        vec4[0] = x;
        vec4[1] = y;
        vec4[2] = z;
        vec4[3] = w;
        glUniform4fv(static_cast<int>(location), 1, vec4);
    }

    bool Shader::CompileShader(const char *filename, GLenum shaderType, GLuint &outShader) {
        std::ifstream shaderFile(filename);
        if (shaderFile.is_open()) {
            std::stringstream sstream;
            sstream << shaderFile.rdbuf();
            std::string shaderCode = sstream.str();
            const char *pShaderCode = shaderCode.c_str();
            outShader = glCreateShader(shaderType);
            glShaderSource(outShader, 1, &(pShaderCode), nullptr);
            glCompileShader(outShader);
            if (!IsCompiled(outShader)) {
                LogManager::GetInstance().Log(ERROR, "Shader failed to compile!");
                return false;
            }
        } else {
            LogManager::GetInstance().Log(ERROR, "Shader file not found!");
            return false;
        }
        return true;
    }

    bool Shader::IsCompiled(GLuint shader) {
        GLint compiledStatus;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledStatus);
        if (compiledStatus != GL_TRUE) {
            char error[1024];
            error[0] = 0;
            glGetShaderInfoLog(shader, 1023, nullptr, error);
            LogManager::GetInstance().Log(ERROR, "Shader failed to compile!");
            return false;
        }
        return true;
    }

    bool Shader::IsValidProgram() const {
        GLint linkedStatus;
        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &linkedStatus);
        if (linkedStatus != GL_TRUE) {
            char error[1024];
            error[0] = 0;
            glGetShaderInfoLog(m_shaderProgram, 1023, nullptr, error);
            LogManager::GetInstance().Log(ERROR, "Shader failed to link!");
            return false;
        }
        return true;
    }
}
