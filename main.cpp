// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>

// clang-format on

// PATHS
std::string path = std::filesystem::current_path();
// SPHERE
std::string sphere_obj = path + "/resources/models/sphere/shpere.obj";
// BACKGROUND
std::string space_tex = path + "/resources/textures/space_texture.jpg";
// SUN
std::string sun_tex = path + "/resources/textures/sun_texture.jpg";
// MERCURY
std::string mercury_tex = path + "/resources/textures/mercury_texture.jpg";
// VENUS
std::string venus_tex = path + "/resources/textures/venus_texture.jpg";
// EARTH
std::string earth_tex = path + "/resources/textures/earth_texture.jpg";
// MARS
std::string mars_tex = path + "/resources/textures/mars_texture.jpg";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

unsigned int loadTexture(char const* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading
    // model).
    //    stbi_set_flip_vertically_on_load(true);
    // invert the texture coordinates on the z-axis
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("model_loading.vs", "model_loading.fs");
    Shader lightingShader("lighting.vs", "lighting.fs");
    Shader sunlightShader("sunlight.vs", "sunlight.fs");

    // load models

    Model spaceModel(sphere_obj);

    Model earthModel(sphere_obj);
    Model sunModel(sphere_obj);
    Model mercuryModel(sphere_obj);
    Model venusModel(sphere_obj);
    Model marsModel(sphere_obj);

    unsigned int spaceMap = loadTexture(space_tex.c_str());
    unsigned int sunMap = loadTexture(sun_tex.c_str());
    unsigned int mercuryMap = loadTexture(mercury_tex.c_str());
    unsigned int venusMap = loadTexture(venus_tex.c_str());
    unsigned int earthMap = loadTexture(earth_tex.c_str());
    unsigned int marsMap = loadTexture(mars_tex.c_str());

    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // set texture to background

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.5f, 50000.0f);
        // glm::mat4 projection = glm::perspective(
        //     105.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 matrixSpace = glm::mat4(1.0f);
        glm::mat4 matrixSun = glm::mat4(1.0f);
        glm::mat4 matrixMercury = glm::mat4(1.0f);
        glm::mat4 matrixVenus = glm::mat4(1.0f);
        glm::mat4 matrixEarth = glm::mat4(1.0f);
        glm::mat4 matrixMars = glm::mat4(1.0f);

        // clang-format off
        matrixSpace = glm::translate(matrixSpace, glm::vec3(0.0f, 0.0f, 0.0f));
        matrixSun =     glm::translate(matrixSun, glm::vec3(0.0f, 0.0f, 0.0f));
        matrixMercury = glm::translate(matrixMercury, glm::vec3(0.0f, 0.0f, -78.0f));
        matrixVenus =   glm::translate(matrixVenus, glm::vec3(0.0f, 0.0f, -144.0f));
        matrixEarth =   glm::translate(matrixEarth, glm::vec3(0.0f, 0.0f, -200.0f));
        matrixMars =    glm::translate(matrixMars, glm::vec3(0.0f, 0.0f, -304.0f));

        matrixSpace = glm::scale(matrixSpace, glm::vec3(500.0f, 500.0f, 500.0f));
        matrixSun =     glm::scale(matrixSun, glm::vec3(1.0f));
        matrixMercury = glm::scale(matrixMercury, glm::vec3(0.033f)); //0.033
        matrixVenus =   glm::scale(matrixVenus, glm::vec3(0.095f));
        matrixEarth =   glm::scale(matrixEarth, glm::vec3(0.095f));
        matrixMars =    glm::scale(matrixMars, glm::vec3(0.053f));


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);

        // space
        glBindTexture(GL_TEXTURE_2D, spaceMap);
        ourShader.setMat4("model", matrixSpace);
        spaceModel.Draw(ourShader);

        // sun
        sunlightShader.use();
        glBindTexture(GL_TEXTURE_2D, sunMap);
        // ourShader.setMat4("model", matrixSun);
        // sunModel.Draw(ourShader);
        
        sunlightShader.setMat4("projection", projection);
        sunlightShader.setMat4("view", view);
        sunlightShader.setMat4("model", matrixSun);
        sunModel.Draw(sunlightShader);


        float ambientStrength = 1.0f;
        float diffuseStrength = 50.0f;
        float specularStrength = 50.0f;
        float shininessStrength = 64;

        lightingShader.use();
        glBindTexture(GL_TEXTURE_2D, mercuryMap);
        // ourShader.setMat4("model", matrixMercury);
        // mercuryModel.Draw(ourShader);

        // lightingShader.use();
        lightingShader.setVec3("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));
        lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        // lightingShader.setVec3("objectColor", glm::vec3(0.1f, 0.0f, 0.0f));
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("ambientStrength", ambientStrength);
        lightingShader.setFloat("diffuseStrength", diffuseStrength);
        lightingShader.setFloat("specularStrength", specularStrength);
        lightingShader.setFloat("shininessStrength", shininessStrength);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("model", matrixMercury);
        mercuryModel.Draw(lightingShader);

        // glBindTexture(GL_TEXTURE_2D, venusMap);
        // ourShader.setMat4("model", matrixVenus);
        // venusModel.Draw(ourShader);

        // glBindTexture(GL_TEXTURE_2D, earthMap);
        // ourShader.setMat4("model", matrixEarth);
        // earthModel.Draw(ourShader);

        // glBindTexture(GL_TEXTURE_2D, marsMap);
        // ourShader.setMat4("model", matrixMars);
        // marsModel.Draw(ourShader);

        // clang-format on

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
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

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos;  // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
