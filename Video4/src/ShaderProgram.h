#ifndef SHADERPROGRAM_H
# include <GL/glew.h>
#include "glm/glm.hpp"
#include <map>


#include <string>
using namespace std;
class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();
    enum ShaderType
    {
        VERTEX,
        FRAGMENT_SHADER,
        PROGRAM
    };
    bool loadShaders(const char *vsFileNamz, const char *fsFileName);
    void use();
    void setUniform(const GLchar *name, const glm::vec2 &v);
    void setUniform(const GLchar *name, const glm::vec3 &v);
    void setUniform(const GLchar *name, const glm::vec4 &v);

    private:

    string FileToString(const char *vsfileName);

    void checkCompileErrors(GLuint shader, ShaderType type);

    GLuint mHandle;
    GLint getUniformLocation(const GLchar *name);

    std::map<string, int> mUniformLocations;
};


#endif // SHADERPROGRAM_H