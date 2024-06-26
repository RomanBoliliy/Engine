#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Resources/ResourceManager.h"
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


glm::ivec2 g_windowSize(640, 480);


GLfloat point[] = {
     0.0f,  0.5f,0.0f,
     50.f, -50.f,0.0f,
    -50.f, -50.f,0.0f,
};


GLfloat colors[] = {
     1.0f,  0.0f,0.0f,
     0.0f, 1.0f,0.0f,
     0.0f, 0.0f,1.0f,
};

GLfloat texCoord[] = {
     0.5f,  1.0f,
     1.0f, 0.0f,
     0.0f, 0.0f
};



void glfwWindowSizeCallBack(GLFWwindow* pWindow, int width, int height) {
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, width, height);
}

void glfwKeyCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
}

int main(int argc, char** argv)
{

    using namespace std;
    /* Initialize the library */
    if (!glfwInit())
    {
        cout << "glfwInit Error!" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Engine", nullptr, nullptr);

    if (!pWindow)
    {
        cout << "glfwCreateWindow Error!" << endl;
        glfwTerminate();
        return -1;
    }


    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallBack);
    glfwSetKeyCallback(pWindow, glfwKeyCallBack);


    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if(!gladLoadGL())
    {

        std::cout<<"Can`t load GLAD"<<std::endl;
        return -1;
    }

    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
     cout<<"OpenGL version: " << glGetString(GL_VERSION) << endl;

     glClearColor(1,1,1,1);
     {
         ResourceManager resourceManager(argv[0]);
         auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
         if (!pDefaultShaderProgram)
         {
             cerr << "Can`t create shader program: " << "DefaultShader " << std::endl;
             return -1;

         }

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            std::cerr << "Can't create shader program: " << "SpriteShader" << std::endl;
            return -1;
        }


         auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

         auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTexture", "SpriteShader", 50, 100);
         pSprite->setPosition(glm::vec2(300, 100));

         GLuint points_vbo = 0;
         glGenBuffers(1, &points_vbo);
         glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
         glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

         GLuint colors_vbo = 0;
         glGenBuffers(1, &colors_vbo);
         glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
         glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

         GLuint texCoord_vbo = 0;
         glGenBuffers(1, &texCoord_vbo);
         glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
         glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

         GLuint vao = 0;
         glGenVertexArrays(1, &vao);
         glBindVertexArray(vao);

         glEnableVertexAttribArray(0);
         glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

         glEnableVertexAttribArray(1);
         glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

         glEnableVertexAttribArray(2);
         glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
         glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

         pDefaultShaderProgram->use();
         pDefaultShaderProgram->setInt("tex", 0);

         //modelMatrix
         glm::mat4 modelMatrix1 = glm::mat4(1.f);
         modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(100.f,200.f,0.f));

         glm::mat4 modelMatrix2 = glm::mat4(1.f);
         modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(590.f, 200.f, 0.f));

         //projectionMatrix
         glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

         pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

         pSpriteShaderProgram->use();
         pSpriteShaderProgram->setInt("tex", 0);
         pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);


         /* Loop until the user closes the window */
         while (!glfwWindowShouldClose(pWindow))
         {
             /* Render here */  
             glClear(GL_COLOR_BUFFER_BIT);

             pDefaultShaderProgram->use();
             glBindVertexArray(vao);
             tex->bind();


             pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix1);
             glDrawArrays(GL_TRIANGLES, 0, 3);

             pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix2);
             glDrawArrays(GL_TRIANGLES, 0, 3);

             pSprite->render();

             /* Swap front and back buffers */
             glfwSwapBuffers(pWindow);

             /* Poll for and process events */
             glfwPollEvents();
         }
     }

    glfwTerminate();
    return 0;
}
