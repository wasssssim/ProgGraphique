
#include <iostream>
#include <sstream>
#include <string>
#define GLEW_STATIC
#include "GL/glew.h"    // Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Skybox.hpp"
#include "ShaderProgram.hpp"
#include "Texture2d.hpp"
#include "Mesh.hpp"
#include "Roquette.hpp"
#include "Camera.hpp"
#include "ObjectLoader.cpp"





// Global Variables
const char* APP_TITLE = "Introduction to Modern OpenGL - Blinn-Phong Lighting Model and Materials";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow* gWindow = NULL;
bool gWireframe = false;
glm::vec4 gClearColor(0.23f, 0.38f, 0.47f, 1.0f);

FPSCamera fpsCamera(glm::vec3(0.0f, 2.0f, 10.0f));
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 5.0; // units per second
const float MOUSE_SENSITIVITY = 0.1f;

// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();
std::vector<Roquette> roq;
float deltaTime = 0.0f;  // Variable globale


//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main()
{
   if (!initOpenGL())
   {
       // An error occured
       std::cerr << "GLFW initialization failed" << std::endl;
       return -1;
   }
   Skybox skybox; 

   GLuint skyboxTexture = skybox.loadCubemap();

   ShaderProgram shaderProgram;
   shaderProgram.loadShaders("src/shaders/lighting_phong_materials.vert", "src/shaders/lighting_phong_materials.frag");

   ShaderProgram shaderSkybox;
   shaderSkybox.loadShaders("src/shaders/Skybox.vert", "src/shaders/Skybox.frag");




   // Load meshes and textures
   ObjectLoader ObjectLoader ; 
   std::vector<Mesh> mesh = ObjectLoader.loadAllModel();
   std::vector<Texture2d> texture = ObjectLoader.loadAllTexture(); 
   Roquette roquette(fpsCamera.getPosition(),fpsCamera.getFOV(),100,1); 


   Mesh lightMesh;
   lightMesh.loadOBJ("src/ressources/IOGL-datas/lights/light.obj");

       // Model positions
   std::vector<glm::vec3>  modelPos = ObjectLoader.getAutomaticPositions(mesh); 
 
   std::vector<glm::vec3>  modelScale = ObjectLoader.getAutomaticScales(mesh); 

   double lastTime = glfwGetTime();
   float angle = 0.0f;

   // Rendering loop
   while (!glfwWindowShouldClose(gWindow))
   {
       showFPS(gWindow);

       double currentTime = glfwGetTime();
       deltaTime = currentTime - lastTime;

       // Poll for and process events
       glfwPollEvents();
       update(deltaTime);

       // Clear the screen
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       glm::mat4 model(1.0), view(1.0), projection(1.0);

       // Create the View matrix
       view = fpsCamera.getViewMatrix();

       // Create the projection matrix
       projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

           // Rendu de la skybox
       glDepthFunc(GL_LEQUAL);  // Change la fonction de profondeur pour que la skybox soit toujours derrière


       // update the view (camera) position
       glm::vec3 viewPos;
       viewPos.x = fpsCamera.getPosition().x;
       viewPos.y = fpsCamera.getPosition().y;
       viewPos.z = fpsCamera.getPosition().z;

       
       glm::vec3 pointLightPos[3] = {
           glm::vec3(-5.0f, 3.8f, 0.0f),
           glm::vec3(0.5f,  3.8f, 0.0f),
           glm::vec3(5.0f,  3.8,  0.0f)
       };

       glm::vec3 lightPos(0.0f, 1.0f, 10.0f);
       glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

       // Move the light
       angle += (float)deltaTime * 50.0f;
       lightPos.x = 8.0f * sinf(glm::radians(angle));  // slide back and forth

       // Must be called BEFORE setting uniforms because setting uniforms is done on the currently active shader program.
       shaderProgram.use();

       // Simple light
       shaderProgram.setUniform("view", view);
       shaderProgram.setUniform("projection", projection);
       shaderProgram.setUniform("viewPos", viewPos);
       shaderProgram.setUniform("light.position", lightPos);
       shaderProgram.setUniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
       shaderProgram.setUniform("light.diffuse", lightColor);
       shaderProgram.setUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
               
       // Render the scene
       for (int i = 0; i < texture.size(); i++)
       {
           model = glm::translate(glm::mat4(1.0), modelPos[i]) * glm::scale(glm::mat4(1.0), modelScale[i]);
           shaderProgram.setUniform("model", model);

           // Set material properties
           shaderProgram.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
           shaderProgram.setUniformSampler("material.diffuseMap", 0);
           shaderProgram.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
           shaderProgram.setUniform("material.shininess", 32.0f);

           texture[i].bind(0);     // set the texture before drawing.
           mesh[i].draw();         // Render the OBJ mesh
           texture[i].unbind(0);
           
       }               

       for (const auto& roquette : roq) {
       glm::mat4 model = glm::translate(glm::mat4(1.0f), roquette.getPosition());
       shaderProgram.setUniform("model", model);
       roquetteTexture.bind(0);
       roquette.draw(); // Assure-toi que la méthode `draw` est implémentée
       roquetteTexture.unbind(0);
}

       
       lightMesh.draw();
       // Rendu de la skybox
       glDepthFunc(GL_LEQUAL);  // Change la fonction de profondeur pour que la skybox soit toujours derrière

       shaderSkybox.use();
       glm::mat4 skyboxModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 20.0f, 0.0f));
       view = glm::mat4(glm::mat3(fpsCamera.getViewMatrix()));
       shaderSkybox.setUniform("model", skyboxModel);
       shaderSkybox.setUniform("view", view);
       shaderSkybox.setUniform("projection", projection);

       glBindVertexArray(skybox.setupSkyboxVAO()); // Active le VAO de la skybox
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
       glDrawArrays(GL_TRIANGLES, 0, 36); // Dessine le cube de la skybox
       glBindVertexArray(0);

       glDepthFunc(GL_LESS);

       

       glfwSwapBuffers(gWindow);

       lastTime = currentTime;
   }

   glfwTerminate();

   return 0;
}

//-----------------------------------------------------------------------------
// Initialize GLFW and OpenGL
//-----------------------------------------------------------------------------
bool initOpenGL()
{
   // Intialize GLFW 
   // GLFW is configured.  Must be called before calling any GLFW functions
   if (!glfwInit())
   {
       // An error occured
       std::cerr << "GLFW initialization failed" << std::endl;
       return false;
   }

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    // forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3


   // Create an OpenGL 3.3 core, forward compatible context window
   gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
   if (gWindow == NULL)
   {
       std::cerr << "Failed to create GLFW window" << std::endl;
       glfwTerminate();
       return false;
   }

   // Make the window's context the current one
   glfwMakeContextCurrent(gWindow);

   // Initialize GLEW
   glewExperimental = GL_TRUE;
   if (glewInit() != GLEW_OK)
   {
       std::cerr << "Failed to initialize GLEW" << std::endl;
       return false;
   }

   // Set the required callback functions
   glfwSetKeyCallback(gWindow, glfw_onKey);
   glfwSetMouseButtonCallback(gWindow, glfw_onMouseButton);
   glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
   glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

   // Hides and grabs cursor, unlimited movement
   glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

   glClearColor(gClearColor.r, gClearColor.g, gClearColor.b, gClearColor.a);

   // Define the viewport dimensions
   glViewport(0, 0, gWindowWidth, gWindowHeight);
   glEnable(GL_DEPTH_TEST);

   return true;
}


void glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
   if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
   {
       // Appeler la fonction tirer pour ajouter une nouvelle roquette
       fpsCamera.tirer(roq);  // 'roq' est le vecteur de roquettes
       std::cout << "Tir !!" << std::endl;

       // Déplacer toutes les roquettes présentes dans la scène
       for (auto& roquette : roq) {
           roquette.move(deltaTime);  // Déplace la roquette en fonction du temps écoulé
       }
   }
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   // Quitter lorsque la touche Escape est pressée
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
       glfwSetWindowShouldClose(window, GL_TRUE);

   // Activer/désactiver le mode wireframe avec F1
   if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
   {
       gWireframe = !gWireframe;
       if (gWireframe)
           glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
       else
           glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   }
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
   gWindowWidth = width;
   gWindowHeight = height;
   glViewport(0, 0, gWindowWidth, gWindowHeight);
}

//-----------------------------------------------------------------------------
// Called by GLFW when the mouse wheel is rotated
//-----------------------------------------------------------------------------
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
   double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

   fov = glm::clamp(fov, 1.0, 120.0);

   fpsCamera.setFOV((float)fov);
}

//-----------------------------------------------------------------------------
// Update stuff every frame
//-----------------------------------------------------------------------------
void update(double elapsedTime)
{
   // Camera orientation
   double mouseX, mouseY;

   // Get the current mouse cursor position delta
   glfwGetCursorPos(gWindow, &mouseX, &mouseY);

   // Rotate the camera the difference in mouse distance from the center screen.  Multiply this delta by a speed scaler
   fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

   // Clamp mouse cursor to center of screen
   glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

   // Camera FPS movement

   // Forward/backward
   if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
       fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
   else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
       fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());

   // Strafe left/right
   if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
       fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
   else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
       fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());

   // Up/down
   if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
       fpsCamera.move(MOVE_SPEED * (float)elapsedTime * glm::vec3(0.0f, 1.0f, 0.0f));
   else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
       fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -glm::vec3(0.0f, 1.0f, 0.0f));
}

//-----------------------------------------------------------------------------
// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the window caption bar.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow* window)
{
   static double previousSeconds = 0.0;
   static int frameCount = 0;
   double elapsedSeconds;
   double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

   elapsedSeconds = currentSeconds - previousSeconds;

   // Limit text updates to 4 times per second
   if (elapsedSeconds > 0.25)
   {
       previousSeconds = currentSeconds;
       double fps = (double)frameCount / elapsedSeconds;
       double msPerFrame = 1000.0 / fps;

       // The C++ way of setting the window title
       std::ostringstream outs;
       outs.precision(3);  // decimal places
       outs << std::fixed
           << APP_TITLE << "    "
           << "FPS: " << fps << "    "
           << "Frame Time: " << msPerFrame << " (ms)";
       glfwSetWindowTitle(window, outs.str().c_str());

       // Reset for next average.
       frameCount = 0;
   }

   frameCount++;
}
