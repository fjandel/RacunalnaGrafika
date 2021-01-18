#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "trajectory.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

// camera
Camera camera(glm::vec3(5.5f, 4.5f, -1.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B Spline", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // ------------------------------
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("res/shaders/vertex.shader", "res/shaders/fragment.shader");

    // load model
    // -----------
    Model ourModel("res/objects/f16.obj");
    //Model ourModel("747.obj");

    // load trajectory
    // ---------------
    Trajectory ourTrajectory("trajectory.txt");

    // set camera front
    // ----------------
    camera.Front = glm::vec3(-0.544004, -0.323917, 0.774039);

    // set object position
    // -------------------
    glm::vec3 position(0.0f, 0.0f, 0.0f);

    // set rotation parameters
    // -----------------------
    glm::vec3 s(0.0f, 0.0f, 1.0f);
    glm::vec3 e = glm::normalize(ourTrajectory.vertices[0].Tangent);
    glm::vec3 os = glm::cross(s, e);
    float angle = acos(glm::dot(s, e));

    // tangent vertices
    // ----------------
    std::vector<Vertex> tan_vertices;
    Vertex tan_first;
    tan_first.Position = glm::vec3(0.0f, 0.0f, 0.0f);
    tan_vertices.push_back(tan_first);
    Vertex tan_second;
    tan_second.Position = e;
    tan_vertices.push_back(tan_second);

    unsigned int tan_indices[] = {
        0, 1 
    };

    // tangent buffers
    // ---------------
    unsigned int VBO_T, VAO_T, EBO_T;
    glGenVertexArrays(1, &VAO_T);
    glGenBuffers(1, &VBO_T);
    glGenBuffers(1, &EBO_T);

    glBindVertexArray(VAO_T);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
    glBufferData(GL_ARRAY_BUFFER, tan_vertices.size() * sizeof(Vertex), &tan_vertices[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_T);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tan_indices), tan_indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glBindVertexArray(0);

    int fpsCounter = 0;
    int index = 0;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // object speed and orientation logic
        // ------------------
        fpsCounter++;
        if (fpsCounter % 45 == 0)
        {
            index++;
            if (index >= ourTrajectory.vertices.size()) index = 0;

            position = ourTrajectory.vertices[index].Position;

            e = glm::normalize(ourTrajectory.vertices[index].Tangent);
            angle = acos(glm::dot(s, e));
            os = glm::cross(s, e);

            tan_vertices.at(1).Position = e;
            glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
            glBufferData(GL_ARRAY_BUFFER, tan_vertices.size() * sizeof(Vertex), &tan_vertices[0], GL_DYNAMIC_DRAW);
        }

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.57f, 0.76f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // enable shader before setting uniforms
        // -------------------------------------
        ourShader.use();

        // view/projection transformations
        // -------------------------------
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        // -----------------------
        glm::mat4 object_model = glm::mat4(1.0f);
        object_model = glm::translate(object_model, position);
        object_model = glm::rotate(object_model, angle, os);
        ourShader.setMat4("model", object_model);
        ourShader.setVec4("ourColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        ourModel.Draw(ourShader);

        // render the loaded trajectory
        // ----------------------------
        glm::mat4 trajectory_model = glm::mat4(1.0f);
        ourShader.setMat4("model", trajectory_model);
        ourShader.setVec4("ourColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        ourTrajectory.Draw();

        // render tangent
        // --------------
        glm::mat4 tangent_model = glm::mat4(1.0f);
        tangent_model = glm::translate(tangent_model, position);
        tangent_model = glm::scale(tangent_model, glm::vec3(2.0f, 2.0f, 2.0f));
        ourShader.setMat4("model", tangent_model);
        ourShader.setVec4("ourColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO_T);
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(PRINT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}