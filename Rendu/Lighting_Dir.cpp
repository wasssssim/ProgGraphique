//-----------------------------------------------------------------------------
// Includes and Defines
//-----------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <fstream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture2D.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Camera.h"

// Global Variables
const char* APP_TITLE = "Blé Automatique";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow* gWindow = NULL;
glm::vec4 gClearColor(0.06f, 0.06f, 0.07f, 1.0f);
FPSCamera fpsCamera(glm::vec3(0.0f, 10.5f, 10.0f));

// OpenGL settings
bool initOpenGL();

// Function prototypes
void showFPS(GLFWwindow* window);
void update(double elapsedTime);
void ajouterBle(int nombreBle, float espacement, const std::string& nomModele, const std::string& nomTexture);

// Vectors to store Mesh, Texture, positions, and scales
std::vector<Mesh> mesh;
std::vector<Texture2D> texture;
std::vector<glm::vec3> positions;
std::vector<glm::vec3> echelles;

int main() {
    if (!initOpenGL()) {
        std::cerr << "Échec de l'initialisation de GLFW" << std::endl;
        return -1;
    }

    ShaderProgram lightingShader;
    lightingShader.loadShaders("shaders/lighting.vert", "shaders/lighting.frag");

    // Appel de la fonction pour ajouter des instances de blé
    ajouterBle(100, 2.0f, "ble", "ble.png");

    double lastTime = glfwGetTime();

    // Boucle de rendu
    while (!glfwWindowShouldClose(gWindow)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        showFPS(gWindow);
        update(deltaTime);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = fpsCamera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(fpsCamera.getFOV()), 
                                                (float)gWindowWidth / gWindowHeight, 0.1f, 200.0f);

        lightingShader.use();
        lightingShader.setUniform("view", view);
        lightingShader.setUniform("projection", projection);

        // Rendu de tous les modèles, y compris les instances de blé
        for (size_t i = 0; i < mesh.size(); i++) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), positions[i]) * glm::scale(glm::mat4(1.0f), echelles[i]);
            lightingShader.setUniform("model", model);

            // Paramètres de matériau
            lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            lightingShader.setUniformSampler("material.diffuseMap", 0);
            lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
            lightingShader.setUniform("material.shininess", 32.0f);

            // Lier la texture et dessiner le modèle
            texture[i].bind(0);
            mesh[i].draw();
            texture[i].unbind(0);
        }

        // Swap buffers et gestion des événements
        glfwSwapBuffers(gWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//-----------------------------------------------------------------------------
// Fonction pour ajouter dynamiquement des instances de blé
//-----------------------------------------------------------------------------
void ajouterBle(int nombreBle, float espacement, const std::string& nomModele, const std::string& nomTexture) {
    Mesh nouveauMesh;
    Texture2D nouvelleTexture;

    // Charger le modèle .obj du blé
    std::string cheminModele = "models/" + nomModele + ".obj";
    if (!nouveauMesh.loadOBJ(cheminModele)) {
        std::cerr << "Erreur : Impossible de charger le modèle " << cheminModele << std::endl;
        return;
    }

    // Charger la texture du blé
    std::string cheminTexture = "textures/" + nomTexture;
    if (!nouvelleTexture.loadTexture(cheminTexture, true)) {
        std::cerr << "Erreur : Impossible de charger la texture " << cheminTexture << std::endl;
        return;
    }

    // Créer une grille d'instances de blé en fonction du nombre spécifié
    for (int i = 0; i < sqrt(nombreBle); i++) {
        for (int j = 0; j < sqrt(nombreBle); j++) {
            float x = i * espacement;
            float z = j * espacement;

            mesh.push_back(nouveauMesh);
            texture.push_back(nouvelleTexture);
            positions.push_back(glm::vec3(x, 0.0f, z));
            echelles.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
        }
    }
}

//-----------------------------------------------------------------------------
// Initialize OpenGL
//-----------------------------------------------------------------------------
bool initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Erreur lors de l'initialisation de GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
    if (gWindow == NULL) {
        std::cerr << "Erreur lors de la création de la fenêtre GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(gWindow);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Erreur lors de l'initialisation de GLEW" << std::endl;
        return false;
    }

    glClearColor(gClearColor.r, gClearColor.g, gClearColor.b, gClearColor.a);
    glEnable(GL_DEPTH_TEST);

    return true;
}

//-----------------------------------------------------------------------------
// Show FPS
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow* window) {
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double currentSeconds = glfwGetTime();
    double elapsedSeconds = currentSeconds - previousSeconds;

    if (elapsedSeconds > 0.25) {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed << APP_TITLE << " - FPS: " << fps << " - Frame Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(window, outs.str().c_str());

        frameCount = 0;
    }

    frameCount++;
}

//-----------------------------------------------------------------------------
// Update each frame
//-----------------------------------------------------------------------------
void update(double elapsedTime) {
    // Camera orientation and movement
    double mouseX, mouseY;
    glfwGetCursorPos(gWindow, &mouseX, &mouseY);
    fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * 0.1f, 
                     (float)(gWindowHeight / 2.0 - mouseY) * 0.1f);
    glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
        fpsCamera.move(35.0f * (float)elapsedTime * fpsCamera.getLook());
    if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
        fpsCamera.move(35.0f * (float)elapsedTime * -fpsCamera.getLook());
    if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
        fpsCamera.move(35.0f * (float)elapsedTime * -fpsCamera.getRight());
    if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
        fpsCamera.move(35.0f * (float)elapsedTime * fpsCamera.getRight());
}
