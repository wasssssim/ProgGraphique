#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include <iostream>
#include <sstream>

const char* app_title = "Test Triangle";
const int app_width = 840;
const int app_height = 680;
bool full_screen=false;
GLFWwindow* window=NULL;
bool gwireframe = false;





// Déclaration de la fonction de rappel pour les événements clavier
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
void show_fps(GLFWwindow* window);
bool InitOpenGl();

int main() {
        if (!InitOpenGl()){

   std::cerr<< "GLW initialization failed "<<std::endl;
   return -1 ; 


    }
GLfloat vertices[] = {
    // triangle 0 
    -0.5f, 0.5f, 0.0f,       
    0.5f, 0.5f, 0.0f,     
    0.5f,  -0.5f, 0.0f,    
    -0.5f, -0.5f, 0.0f,


};
GLuint indices[] = {
    0,1,2,
    0,2,3
};



    GLuint vbo,ibo,vao;

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    // Position attribute
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);


    ShaderProgram shaderProgram;
    shaderProgram.loadShaders("../shaders/basic.vert","../shaders/basic.frag");

    // Boucle principale de rendu
    while (!glfwWindowShouldClose(window)) {
        // Événements d'entrée utilisateur
        show_fps(window);

        glfwPollEvents();


        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        GLfloat time = glfwGetTime();
        GLfloat blue_color = (sin(time) / 2) + 0.5;

        glm::vec2 pos;
        pos.x = sin(time) / 2;
        pos.y = cos(time) / 2;
        shaderProgram.setUniform("offset",pos);

        shaderProgram.setUniform("vertColor",glm::vec4(0.0f,0.0f,blue_color,1.0f));


        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        // Échanger les buffers (double buffering)
        glfwSwapBuffers(window);
    }

    // Terminer GLFW (doit être après la boucle principale)
    glDeleteVertexArrays(1,&vao);
    glfwTerminate();

    return 0;
}

// Fonction de rappel pour gérer les événements clavier
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Fermer la fenêtre si la touche Échap est pressée
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(key == GLFW_KEY_W && action == GLFW_PRESS){
        gwireframe = !gwireframe;
        if(gwireframe){
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        }
        else{ 
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        }
    }
}
void show_fps(GLFWwindow* window){
     static double previous_seconds = 0.0;
     static int frame_count = 0;
     double elapsed_seconds;
     double current_seconds = glfwGetTime(); // Retourne le nombre(double) de secondes écoulées depuis l'initialisation de GLFW

        elapsed_seconds = current_seconds - previous_seconds;
    if(elapsed_seconds > 0.15){ 
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

bool InitOpenGl(){
     // Démarrer le contexte GL et la fenêtre OS avec la bibliothèque GLFW
    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        return false;
    }

    // Paramètres de contexte OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Créer une fenêtre GLFW
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
    }
            return true ;

}
