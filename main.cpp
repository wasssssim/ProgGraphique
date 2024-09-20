#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>

const char* app_title = "Test App";
const int app_width = 840;
const int app_height = 680;
bool full_screen=true;

// Déclaration de la fonction de rappel pour les événements clavier
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods);

void show_fps(GLFWwindow* window);

int main() {
    // Démarrer le contexte GL et la fenêtre OS avec la bibliothèque GLFW
    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        return 1;
    }

    // Paramètres de contexte OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Créer une fenêtre GLFW
    GLFWwindow* window = NULL ; 
    if(full_screen){
        GLFWmonitor* moniteur = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(moniteur);
        window = glfwCreateWindow(mode->width, mode->height, app_title, moniteur, NULL);
    }
    
  else{  
   window= glfwCreateWindow(app_width, app_height, app_title, NULL, NULL);

  }
    if (window == NULL) {
        std::cerr << "Error: Could not create window" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Créer le contexte OpenGL
    glfwMakeContextCurrent(window);

    // Enregistrer la fonction de rappel pour les événements clavier
    glfwSetKeyCallback(window, glfw_onKey);

    // Initialiser GLEW
    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: Could not initialize GLEW" << std::endl;
        return 1;
    }

    // Boucle principale de rendu
    while (!glfwWindowShouldClose(window)) {
        // Événements d'entrée utilisateur
        glfwPollEvents();
        show_fps(window);

        // Mettre la couleur de fond ici
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Échanger les buffers (double buffering)
        glfwSwapBuffers(window);
    }

    // Terminer GLFW (doit être après la boucle principale)
    glfwTerminate();

    return 0;
}

// Fonction de rappel pour gérer les événements clavier
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Fermer la fenêtre si la touche Échap est pressée
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
void show_fps(GLFWwindow* window){
     static double previous_seconds = 0.0;
     static int frame_count = 0;
     double elapsed_seconds;
     double current_seconds = glfwGetTime(); // Retourne le nombre(double) de secondes écoulées depuis l'initialisation de GLFW

        elapsed_seconds = current_seconds - previous_seconds;
    if(elapsed_seconds > 0.25){
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;
        double ms_per_frame = 1000.0 / fps;
        std:: ostringstream outs; 
        outs.precision(3); // 3 chiffres significatifs
        outs << std::fixed
        << app_title << " "
        << "FPS: " << fps << " "
        << "Frame Time: " << ms_per_frame << "(ms)";
        glfwSetWindowTitle(window, outs.str().c_str());
        frame_count = 0;


    }
frame_count++;


};
