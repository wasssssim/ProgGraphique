#ifndef SHADERPROGRAM_H
# include <GL/glew.h>


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

    private:
    string FileToString(const char *vsfileName);
    void checkCompileErrors(GLuint shader, ShaderType type);
    GLuint mHandle;
};


#endif // SHADERPROGRAM_H