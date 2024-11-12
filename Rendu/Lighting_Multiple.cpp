//-----------------------------------------------------------------------------
// This code demonstrates how to implement the Blinn-Phong lighting model for
// multiple light sources using OpenGL 3.3 or a forward compatible
// OpenGL 3.0 rendering context. 
//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <string>
#define GLEW_STATIC
#include "GL/glew.h"	// Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Mesh.h"


// Global Variables
const char* APP_TITLE = "Introduction to Modern OpenGL - Multiple Lights";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow* gWindow = NULL;
bool gWireframe = false;
bool gFlashlightOn = true;
glm::vec4 gClearColor(0.06f, 0.06f, 0.07f, 1.0f);
void initble(Mesh* mesh, Texture2D* texture, glm::vec3* modelpos, glm::vec3* modelscale, int indice, int nb_itr, float pos_x, float pos_y,float pos_z,float scale_x,float scale_y,float scale_z , const std::string& objName, const std::string& textureName) ;
void updateSatellitePosition(glm::vec3& satellitePos, const glm::vec3& moonPos, float orbitRadius, float orbitSpeed, double time) ;
FPSCamera fpsCamera(glm::vec3(0.0f, 10.5f, 10.0f));
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 35.0; // units per second
const float MOUSE_SENSITIVITY = 0.1f;


// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime,glm::vec3&);
void showFPS(GLFWwindow* window);
bool initOpenGL();

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

	ShaderProgram lightingShader;
	lightingShader.loadShaders("shaders/lighting_dir_point_spot.vert", "shaders/lighting_dir_point_spot.frag");

	// Load meshes and textures
	const int numModels = 99;
	const int nbobj = 13;

	
	Mesh mesh[numModels];
	Texture2D texture[numModels];
	std::string files_obj[nbobj] = {
				"Road2",
				"Road2",
				"Saloon2",

				"paille",
				"wagon2",
				"sahara2",

				"taxi2",	
				"poto2",
				"Moon2",

				"Train2",
				"Tractor2",
				"Farm",

				"Barn2",





};

// Boucle pour charger chaque modèle avec le chemin complet
for(int i=0 ; i < nbobj; i++) {
    std::string fullPath = "models/" + files_obj[i] + ".obj";  // Construction du chemin complet
    mesh[i].loadOBJ(fullPath.c_str());  // Conversion en C-string pour loadOBJ
}



	

	
// Tableau contenant le nom complet des fichiers de texture avec extension
std::string files_txt[nbobj] = {
    "Road.png",
    "Road.png",
    "Saloon.png",

    "paille.png",
    "wagon.jpg",
    "sahara.png",

    "Taxi.png",
    "poto.png",
    "Moon.png",

	"Train.png",
	"Tractor.png",
	"Farm.png",

	"Barn.png",







};
int indice = 0;
// Boucle pour charger chaque texture en ajoutant le chemin de base
for(int i = 0; i < nbobj; i++) {
    std::string fullPath = "textures/" + files_txt[i];  // Construction du chemin complet
    texture[i].loadTexture(fullPath.c_str(), true);  // Conversion en C-string pour loadTexture
	indice=indice+1;
}





	



	// Model positions	
glm::vec3 modelPos[numModels]; // Assuming there are 34 elements as given in the initial array.

// Assign values to each element
modelPos[0] = glm::vec3(0.0f, 0.0f, 0.0f); // Road
modelPos[1] = glm::vec3(-10.0f, 0.0f, -15.0f); // Road
modelPos[2] = glm::vec3(-10.0f, 0.0f, -5.5f); // Saloon

modelPos[3] = glm::vec3(-10.0f, 15.0f, 133.0f); // Paille
modelPos[4] = glm::vec3(-10.0f, 13.0f, 90.0f); // Wagon
modelPos[5] = glm::vec3(0.0f, 0.0f, 0.0f); // Sahara

modelPos[6] = glm::vec3(0.0f, 1.0f, 0.0f); // Taxi
modelPos[7] = glm::vec3(-30.0f, 0.0f, -8.0f); // Poto
modelPos[8] = glm::vec3(100.0f, 30.0f, -100.0f); // Moon

modelPos[9] = glm::vec3(-105.0f, 1.0f, -25.0f); // Train
modelPos[10] = glm::vec3(-10.0f, 1.0f, -16.0f); // Tractor
modelPos[11] = glm::vec3(-10.0f, 1.0f, -65.5f); // Ferme

modelPos[12] = glm::vec3(-50.0f, 10.0f, -65.5f); // Barn




	// Model scale
glm::vec3 modelScale[numModels]; // Assuming there are 34 elements as in the initial array.

// Assign values to each element
modelScale[0] = glm::vec3(150.1f, 10.1f, 15.1f); // Road
modelScale[1] = glm::vec3(150.1f, 10.1f, 15.1f); // Road
modelScale[2] = glm::vec3(5.0f, 7.0f, 4.0f); // Saloon

modelScale[3] = glm::vec3(1.0f, 1.0f, 1.0f); // Paille
modelScale[4] = glm::vec3(1.0f, 1.0f, 1.0f); // Wagon
modelScale[5] = glm::vec3(150.0f, 10.0f, 150.0f); // Sahara

modelScale[6] = glm::vec3(5.0f, 5.0f, 5.0f); // Taxi
modelScale[7] = glm::vec3(75.0f, 75.0f, 75.0f); // Poto
modelScale[8] = glm::vec3(1.0f, 1.0f, 1.0f); // Moon


modelScale[9] = glm::vec3(10.0f, 10.0f, 10.0f); // Train
modelScale[10] = glm::vec3(0.3f, 0.3f, 0.3f); // Tractor
modelScale[11] = glm::vec3(1.0f, 1.0f, 1.0f); // Ferme

modelScale[12] = glm::vec3(0.3f, 0.3f, 0.3f); // Barn

initble(mesh,texture,modelPos,modelScale,13,17,		-100.0f,-85.5f,1.0f,	1.0f,1.0f,1.0f		,"ble","ble.png");
initble(mesh,texture,modelPos,modelScale,30,17,		-100.0f,-105.0f,1.0f,	1.0f,1.0f,1.0f		,"ble","ble.png");
initble(mesh,texture,modelPos,modelScale,47,17,		-120.0f,40.0f,1.0f,		1.0f,1.0f,1.0f		,"ble","ble.png");
initble(mesh,texture,modelPos,modelScale,64,17,		-120.0f,60.0f,1.0f,		1.0f,1.0f,1.0f		,"ble","ble.png");
initble(mesh,texture,modelPos,modelScale,81,17,		-105.0f, -25.0f, 1.0f,	5.0f, 5.0f, 1.0f	,"raille7","raille.png");
initble(mesh,texture,modelPos,modelScale,98,1,		-105.0f,-25.0f, 35.0f,	5.0f, 5.0f, 5.0f	,"Satellite","Satellite.png");






	// Point Light positions
	glm::vec3 pointLightPos[3] = {
		glm::vec3(-5.0f, 3.8f, 0.0f),
		glm::vec3(0.5f,  3.8f, 0.0f),
		glm::vec3(5.0f,  3.8,  0.0f),

	};

	// Initialiser le circuit de rails carrés
// Initialiser le circuit de rails carrés
//initroad(mesh, texture, modelPos, modelScale);
	double lastTime = glfwGetTime();

	// Rendering loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		// Poll for and process events
		glfwPollEvents();
		update(deltaTime,modelPos[6]);
		update(deltaTime,modelPos[9]);
		updateSatellitePosition(modelPos[98], modelPos[8], 20.0f, 1.5f, currentTime); // Exemple : 20.0f pour le rayon, 0.5f pour la vitesse

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0), view(1.0), projection(1.0);

		// Create the View matrix
		view = fpsCamera.getViewMatrix();

		// Create the projection matrix
		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

		// update the view (camera) position
		glm::vec3 viewPos;
		viewPos.x = fpsCamera.getPosition().x;
		viewPos.y = fpsCamera.getPosition().y;
		viewPos.z = fpsCamera.getPosition().z;


		// Must be called BEFORE setting uniforms because setting uniforms is done
		// on the currently active shader program.
		lightingShader.use(); 
		lightingShader.setUniform("model", glm::mat4(1.0));  // do not need to translate the models so just send the identity matrix
		lightingShader.setUniform("view", view);
		lightingShader.setUniform("projection", projection);
		lightingShader.setUniform("viewPos", viewPos);

		// Directional light
		lightingShader.setUniform("sunLight.direction", glm::vec3(0.5f, -0.5f, -0.2f)); 
		lightingShader.setUniform("sunLight.ambient", glm::vec3(0.3f, 0.2f, 0.2f));
		lightingShader.setUniform("sunLight.diffuse", glm::vec3(1.0f, 0.6f, 0.3f)); 	// dark
		lightingShader.setUniform("sunLight.specular", glm::vec3(0.5f, 0.4f, 0.3f));

		// Point Light 1
		lightingShader.setUniform("pointLights[0].ambient", glm::vec3(0.3f, 0.2f, 0.2f));
		lightingShader.setUniform("pointLights[0].diffuse", glm::vec3(0.0f, 1.0f, 0.1f));	// green-ish light
		lightingShader.setUniform("pointLights[0].specular", glm::vec3(0.5f, 0.4f, 0.3f));
		lightingShader.setUniform("pointLights[0].position", pointLightPos[0]);
		lightingShader.setUniform("pointLights[0].constant", 1.0f);
		lightingShader.setUniform("pointLights[0].linear", 0.22f);
		lightingShader.setUniform("pointLights[0].exponent", 0.20f);

		// Point Light 2
		lightingShader.setUniform("pointLights[1].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniform("pointLights[1].diffuse", glm::vec3(1.0f, 0.1f, 0.0f));	// red-ish light
		lightingShader.setUniform("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("pointLights[1].position", pointLightPos[1]);
		lightingShader.setUniform("pointLights[1].constant", 1.0f);
		lightingShader.setUniform("pointLights[1].linear", 0.22f);
		lightingShader.setUniform("pointLights[1].exponent", 0.20f);

		// Point Light 3
		lightingShader.setUniform("pointLights[2].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniform("pointLights[2].diffuse", glm::vec3(0.0f, 0.1f, 1.0f));	// blue-ish light
		lightingShader.setUniform("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("pointLights[2].position", pointLightPos[2]);
		lightingShader.setUniform("pointLights[2].constant", 1.0f);
		lightingShader.setUniform("pointLights[2].linear", 0.22f);
		lightingShader.setUniform("pointLights[2].exponent", 0.20f);

		// Spot light
		glm::vec3 spotlightPos = fpsCamera.getPosition();

		// offset the flash light down a little
		spotlightPos.y -= 0.5f;

		lightingShader.setUniform("spotLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		lightingShader.setUniform("spotLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("spotLight.position", spotlightPos);
		lightingShader.setUniform("spotLight.direction", fpsCamera.getLook());
		lightingShader.setUniform("spotLight.cosInnerCone", glm::cos(glm::radians(15.0f)));
		lightingShader.setUniform("spotLight.cosOuterCone", glm::cos(glm::radians(20.0f)));
		lightingShader.setUniform("spotLight.constant", 1.0f);
		lightingShader.setUniform("spotLight.linear", 0.07f);
		lightingShader.setUniform("spotLight.exponent", 0.017f);
		lightingShader.setUniform("spotLight.on", gFlashlightOn);

		// Render the scene
		for (int i = 0; i < numModels; i++)
		{
			model = glm::translate(glm::mat4(1.0), modelPos[i]) * glm::scale(glm::mat4(1.0), modelScale[i]);
			lightingShader.setUniform("model", model);

			// Set material properties
			lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
			lightingShader.setUniform("material.shininess", 32.0f);

			texture[i].bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
			mesh[i].draw();			// Render the OBJ mesh
			texture[i].unbind(0);
		}

				// Swap front and back buffers
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3


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
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
	glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

	// Hides and grabs cursor, unlimited movement
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	glClearColor(gClearColor.r, gClearColor.g, gClearColor.b, gClearColor.a);

    // Define the viewport dimensions
    int w, h;
    glfwGetFramebufferSize( gWindow, &w, &h); // For retina display
    glViewport(0, 0, w, h);
    
    //    glViewport(0, 0, gWindowWidth, gWindowHeight);

    glEnable(GL_DEPTH_TEST);

	return true;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		// toggle the flashlight
		gFlashlightOn = !gFlashlightOn;
	}
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	gWindowWidth = width;
	gWindowHeight = height;

    // Define the viewport dimensions
    int w, h;
    glfwGetFramebufferSize( gWindow, &w, &h); // For retina display
    glViewport(0, 0, w, h);
    
    //    glViewport(0, 0, gWindowWidth, gWindowHeight);

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
//-----------------------------------------------------------------------------
// Update stuff every frame
//-----------------------------------------------------------------------------
	float movementRange = 90.0f; // Amplitude de déplacement en fonction de la taille du sol
	float movementFrequency = 0.2f; // Diminuez cette valeur pour ralentir la vitesse


	void update(double elapsedTime, glm::vec3& pos)
	{
		// Calcule la nouvelle position sur l'axe X pour la paille avec une plus grande amplitude
		float posX = movementRange * sin(movementFrequency * (float)glfwGetTime());
		pos.x = posX;

		// Camera orientation
		double mouseX, mouseY;
		glfwGetCursorPos(gWindow, &mouseX, &mouseY);

		fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, 
						(float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

		// Clamp mouse cursor to center of screen
		glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

		// Camera FPS movement
		if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
			fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
		else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
			fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());

		if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
			fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
		else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
			fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());

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
		outs.precision(3);	// decimal places
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
void initble(Mesh* mesh, Texture2D* texture, glm::vec3* modelpos, glm::vec3* modelscale, int indice, int nb_itr, float pos_x, float pos_y,float pos_z,float scale_x,float scale_y,float scale_z , const std::string& objName, const std::string& textureName) 
{    int nb_models = nb_itr + indice;
    for (int j = 0; j < nb_itr; j++) {
        // Construct file paths for the object and texture
        std::string objPath = "models/" + objName + ".obj";
        std::string texturePath = "textures/" + textureName;

        // Load the object and texture
        mesh[indice + j].loadOBJ(objPath.c_str());
        texture[indice + j].loadTexture(texturePath.c_str());

        // Set the scale and position for each model
        modelscale[indice + j] = glm::vec3(scale_x, scale_z, scale_y); // Ble scale
        modelpos[indice + j] = glm::vec3(pos_x + j * 10, pos_z, pos_y); // Ble position
    }
}
void updateSatellitePosition(glm::vec3& satellitePos, const glm::vec3& moonPos, float orbitRadius, float orbitSpeed, double time) {
    // Calcul de la nouvelle position du satellite autour de la lune
    satellitePos.x = moonPos.x + orbitRadius * cos(orbitSpeed * time);
    satellitePos.z = moonPos.z + orbitRadius * sin(orbitSpeed * time);
    satellitePos.y = moonPos.y; // Garder la même hauteur que la lune
}