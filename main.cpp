#include<GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "WindowManager.h"
#include "Camera.h"

#include <iostream>

// array to store key status
bool keys[1024];
// callback to set key status
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
// callback to get mouse location
void cursor_callback(GLFWwindow *window, double x_pos, double y_pos);

void button_callback(GLFWwindow *window, int key, int scancode, int action);
// function called in the main loop to process input
void handleMovement(Camera &camera);
void handleRotation(Camera &camera, float xoffset, float yoffset);
//void handleInput(glm::vec3 &cameraPos, glm::vec3 &cameraUp, glm::vec3 &cameraFront, glm::vec3 &cameraRight);


int width, height;
Camera camera(glm::vec3(0.0f, 0.0f, 1.5f));

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CURSOR_HIDDEN, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(600, 600, "TEST", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, button_callback);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout<<"ERROR::Failed to initialize glew"<<std::endl;
        return -1;
    }
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    GLfloat cube_with_color_normal_outside[] = {
            // back Blue
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            // front Green

            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,

            // left Red
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            // right Yellow
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            // bottom Cyan
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -0.5f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -0.5f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -0.5f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -0.5f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -0.5f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, -0.5f, 0.0f,
            // up Purple
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.5f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.5f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.5f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.5f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.5f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.5f, 0.0f
    };

    GLfloat cube_with_color_normal_inside[] = {
            // back Blue
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
            // front white
            /*
            -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
             */
            // left Red
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            // right Green
            0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            // bottom White
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,  0.0f,
            // up Black
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f
    };

    Shader colorShader("../Shaders/simple_vert.glsl", "../Shaders/simple_frag.glsl");
    Shader lightingShader("../Shaders/phong_vert.glsl", "../Shaders/phong_frag.glsl");
    Shader singleColorShader("../Shaders/singleColor_vert.glsl", "../Shaders/singleColor_frag.glsl");

    GLuint wall_VAO, wall_VBO;
    GLuint cube_VAO, cube_VBO;
    GLuint lamp_VAO, lamp_VBO;

    glGenVertexArrays(1, &wall_VAO);
    glGenBuffers(1, &wall_VBO);
    glBindVertexArray(wall_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, wall_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_with_color_normal_inside), cube_with_color_normal_inside, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glGenVertexArrays(1, &cube_VAO);
    glGenBuffers(1, &cube_VBO);
    glBindVertexArray(cube_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_with_color_normal_outside), cube_with_color_normal_outside, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glGenVertexArrays(1, &lamp_VAO);
    glGenBuffers(1, &lamp_VBO);
    glBindVertexArray(lamp_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, lamp_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_with_color_normal_outside), cube_with_color_normal_outside, GL_STATIC_DRAW);
    // Set the offset to 9 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(50.0f), 1.0f, 0.1f, 100.0f);
    glm::mat4 model(1.0f);
    glm::vec3 light_pos = glm::vec3(0.0f, 0.48f, 0.0f);
    glm::mat4 cube_model = model;
    glm::mat4 light_model = glm::translate(model, light_pos);
    light_model = glm::scale(light_model, glm::vec3(0.5f, 0.02f, 0.5f));
    glm::mat4 wall_model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 ind(1.0f);



    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    float degree = 0.0f;
    float lastTime = (float) glfwGetTime();
    glClearColor(0.2f, 0.4f, 0.3f, 1.0f);
    while (glfwWindowShouldClose(window) != GL_TRUE) {
        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // Update camera
        handleMovement(camera);
        glm::mat4 view = camera.viewMatrix();

        auto currentTime = (float) glfwGetTime();
        degree += 0.3f * (currentTime - lastTime);
        lastTime = currentTime;
        if(degree > glm::radians(360.f));
            degree -= glm::radians(360.f);
        // Spin the cube
        glm::mat4 rotate= glm::rotate(ind, degree, glm::vec3(-1.0f, 1.0f, 1.0f));
        cube_model = glm::scale(model, glm::vec3(0.2f));
        cube_model = rotate * cube_model;

        // Draw the walls and cube
        glBindVertexArray(wall_VAO);
        lightingShader.use();
        lightingShader.setMat4("model", wall_model);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("light_pos", light_pos);
        lightingShader.setVec3("view_pos", camera.pos());
        lightingShader.setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 32);
        glBindVertexArray(cube_VAO);
        lightingShader.setMat4("model", cube_model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Draw the lamp;
        glBindVertexArray(lamp_VAO);
        singleColorShader.use();
        singleColorShader.setMat4("model", light_model);
        singleColorShader.setMat4("view", view);
        singleColorShader.setMat4("projection", projection);
        singleColorShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE&&action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}


static bool pressed = false;

void button_callback(GLFWwindow *window, int key, int action, int mode) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
        pressed = true;
    }
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}



void cursor_callback(GLFWwindow *window, double x_pos, double y_pos){
    static float lastX = width /2;
    static float lastY = height /2;
    if(!keys[GLFW_MOUSE_BUTTON_LEFT]) return;
    if(pressed){
        lastX = (float)x_pos;
        lastY = (float)y_pos;
        pressed = false;
    }
    float xoffset = (float)x_pos - lastX;
    float yoffset = (float)y_pos - lastY;
    lastX = (float)x_pos; lastY = (float)y_pos;
    float sensitivity = 0.01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    handleRotation(camera, xoffset, yoffset);
    //std::cout<<"("<<x_pos<<", "<<y_pos<<")"<<std::endl;
}

void handleMovement(Camera &camera) {
    static GLfloat currentFrame = 0.0f, lastFrame = 0.0f;
    currentFrame = (GLfloat) glfwGetTime();
    GLfloat deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;


    GLfloat cameraSpeed = 2.0f * deltaTime;
    vec3 delta_dir = vec3(0.0f);
    if (keys[GLFW_KEY_W]) {
        delta_dir.z -= cameraSpeed;
    }
    if (keys[GLFW_KEY_S]) {
        delta_dir.z += cameraSpeed;
    }
    if (keys[GLFW_KEY_A]) {
        delta_dir.x -= cameraSpeed;
    }
    if (keys[GLFW_KEY_D]) {
        delta_dir.x += cameraSpeed;
    }
    if (keys[GLFW_KEY_Q]) {
        delta_dir.y +=  cameraSpeed;
    }
    if (keys[GLFW_KEY_E]) {
        delta_dir.y -= cameraSpeed;
    }
    camera.updatePos(delta_dir);

    if (keys[GLFW_KEY_R]){
        camera.reset();
    }

}

void handleRotation(Camera &camera, float xoffset, float yoffset) {
    camera.updateView(yoffset, xoffset);
}