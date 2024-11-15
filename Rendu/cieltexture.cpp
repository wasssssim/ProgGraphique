#include <vector>
#include <string>
#include <GL/glew.h>
#include "libraries/stb-master/stb_image.h"
#include <iostream>
#include "cieltexture.hpp"
#include "ShaderProgram.h"


std::vector<std::string> cieltexture::getFaces()
{
   return faces; 
}

unsigned int cieltexture::loadSkybox()
{
   unsigned int textureID;
   glGenTextures(1, &textureID);
   glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

   int width, height, nrChannels;
   for (unsigned int i = 0; i < faces.size(); i++)
   {
       unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
       if (data)
       {
           glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                        0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
           stbi_image_free(data);
       }
       else
       {
           std::cout << "Échec du chargement de la texture : " << faces[i] << std::endl;
           stbi_image_free(data);
       }
   }

   // Paramètres de la texture
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_RGB);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_RGB);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

   return textureID;
}

// Fonction pour configurer les positions et buffers du Skybox
unsigned int cieltexture::setupSkyboxVAO()
{
   float skyboxVertices[] = {
       // Positions
       -1.0f,  1.0f, -1.0f, // Face arrière
       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,

       -1.0f, -1.0f,  1.0f, // Face avant
       -1.0f, -1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f, // Face droite
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

       -1.0f, -1.0f,  1.0f, // Face gauche
       -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f,

       -1.0f,  1.0f, -1.0f, // Face supérieure
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f, -1.0f,

       -1.0f, -1.0f, -1.0f, // Face inférieure
       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
   };

   unsigned int skyboxVAO, skyboxVBO;
   glGenVertexArrays(1, &skyboxVAO);
   glGenBuffers(1, &skyboxVBO);

   glBindVertexArray(skyboxVAO);
   glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

   return skyboxVAO;
}


unsigned int cieltexture::loadCubemap()
{
   unsigned int textureID;
   glGenTextures(1, &textureID);
   glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

   int width, height, nrChannels;
   for (unsigned int i = 0; i < faces.size(); i++)
   {
       unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
       
       if (data)
       {
           glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                        0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
           );
           stbi_image_free(data);
       }
       else
       {
           std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
           stbi_image_free(data);
       }
   }
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

   return textureID;
} 

void cieltexture::render(const glm::mat4& view, const glm::mat4& projection)
{
    static ShaderProgram skyboxShader;
    static bool shaderLoaded = false;

    if (!shaderLoaded) {
        // Chargez le shader du skybox (assurez-vous d'avoir les fichiers correspondants)
        skyboxShader.loadShaders("shaders/cieltexture.vert", "shaders/cieltexture.frag");
        shaderLoaded = true;
    }

    skyboxShader.use();
    skyboxShader.setUniform("view", view);
    skyboxShader.setUniform("projection", projection);

    glBindVertexArray(setupSkyboxVAO());
    glBindTexture(GL_TEXTURE_CUBE_MAP, loadSkybox());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}