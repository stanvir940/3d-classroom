// ================================
// ENHANCED 3D CLASSROOM
// CSE 4208 – Computer Graphics Lab
// ================================

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "basic_camera.h"
#include <iostream>
using namespace std;

// ---------------- SETTINGS ----------------
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// ---------------- CAMERA ------------------
float eyeX = 0.0f, eyeY = 5.0f, eyeZ = 15.0f;
float lookAtX = 0.0f, lookAtY = 2.0f, lookAtZ = 0.0f;
glm::vec3 UP(0.0f, 1.0f, 0.0f);
BasicCamera camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, UP);

// ---------------- TIMING ------------------
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ---------------- STATE -------------------
bool lightOn = true;
bool fanOn = false;
bool doorOpen = false;
bool windowOpen = false;
bool orbitMode = false;
bool projectorOn = false;

float fanAngle = 0.0f;
float doorAngle = 0.0f;
float windowAngle = 0.0f;
float orbitAngle = 0.0f;

// ---------------- CALLBACKS ----------------
void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
void key_callback(GLFWwindow*, int, int, int, int);

// ---------------- DRAW FUNCTION ------------
void drawCube(Shader& shader, unsigned int VAO,
              glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
              glm::vec3 color = glm::vec3(0.8f))
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1,0,0));
    model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0,1,0));
    model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0,0,1));
    model = glm::scale(model, scale);

    shader.setMat4("model", model);
    shader.setVec3("objectColor", color);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// ---------------- CLASSROOM OBJECTS ----------------
void drawDesk(Shader& shader, unsigned int VAO, glm::vec3 pos, float rotation = 0.0f)
{
    // Desktop
    drawCube(shader, VAO, pos + glm::vec3(0, 0.75f, 0), {0, rotation, 0}, {1.2f, 0.1f, 0.8f}, {0.6f, 0.4f, 0.2f});
    
    // Legs
    drawCube(shader, VAO, pos + glm::vec3(-0.5f, 0.37f, -0.3f), {0, rotation, 0}, {0.1f, 0.75f, 0.1f}, {0.3f, 0.2f, 0.1f});
    drawCube(shader, VAO, pos + glm::vec3(0.5f, 0.37f, -0.3f), {0, rotation, 0}, {0.1f, 0.75f, 0.1f}, {0.3f, 0.2f, 0.1f});
    drawCube(shader, VAO, pos + glm::vec3(-0.5f, 0.37f, 0.3f), {0, rotation, 0}, {0.1f, 0.75f, 0.1f}, {0.3f, 0.2f, 0.1f});
    drawCube(shader, VAO, pos + glm::vec3(0.5f, 0.37f, 0.3f), {0, rotation, 0}, {0.1f, 0.75f, 0.1f}, {0.3f, 0.2f, 0.1f});
}

void drawChair(Shader& shader, unsigned int VAO, glm::vec3 pos, float rotation = 0.0f)
{
    // Seat
    drawCube(shader, VAO, pos + glm::vec3(0, 0.5f, 0), {0, rotation, 0}, {0.8f, 0.1f, 0.8f}, {0.2f, 0.2f, 0.8f});
    
    // Backrest (moved to front so chair faces positive Z)
    drawCube(shader, VAO, pos + glm::vec3(0, 0.9f, 0.35f), {0, rotation, 0}, {0.8f, 0.7f, 0.1f}, {0.2f, 0.2f, 0.8f});
    
    // Legs
    drawCube(shader, VAO, pos + glm::vec3(-0.3f, 0.25f, -0.3f), {0, rotation, 0}, {0.1f, 0.5f, 0.1f}, {0.15f, 0.15f, 0.6f});
    drawCube(shader, VAO, pos + glm::vec3(0.3f, 0.25f, -0.3f), {0, rotation, 0}, {0.1f, 0.5f, 0.1f}, {0.15f, 0.15f, 0.6f});
    drawCube(shader, VAO, pos + glm::vec3(-0.3f, 0.25f, 0.3f), {0, rotation, 0}, {0.1f, 0.5f, 0.1f}, {0.15f, 0.15f, 0.6f});
    drawCube(shader, VAO, pos + glm::vec3(0.3f, 0.25f, 0.3f), {0, rotation, 0}, {0.1f, 0.5f, 0.1f}, {0.15f, 0.15f, 0.6f});
}

void drawBlackboard(Shader& shader, unsigned int VAO, glm::vec3 pos)
{
    // Board
    drawCube(shader, VAO, pos, {0, 0, 0}, {5.0f, 2.5f, 0.1f}, {0.1f, 0.3f, 0.1f});
    
    // Frame
    drawCube(shader, VAO, pos + glm::vec3(0, 1.3f, 0), {0, 0, 0}, {5.2f, 0.15f, 0.15f}, {0.5f, 0.35f, 0.2f});
    drawCube(shader, VAO, pos + glm::vec3(0, -1.3f, 0), {0, 0, 0}, {5.2f, 0.15f, 0.15f}, {0.5f, 0.35f, 0.2f});
}

void drawBookshelf(Shader& shader, unsigned int VAO, glm::vec3 pos)
{
    // Main structure
    drawCube(shader, VAO, pos, {0, 0, 0}, {1.5f, 3.0f, 0.6f}, {0.45f, 0.3f, 0.15f});
    
    // Shelves
    for (int i = 0; i < 4; i++) {
        float y = -1.3f + i * 0.9f;
        drawCube(shader, VAO, pos + glm::vec3(0, y, 0), {0, 0, 0}, {1.4f, 0.05f, 0.55f}, {0.6f, 0.4f, 0.2f});
    }
    
    // Books (colored)
    drawCube(shader, VAO, pos + glm::vec3(-0.5f, -0.9f, 0), {0, 0, 0}, {0.2f, 0.5f, 0.4f}, {0.8f, 0.2f, 0.2f});
    drawCube(shader, VAO, pos + glm::vec3(-0.2f, -0.9f, 0), {0, 0, 0}, {0.2f, 0.5f, 0.4f}, {0.2f, 0.4f, 0.8f});
    drawCube(shader, VAO, pos + glm::vec3(0.1f, -0.9f, 0), {0, 0, 0}, {0.2f, 0.5f, 0.4f}, {0.3f, 0.7f, 0.3f});
}

void drawTeacherDesk(Shader& shader, unsigned int VAO, glm::vec3 pos)
{
    // Desktop
    drawCube(shader, VAO, pos + glm::vec3(0, 0.85f, 0), {0, 0, 0}, {2.5f, 0.1f, 1.2f}, {0.55f, 0.35f, 0.15f});
    
    // Legs
    drawCube(shader, VAO, pos + glm::vec3(-1.1f, 0.42f, -0.5f), {0, 0, 0}, {0.15f, 0.85f, 0.15f}, {0.3f, 0.2f, 0.1f});
    drawCube(shader, VAO, pos + glm::vec3(1.1f, 0.42f, -0.5f), {0, 0, 0}, {0.15f, 0.85f, 0.15f}, {0.3f, 0.2f, 0.1f});
    drawCube(shader, VAO, pos + glm::vec3(-1.1f, 0.42f, 0.5f), {0, 0, 0}, {0.15f, 0.85f, 0.15f}, {0.3f, 0.2f, 0.1f});
    drawCube(shader, VAO, pos + glm::vec3(1.1f, 0.42f, 0.5f), {0, 0, 0}, {0.15f, 0.85f, 0.15f}, {0.3f, 0.2f, 0.1f});
    
    // Computer monitor
    drawCube(shader, VAO, pos + glm::vec3(0.5f, 1.1f, 0), {0, 0, 0}, {0.8f, 0.6f, 0.1f}, {0.1f, 0.1f, 0.1f});
    
    // Monitor stand
    drawCube(shader, VAO, pos + glm::vec3(0.5f, 0.95f, 0.05f), {0, 0, 0}, {0.3f, 0.1f, 0.2f}, {0.2f, 0.2f, 0.2f});
}

void drawCeilingFan(Shader& shader, unsigned int VAO, glm::vec3 pos, float angle)
{
    // Central hub
    drawCube(shader, VAO, pos, {0, 0, 0}, {0.3f, 0.3f, 0.3f}, {0.7f, 0.7f, 0.7f});
    
    // Blades
    drawCube(shader, VAO, pos + glm::vec3(0, -0.05f, 0), {0, angle, 0}, {3.0f, 0.05f, 0.4f}, {0.9f, 0.9f, 0.9f});
    drawCube(shader, VAO, pos + glm::vec3(0, -0.05f, 0), {0, angle + 90, 0}, {3.0f, 0.05f, 0.4f}, {0.9f, 0.9f, 0.9f});
}

void drawWindow(Shader& shader, unsigned int VAO, glm::vec3 pos, float openAngle)
{
    // Window frame
    drawCube(shader, VAO, pos, {0, 0, 0}, {2.5f, 3.0f, 0.15f}, {0.4f, 0.3f, 0.2f});
    
    // Glass pane (slightly transparent look with lighter color)
    drawCube(shader, VAO, pos + glm::vec3(-0.6f, 0, 0), {0, openAngle, 0}, {1.1f, 2.8f, 0.05f}, {0.7f, 0.85f, 0.95f});
    drawCube(shader, VAO, pos + glm::vec3(0.6f, 0, 0), {0, 0, 0}, {1.1f, 2.8f, 0.05f}, {0.7f, 0.85f, 0.95f});
}

void drawClock(Shader& shader, unsigned int VAO, glm::vec3 pos)
{
    // Clock face
    drawCube(shader, VAO, pos, {0, 0, 0}, {0.8f, 0.8f, 0.1f}, {0.95f, 0.95f, 0.95f});
    
    // Clock border
    drawCube(shader, VAO, pos + glm::vec3(0, 0, -0.05f), {0, 0, 0}, {0.9f, 0.9f, 0.05f}, {0.1f, 0.1f, 0.1f});
}

// ---------------- MAIN ---------------------
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Enhanced 3D Classroom", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    Shader shader("vertexShader.vs", "fragmentShader.fs");

    // ----------- CUBE DATA (position, normal, color) ----------
    float vertices[] = {
        // Front face
        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,1.0f,  1.0f,1.0f,1.0f,
         0.5f,-0.5f, 0.5f,  0.0f,0.0f,1.0f,  1.0f,1.0f,1.0f,
         0.5f, 0.5f, 0.5f,  0.0f,0.0f,1.0f,  1.0f,1.0f,1.0f,
        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,1.0f,  1.0f,1.0f,1.0f,
         0.5f, 0.5f, 0.5f,  0.0f,0.0f,1.0f,  1.0f,1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f,0.0f,1.0f,  1.0f,1.0f,1.0f,

        // Back face
         0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f, 1.0f,1.0f,1.0f,
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f, 1.0f,1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f, 1.0f,1.0f,1.0f,
         0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f, 1.0f,1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f, 1.0f,1.0f,1.0f,
         0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f, 1.0f,1.0f,1.0f,

        // Left face
        -0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,
        -0.5f,-0.5f, 0.5f, -1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,
        -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,
        -0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,
        -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,
        -0.5f, 0.5f,-0.5f, -1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,

        // Right face
         0.5f,-0.5f, 0.5f,  1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,
         0.5f, 0.5f,-0.5f,  1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,
         0.5f,-0.5f, 0.5f,  1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,
         0.5f, 0.5f,-0.5f,  1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,
         0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f,  1.0f,1.0f,1.0f,

        // Top face
        -0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,  1.0f,1.0f,1.0f,
         0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,  1.0f,1.0f,1.0f,
         0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f,  1.0f,1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,  1.0f,1.0f,1.0f,
         0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f,  1.0f,1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f,  1.0f,1.0f,1.0f,

        // Bottom face
        -0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f, 1.0f,1.0f,1.0f,
         0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f, 1.0f,1.0f,1.0f,
         0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f, 1.0f,1.0f,1.0f,
        -0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f, 1.0f,1.0f,1.0f,
         0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f, 1.0f,1.0f,1.0f,
        -0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f, 1.0f,1.0f,1.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ---------------- RENDER LOOP ----------------
    while (!glfwWindowShouldClose(window))
    {
        float current = glfwGetTime();
        deltaTime = current - lastFrame;
        lastFrame = current;

        processInput(window);

        // Animations
        if (fanOn) fanAngle += 300 * deltaTime;
        doorAngle += doorOpen ? 60*deltaTime : -60*deltaTime;
        windowAngle += windowOpen ? 50*deltaTime : -50*deltaTime;
        doorAngle = glm::clamp(doorAngle, 0.0f, 90.0f);
        windowAngle = glm::clamp(windowAngle, 0.0f, 45.0f);

        if (orbitMode) {
            orbitAngle += deltaTime * 0.3f;
            camera.eye.x = 15 * cos(orbitAngle);
            camera.eye.z = 15 * sin(orbitAngle);
            camera.eye.y = 8.0f;
        }

        glClearColor(0.85f, 0.9f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setBool("lightOn", lightOn);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                            (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        shader.setMat4("view", camera.createViewMatrix());
        // lighting
        shader.setVec3("lightDir", glm::vec3(-0.3f, -1.0f, -0.2f));
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 0.95f));
        shader.setVec3("viewPos", camera.eye);

        // ========== ROOM STRUCTURE ==========
        // Floor
        drawCube(shader, VAO, {0, -0.05f, 0}, {0,0,0}, {22, 0.1f, 20}, {0.75f, 0.75f, 0.7f});
        
        // Ceiling
        drawCube(shader, VAO, {0, 10.0f, 0}, {0,0,0}, {22, 0.1f, 20}, {0.95f, 0.95f, 0.95f});
        
        // Back wall (with blackboard)
        drawCube(shader, VAO, {0, 5, -10}, {0,0,0}, {22, 10, 0.2f}, {0.9f, 0.9f, 0.85f});
        
        // Front wall
        drawCube(shader, VAO, {0, 5, 10}, {0,0,0}, {22, 10, 0.2f}, {0.9f, 0.9f, 0.85f});
        
        // Left wall
        drawCube(shader, VAO, {-11, 5, 0}, {0,0,0}, {0.2f, 10, 20}, {0.9f, 0.9f, 0.85f});
        
        // Right wall
        drawCube(shader, VAO, {11, 5, 0}, {0,0,0}, {0.2f, 10, 20}, {0.9f, 0.9f, 0.85f});

        // ========== DOOR ==========
        drawCube(shader, VAO, {-10, 2.5f, 5}, {0, doorAngle, 0}, {0.15f, 5, 2.2f}, {0.5f, 0.35f, 0.2f});
        drawCube(shader, VAO, {-9.8f, 2.5f, 5.5f}, {0, 0, 0}, {0.1f, 0.2f, 0.2f}, {0.8f, 0.7f, 0.3f}); // Door handle

        // ========== WINDOWS ==========
        drawWindow(shader, VAO, {8, 6, -9.9f}, windowAngle);
        drawWindow(shader, VAO, {-3, 6, -9.9f}, windowAngle);

        // ========== BLACKBOARD ==========
        drawBlackboard(shader, VAO, {0, 5, -9.85f});

        // ========== CLOCK ==========
        drawClock(shader, VAO, {-5, 8, -9.85f});

        // ========== TEACHER'S AREA ==========
        drawTeacherDesk(shader, VAO, {0, 0, -7});
        drawChair(shader, VAO, {0, 0, -5.5f}, 0);

        // ========== STUDENT DESKS & CHAIRS (4 rows x 3 columns) ==========
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 3; col++) {
                float x = -5.0f + col * 5.0f;
                float z = -2.0f + row * 3.0f;
                drawDesk(shader, VAO, {x, 0, z});
                drawChair(shader, VAO, {x, 0, z - 1.2f}, 0);
            }
        }

        // ========== BOOKSHELVES ==========
        drawBookshelf(shader, VAO, {-10.2f, 1.5f, -7});
        drawBookshelf(shader, VAO, {-10.2f, 1.5f, 7});

        // ========== CEILING FANS ==========
        drawCeilingFan(shader, VAO, {-5, 9.5f, 0}, fanAngle);
        drawCeilingFan(shader, VAO, {5, 9.5f, 0}, fanAngle);

        // ========== PROJECTOR (if on) ==========
        if (projectorOn) {
            drawCube(shader, VAO, {0, 8.5f, 5}, {0, 0, 0}, {0.8f, 0.4f, 0.6f}, {0.3f, 0.3f, 0.3f});
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// ---------------- INPUT -------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = 5.0f * deltaTime;
    glm::vec3 forward = glm::normalize(camera.lookAt - camera.eye);
    glm::vec3 right = glm::normalize(glm::cross(forward, UP));

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.eye += speed * forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.eye -= speed * forward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.eye -= speed * right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.eye += speed * right;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.eye.y += speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.eye.y -= speed;

    // Camera rotation
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) camera.Pitch += 40 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) camera.Pitch -= 40 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camera.Yaw -= 40 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.Yaw += 40 * deltaTime;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_L: lightOn = !lightOn; break;
            case GLFW_KEY_F: fanOn = !fanOn; break;
            case GLFW_KEY_O: doorOpen = !doorOpen; break;
            case GLFW_KEY_P: windowOpen = !windowOpen; break;
            case GLFW_KEY_T: orbitMode = !orbitMode; break;
            case GLFW_KEY_J: projectorOn = !projectorOn; break;
        }
    }
}

// ---------------- CALLBACK ----------------
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

/* ==========================================
   CONTROLS:
   
   Movement:
   W/S - Move forward/backward
   A/D - Move left/right
   E/Q - Move up/down
   Arrow Keys - Rotate camera
   
   Features:
   L - Toggle lights
   F - Toggle ceiling fans
   O - Open/close door
   P - Open/close windows
   T - Toggle orbit camera mode
   J - Toggle projector
   ESC - Exit
   ========================================== */