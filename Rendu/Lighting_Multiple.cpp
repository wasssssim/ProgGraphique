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

#include"cieltexture.hpp"
	#include "ShaderProgram.h"
	#include "Texture2D.h"
	#include "Camera.h"
	#include "Mesh.h"


	// Global Variables
	bool issun = true;
	const char* APP_TITLE = "Introduction to Modern OpenGL - Multiple Lights";
	int gWindowWidth = 1024;
	int gWindowHeight = 768;
	GLFWwindow* gWindow = NULL;
	bool gWireframe = false;
	bool gFlashlightOn = true;
	glm::vec4 gClearColor(0.06f, 0.06f, 0.07f, 1.0f);
	void initble(Mesh* mesh, Texture2D* texture, glm::vec3* modelpos, glm::vec3* modelscale, int indice, int nb_itr, float pos_x, float pos_y,float pos_z,float scale_x,float scale_y,float scale_z , const std::string& objName, const std::string& textureName,int pas) ;
	void updateSatellitePosition(glm::vec3& satellitePos, const glm::vec3& moonPos, float orbitRadius, float orbitSpeed, double time) ;
	void initlight(glm::vec3* lightpos,int indicelight,int indiceobj,glm::vec3* modelpos,float pas_z);
	void setPointLight(ShaderProgram& shader, int index, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, 
					const glm::vec3& position, float constant, float linear, float exponent) ;
void updateLocoPosition(glm::vec3& pos, int x, int index, float speed, float frequency, float spacing) ;
	FPSCamera fpsCamera(glm::vec3(0.0f, 10.5f, 10.0f));

void moveObject(glm::vec3& currentPos, const glm::vec3& startPos, const glm::vec3& targetPos, float speed);

	const double ZOOM_SENSITIVITY = -3.0;
	const float MOVE_SPEED = 35.0; // units per second
	const float MOUSE_SENSITIVITY = 0.1f;


	// Function prototypes
	void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
	void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
	void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
	void update(double elapsedTime);
	void showFPS(GLFWwindow* window);
	bool initOpenGL();
	void generateRandomBisons(
    Mesh* mesh, 
    Texture2D* texture, 
    glm::vec3* modelPos, 
    glm::vec3* modelScale, 
    int startIndex, 
    int numBisons,
	float yFixed, 
    float zFixed, 
    float scaleX, 
    float scaleY, 
    float scaleZ, 
    const std::string& objName, 
    const std::string& textureName, 
    float mapXMin, 
    float mapXMax, 
    float mapYMin, 
    float mapYMax) ;

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
		cieltexture cieltexture;
		GLuint skyboxTexture = cieltexture.loadCubemap();
		ShaderProgram shadercieltexture;
		shadercieltexture.loadShaders("shaders/cieltexture.vert", "shaders/cieltexture.frag");




		ShaderProgram lightingShader;
		lightingShader.loadShaders("shaders/lighting_dir_point_spot.vert", "shaders/lighting_dir_point_spot.frag");

		// Load meshes and texturestransparent
		const int numModels = 198;
		const int nbobj = 11;

		
		Mesh mesh[numModels];
		Texture2D texture[numModels];
		std::string files_obj[nbobj] = {
					"Road2",
					"Saloon2",

					"paille",
					"wagon2",
					"sahara2",

					"taxi2",	
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
		"Saloon.png",

		"paille.png",
		"wagon.jpg",
		"sahara.png",

		"Taxi.png",
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
	modelPos[0] = glm::vec3(-10.0f, 0.0f, 0.0f); // Road
	modelPos[1] = glm::vec3(60.0f, 0.0f, -65.5f); // Saloon

	modelPos[2] = glm::vec3(-10.0f, 15.0f, 133.0f); // Paille
	modelPos[3] = glm::vec3(-10.0f, 13.0f, 90.0f); // Wagon
	modelPos[4] = glm::vec3(0.0f, 0.0f, 0.0f); // Sahara

	modelPos[5] = glm::vec3(0.0f, 1.0f, 0.0f); // Taxi
	modelPos[6] = glm::vec3(100.0f, 20.0f, -80.0f); // Moon

	modelPos[7] = glm::vec3(-105.0f, 1.0f, -25.0f); // Train
	modelPos[8] = glm::vec3(-10.0f, 1.0f, -16.0f); // Tractor
	modelPos[9] = glm::vec3(-10.0f, 1.0f, -65.5f); // Ferme

	modelPos[10] = glm::vec3(-50.0f, 10.0f, -65.5f); // Barn




		// Model scale
	glm::vec3 modelScale[numModels]; // Assuming there are 34 elements as in the initial array.

	// Assign values to each element
	modelScale[0] = glm::vec3(230.1f, 10.1f, 15.1f); // Road
	modelScale[1] = glm::vec3(15.0f, 17.0f, 14.0f); // Saloon

	modelScale[2] = glm::vec3(1.0f, 1.0f, 1.0f); // Paille
	modelScale[3] = glm::vec3(1.0f, 1.0f, 1.0f); // Wagon
	modelScale[4] = glm::vec3(180.0f, 10.0f, 150.0f); // Sahara

	modelScale[5] = glm::vec3(5.0f, 5.0f, 5.0f); // Taxi
	modelScale[6] = glm::vec3(1.0f, 1.0f, 1.0f); // Moon


	modelScale[7] = glm::vec3(10.0f, 10.0f, 10.0f); // Train
	modelScale[8] = glm::vec3(0.3f, 0.3f, 0.3f); // Tractor
	modelScale[9] = glm::vec3(2.0f, 2.0f, 2.0f); // Ferme

	modelScale[10] = glm::vec3(0.3f, 0.3f, 0.3f); // Barn


	initble(mesh,texture,modelPos,modelScale,11,17,		-100.0f,-85.5f,1.0f,	1.0f,1.0f,1.0f		,"ble","ble.png",10);
	initble(mesh,texture,modelPos,modelScale,30,17,		-100.0f,-105.0f,1.0f,	1.0f,1.0f,1.0f		,"ble","ble.png",10);
	initble(mesh,texture,modelPos,modelScale,47,17,		-120.0f,40.0f,1.0f,		1.0f,1.0f,1.0f		,"ble","ble.png",10);
	initble(mesh,texture,modelPos,modelScale,64,17,		-120.0f,60.0f,1.0f,		1.0f,1.0f,1.0f		,"ble","ble.png",10);
	initble(mesh,texture,modelPos,modelScale,81,17,		-105.0f, -25.0f, 1.0f,	5.0f, 5.0f, 1.0f	,"raille7","raille.png",10);
	initble(mesh,texture,modelPos,modelScale,98,1,		-105.0f,-25.0f, 35.0f,	5.0f, 5.0f, 5.0f	,"Satellite","Satellite.png",10);
	initble(mesh,texture,modelPos,modelScale,116,2,		-100.0f, -27.0f, 5.0f,	3.0f, 4.0f, 4.0f	,"gare","transparent.png",170);
	initble(mesh,texture,modelPos,modelScale,118,1,	 	 75.0f, -27.0f, 10.0f,	5.0f, 5.0f, 5.0f	,"man","transparent.png",10);
	initble(mesh,texture,modelPos,modelScale,119,7	,	-100.0f, 27.0f, 1.0f,	5.0f, 5.0f, 5.0f	,"home","House.png",30);
	initble(mesh,texture,modelPos,modelScale,126,8	,	-100.0f, 5.0f, 1.0f,	3.0f, 3.0f, 3.0f	,"Cactus","Cactus.png",30);
	initble(mesh,texture,modelPos,modelScale,143,7,		-100.0f, -7.5f, 1.0f,	3.0f, 3.0f, 3.0f	,"lamp","lamp.png",30);
	initble(mesh,texture,modelPos,modelScale,150,1,		 30.0f, -65.0f, 2.0f,	3.0f, 3.0f, 3.0f	,"fire","fire.png",30);
	initble(mesh,texture,modelPos,modelScale,151,10,	-130.0f,60.0f,30.0f,	5.0f, 5.0f, 5.0f	,"ovni","ovni.png",30);
	initble(mesh,texture,modelPos,modelScale,161,10,	-130.0f,70.0f,1.0f,		5.0f, 5.0f, 5.0f	,"alien","Alien.png",30);
float mapXMin = -100.0f;
float mapXMax = 100.0f;
float mapYMin = -100.0f;
float mapYMax = 100.0f;

// Ajouter 20 bisons aléatoires avec un Z constant et une taille fixe
generateRandomBisons(
    mesh, 
    texture, 
    modelPos, 
    modelScale, 	
    171,             // Indice de départ dans modelPos et modelScale
    20,              // Nombre de bisons à générer
    1.0f,  
	1.0f,        // Position Z fixe
    5.0f, 5.0f, 5.0f,// Taille fixe des bisons
    "taxi2",         // Nom du fichier OBJ
    "Taxi.png",     // Nom du fichier de texture
    mapXMin, mapXMax,// Limites de la carte sur l'axe X
    mapYMin, mapYMax // Limites de la carte sur l'axe Y
);











		int nb_light= 19;



		// Point Light positions
		glm::vec3 pointLightPos[nb_light];
		for (int y=0;y<7;y++){
		initlight(pointLightPos,y,143+y,modelPos,18.0f);
		}
		initlight(pointLightPos,7,7,modelPos,15.0f);
		initlight(pointLightPos,8,150,modelPos,15.0f);
		for(int y=0;y<10;y++){
			initlight(pointLightPos,9+y,161+y,modelPos,15.0f);

		}

		


		// Définir la position de la lumière de projecteur
		glm::vec3 moonSpotlightPos = glm::vec3(50.0f, 40.0f, -50.0f); // Position du projecteur pour la lune
		glm::vec3 moonSpotlightDir = glm::normalize(modelPos[6] - moonSpotlightPos);

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

			update(deltaTime);
			const int locoStartIndex = 171; // Indice de départ des locomotives dans modelPos
			const int locoCount = 5;        // Nombre total de locomotives
			const float locoSpeed = 10.0f;  // Vitesse des locomotives
			const float locoFrequency = 1.0f; // Fréquence du mouvement
			const float locoSpacing = 20.0f;  // Distance minimale entre deux locomotives



			updateSatellitePosition(modelPos[98], modelPos[6], 20.0f, 1.5f, currentTime); // Exemple : 20.0f pour le rayon, 0.5f pour la vitesse
			


			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 model(1.0), view(1.0), projection(1.0);

			// Create the View matrix
			view = fpsCamera.getViewMatrix();

			// Create the projection matrix
			projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

// Render the skybox



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
			if(issun){
			lightingShader.setUniform("sunLight.direction", glm::vec3(0.5f, -0.5f, -0.2f)); 
			lightingShader.setUniform("sunLight.ambient", glm::vec3(0.3f, 0.2f, 0.2f));
			lightingShader.setUniform("sunLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); 	// dark
			lightingShader.setUniform("sunLight.specular", glm::vec3(0.5f, 0.4f, 0.3f));}
			else{

    // Lumière sombre
    lightingShader.setUniform("sunLight.ambient", glm::vec3(0.3f, 0.2f, 0.2f));
    lightingShader.setUniform("sunLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f)); 
    lightingShader.setUniform("sunLight.specular", glm::vec3(0.5f, 0.4f, 0.3f));
			}
			

			// Réduction 
			// Réduction de l'exponentiel pour une portée encore plus large

			// Spot light
			glm::vec3 spotlightPos[2] = {
    glm::vec3(100.0f, 20.0f, -80.0f), // Spotlight 0
    glm::vec3(-120.0f, -105.0f, 1.0f) // Spotlight 1
};

// Configure Spotlight 0
lightingShader.setUniform("spotLights[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f)); // Rouge vif pour l'ambient
lightingShader.setUniform("spotLights[0].diffuse", glm::vec3(2.0f, 1.0f,  1.0f)); // Rouge vif surpuissant pour le diffuse
lightingShader.setUniform("spotLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f)); // Rouge vif pour le specular

// Position et direction
lightingShader.setUniform("spotLights[0].position", spotlightPos[0]); // Position définie par spotlightPos[0]
lightingShader.setUniform("spotLights[0].direction", glm::vec3(0.0f, -1.0f, 0.0f)); // Pointe vers le bas

// Angles élargis pour un cône 10 fois plus grand
lightingShader.setUniform("spotLights[0].cosInnerCone", glm::cos(glm::radians(160.0f))); // Cône interne presque omnidirectionnel
lightingShader.setUniform("spotLights[0].cosOuterCone", glm::cos(glm::radians(170.0f))); // Cône externe immense

// Paramètres d'atténuation adaptés
lightingShader.setUniform("spotLights[0].constant", 1.0f);  // Pas d'atténuation constante
lightingShader.setUniform("spotLights[0].linear", 0.001f);  // Atténuation très faible
lightingShader.setUniform("spotLights[0].exponent", 0.0001f); // Presque pas d'atténuation quadratique

// Activation du spotlight
lightingShader.setUniform("spotLights[0].on", 1);	


// Configure Spotlight 1
lightingShader.setUniform("spotLights[1].ambient", glm::vec3(0.3f, 0.0f, 0.3f)); // Violet plus subtil
lightingShader.setUniform("spotLights[1].diffuse", glm::vec3(1.0f, 0.0f, 1.0f)); // Violet vif
lightingShader.setUniform("spotLights[1].specular", glm::vec3(1.0f, 0.0f, 1.0f)); // Violet vif
lightingShader.setUniform("spotLights[1].position", spotlightPos[1]);
lightingShader.setUniform("spotLights[1].direction", glm::vec3(0.0f, -1.0f, 0.0f));
lightingShader.setUniform("spotLights[1].cosInnerCone", glm::cos(glm::radians(15.0f)));
lightingShader.setUniform("spotLights[1].cosOuterCone", glm::cos(glm::radians(20.0f)));
lightingShader.setUniform("spotLights[1].constant", 1.0f);
lightingShader.setUniform("spotLights[1].linear", 0.07f);
lightingShader.setUniform("spotLights[1].exponent", 0.017f);
lightingShader.setUniform("spotLights[1].on", 1); // Activer le spotlight
		for (int z = 0; z < nb_light; z++) {
    if (z >= 9 && z <= 18) { // Indices des lumières OVNI (en fonction des indices des aliens)
        setPointLight(
            lightingShader, 
            z, 
            glm::vec3(0.1f, 0.4f, 0.1f),   // Ambiance verte douce
            glm::vec3(0.2f, 1.0f, 0.2f),   // Diffuse verte lumineuse
            glm::vec3(0.4f, 1.5f, 0.4f),   // Speculaire verte éclatante
            pointLightPos[z],              // Position
            1.0f,                          // Facteur constant
            0.03f,                         // Atténuation linéaire
            0.005f                         // Atténuation exponentielle
        );
    } else if (z == 8) {  // Lumière pour "fire"
        setPointLight(
            lightingShader, 
            z, 
            glm::vec3(1.0f, 0.6f, 0.2f),   // Ambiant (couleur chaude pour le feu)
            glm::vec3(3.0f, 2.4f, 1.2f),   // Diffuse (très lumineux)
            glm::vec3(3.0f, 2.7f, 1.5f),   // Specular (haute intensité pour l'éclat)
            pointLightPos[z],              // Position
            1.0f,                          // Facteur constant (1.0)
            0.022f,                        // Atténuation linéaire réduite pour grande portée
            0.0019f                        // Atténuation exponentielle faible
        );
    } else {
        // Paramètres des autres lumières
        setPointLight(
            lightingShader, 
            z, 
            glm::vec3(1.0f, 0.8f, 0.4f),   // Ambiance
            glm::vec3(1.5f, 1.2f, 0.6f),   // Diffuse
            glm::vec3(1.0f, 0.9f, 0.7f),   // Specular
            pointLightPos[z],              // Position
            1.0f,                          // Constant
            0.045f,                        // Linear
            0.007f                         // Exponentiel
        );
    }
}
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


			glfwSwapBuffers(gWindow);

			lastTime = currentTime;


glDepthFunc(GL_LESS);
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

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			issun = !issun;

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


		void update(double elapsedTime)
		{
			// Calcule la nouvelle position sur l'axe X pour la paille avec une plus grande amplitude

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
	void initble(Mesh* mesh, Texture2D* texture, glm::vec3* modelpos, glm::vec3* modelscale, int indice, int nb_itr, float pos_x, float pos_y,float pos_z,float scale_x,float scale_y,float scale_z , const std::string& objName, const std::string& textureName , int pas) 
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
			modelpos[indice + j] = glm::vec3(pos_x + j * pas, pos_z, pos_y); // Ble position
		}
	}
	void updateSatellitePosition(glm::vec3& satellitePos, const glm::vec3& moonPos, float orbitRadius, float orbitSpeed, double time) {
		// Calcul de la nouvelle position du satellite autour de la lune
		satellitePos.x = moonPos.x + orbitRadius * cos(orbitSpeed * time);
		satellitePos.z = moonPos.z + orbitRadius * sin(orbitSpeed * time);
		satellitePos.y = moonPos.y; // Garder la même hauteur que la lune
	}
	void initlight(glm::vec3* lightpos,int indicelight,int indiceobj,glm::vec3* modelpos,float pas_z)
	{



		lightpos[indicelight]= modelpos[indiceobj]+glm::vec3(0.0f,pas_z,0.0f);

		


	}
	void setPointLight(ShaderProgram& shader, int index, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, 
					const glm::vec3& position, float constant, float linear, float exponent) 
	{
		std::string base = "pointLights[" + std::to_string(index) + "].";
		
		shader.setUniform((base + "ambient").c_str(), ambient),0;       // Couleur ambiante
		shader.setUniform((base + "diffuse").c_str(), diffuse);       // Couleur diffuse
		shader.setUniform((base + "specular").c_str(), specular);     // Couleur spéculaire
		shader.setUniform((base + "position").c_str(), position);     // Position de la lumière
		shader.setUniform((base + "constant").c_str(), constant);     // Facteur constant
		shader.setUniform((base + "linear").c_str(), linear);         // Facteur linéaire
		shader.setUniform((base + "exponent").c_str(), exponent);     // Facteur exponentiel
	}
void updateLocoPosition(glm::vec3& pos, int x, int index, float speed, float frequency, float spacing) {
    // Calcul du temps actuel
    float time = (float)glfwGetTime();

    // Calcul dynamique du gap pour éviter les chevauchements
    float gap = spacing / (speed * frequency * glm::pi<float>());

    // Calcul de la position sinusoïdale en fonction du temps, de l'indice et du gap
    float posX = 45.0f * sin(frequency * time - index * gap);

    // Mettre à jour la position avec le décalage basé sur x
    pos.x = posX + x;
	if(posX>100){
		pos.x = -100.0f;
	}
}
void generateRandomBisons(
Mesh* mesh,  Texture2D* texture,  glm::vec3* modelPos,  glm::vec3* modelScale,  int startIndex,  int numBisons,float yFixed , float zFixed,  float scaleX,  float scaleY,  float scaleZ,  const std::string& objName,  const std::string& textureName,  float mapXMin,  float mapXMax,  float mapYMin,  float mapYMax)
{
    // Initialiser le générateur de nombres aléatoires
    srand((unsigned int)time(0)); 

    for (int i = 0; i < numBisons; i++) {
        // Générer des coordonnées aléatoires pour X et Y
        float randomX = mapXMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mapXMax - mapXMin)));
        float randomY = mapYMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mapYMax - mapYMin)));

        // Charger le modèle et la texture pour chaque bison
        std::string objPath = "models/" + objName + ".obj";
        std::string texturePath = "textures/" + textureName;
        mesh[startIndex + i].loadOBJ(objPath.c_str());
        texture[startIndex + i].loadTexture(texturePath.c_str());

        // Définir la position et l'échelle du bison
        modelPos[startIndex + i] = glm::vec3(randomX, zFixed,  yFixed);
        modelScale[startIndex + i] = glm::vec3(scaleX, scaleZ, scaleY);
    }
}

