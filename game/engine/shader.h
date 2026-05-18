// COMP710 GP Framework 2025
#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"
#include "structs/matrix4.h"

namespace Engine {
    class Shader
    {
        // Member methods:
    public:
        Shader();
        ~Shader();
        bool Load(const char* vertexFile, const char* pixelFile);
        void Unload() const;
        void SetActive() const;
        void SetMatrixUniform(const char* name, const Matrix4& matrix) const;
        void SetVector4Uniform(const char* name, float x, float y, float z, float w) const;
    private:
        [[nodiscard]] bool IsValidProgram() const;
        static bool CompileShader(const char* filename, GLenum shaderType, GLuint& outShader);
        static bool IsCompiled(GLuint shader);
        // Member data:
    public:
    protected:
        GLuint m_vertexShader;
        GLuint m_pixelShader;
        GLuint m_shaderProgram;
    private:
    };
}
#endif // SHADER_H