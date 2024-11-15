#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>
#include "glm/glm.hpp"
#include <GL/glew.h>
#include "libraries/stb-master/stb_image.h"
#include <iostream>


class cieltexture{
// Fonction pour charger les textures du Skybox
   public : 
       unsigned int loadSkybox();

       // Fonction pour configurer les positions et buffers du Skybox
       unsigned int setupSkyboxVAO();
   void render(const glm::mat4& view, const glm::mat4& projection);

   std::vector<std::string> getFaces();
   GLuint loadCubemap(); 


   
   private: 
       std::vector<std::string> faces = {
           "textures/px.png",   // Positif X
           "textures/nx.png",   // Négatif X
           "textures/py.png",   // Positif Y
           "textures/ny.png",   // Négatif Y
           "textures/pz.png",   // Positif Z
           "textures/nz.png"    // Négatif Z
       };
}; 

#endif