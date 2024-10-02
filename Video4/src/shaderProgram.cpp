#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>




ShaderProgram::ShaderProgram()
: mHandle(0)
{


}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(mHandle);
}

    bool ShaderProgram :: loadShaders(const char *vsFileName, const char *fsFileName)
    
    {
        string vsString = FileToString(vsFileName); // Lire le fichier
        string fsString = FileToString(fsFileName); 

        const char *VsSourcePtr = vsString.c_str(); // Convertir en chaîne de caractères
        const char *fsSourcePtr = fsString.c_str();

        GLuint vs = glCreateShader(GL_VERTEX_SHADER); // Créer un shader de sommet
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER); // Créer un shader de fragment

        glShaderSource(fs,1,&fsSourcePtr,NULL);
        glShaderSource(vs,1,&VsSourcePtr,NULL); // Charger le code source du shader


        glCompileShader(vs); // Compiler le shader
        glCompileShader(fs);

        checkCompileErrors(vs,ShaderType::VERTEX); // Vérifier les erreurs de compilation
        checkCompileErrors(fs,ShaderType::FRAGMENT_SHADER);




    mHandle = glCreateProgram(); // Créer un programme de shader


    glAttachShader(mHandle,vs); // Attacher le shader de sommet
    glAttachShader(mHandle,fs); // Attacher le shader de fragment

    glLinkProgram(mHandle); // Lier le programme de shader

    checkCompileErrors(mHandle,ShaderType::PROGRAM); // Vérifier les erreurs de liaison


    glDeleteShader(vs);
    glDeleteShader(fs);

        return true; 




    }
    ;
    void ShaderProgram:: use()
    
    {
        if(mHandle > 0)
        {
            glUseProgram(mHandle); // Utiliser le programme
        }




    };
    string ShaderProgram:: FileToString(const char *fileName)
    
    {

            std::stringstream ss;
            std::ifstream file;

            try{
                    file.open(fileName, std::ios::in);  // Ouverture du fichier
                    if(!file.fail())
                    {
                        ss << file.rdbuf();  // Lecture du fichier
                    }

                    file.close();  // Fermeture du fichier
            }
            catch(std::exception &e)
            {
                std::cerr << "Erreur lors de lecture du nom du fichiers des Shader" << std::endl;
                return "";
            }
            return ss.str();
    }
    ;
    void ShaderProgram :: checkCompileErrors(GLuint shader, ShaderType type)
    {

        int status=0;
        if(type == PROGRAM)
        {
            glGetProgramiv(shader,GL_LINK_STATUS,&status);
            if(status == GL_FALSE)
            {
                GLint length = 0;
                glGetProgramiv(shader,GL_INFO_LOG_LENGTH,&length);
                string errorLog(length,' ');
                glGetProgramInfoLog(shader,length,&length,&errorLog[0]);
                std::cerr << "Erreur! Le programme de shader n'a pas pu être lié. " << errorLog << std::endl;
            }
        }
        else // VERTEX ou FRAGMENT
        {
            glGetShaderiv(shader,GL_COMPILE_STATUS,&status);
            if(status == GL_FALSE)
            {
                GLint length = 0;
                glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&length);
                string errorLog(length,' ');
                glGetShaderInfoLog(shader,length,&length,&errorLog[0]);
                std::cerr << "Erreur! Le shader n'a pas pu être compilé. " << errorLog << std::endl;
            }
        }

    }
        GLint ShaderProgram:: getUniformLocation(const GLchar *name){
            std::map<string,int>::iterator it = mUniformLocations.find(name);
            if(it == mUniformLocations.end()){
                mUniformLocations[name] = glGetUniformLocation(mHandle,name);
            }
            return mUniformLocations[name];
        }

    
        void ShaderProgram::setUniform(const GLchar *name, const glm::vec2 &v){
            GLint loc = getUniformLocation(name);
            glUniform2f(loc,v.x,v.y);

        }   
        void ShaderProgram::setUniform(const GLchar *name, const glm::vec3 &v){
            GLint loc = getUniformLocation(name);
            glUniform3f(loc,v.x,v.y,v.z);

        }
        void ShaderProgram::setUniform(const GLchar *name, const glm::vec4 &v){
            GLint loc = getUniformLocation(name);
            glUniform4f(loc,v.x,v.y,v.z,v.w);

        }

