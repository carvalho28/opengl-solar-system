// clang-format off
#define GLFW_INCLUDE_GLU
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "main.h"
#include "main-menu.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
std::vector<std::string> readInfo(const std::string& filePath);


struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;         // Size of glyph
    glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Horizontal offset to advance to next glyph
};

// PATHS
std::string path = std::filesystem::current_path();

// MODELS
// --SPHERE
std::string sphere_obj = path + "/resources/models/sphere/shpere.obj";
// -- SATURN
std::string saturn_obj = path + "/resources/models/saturn/saturn.obj";
// -- SATURN RING
std::string saturn_ring_obj = path + "/resources/models/saturn/rings.obj";
// --ASTEROID
std::string asteroid_obj = path + "/resources/models/asteroid/asteroid.obj";

// TEXTURES
// --SKYBOX
std::string space_tex = path + "/resources/textures/space_texture.jpg";
// --SUN
std::string sun_tex = path + "/resources/textures/sun_texture.jpg";
// --ASTEROID
std::string asteroid_tex = path + "/resources/textures/asteroid_texture.jpg";
// --MERCURY
std::string mercury_tex = path + "/resources/textures/mercury_texture.jpg";
// --VENUS
std::string venus_tex = path + "/resources/textures/venus_texture.jpg";
// --EARTH
std::string earth_tex = path + "/resources/textures/earth_texture.jpg";
// --MOON
std::string moon_tex = path + "/resources/textures/moon_texture.jpg";
// --MARS
std::string mars_tex = path + "/resources/textures/mars_texture.jpg";
// --JUPITER
std::string jupiter_tex = path + "/resources/textures/jupiter_texture.jpg";
// --SATURN
std::string saturn_tex = path + "/resources/textures/saturn_texture.jpg";
// --SATURN RING
std::string saturn_ring_tex =
    path + "/resources/textures/saturn_rings_texture.jpg";
// --URANUS
std::string uranus_tex = path + "/resources/textures/uranus_texture.jpg";
// --NEPTUNE
std::string neptune_tex = path + "/resources/textures/neptune_texture.jpg";

// FONTS
std::string arial_font = path + "/resources/fonts/Poppins-Regular.ttf";

bool showPlanetsMenu = false;
static int numOfPlanets = 10;
// static string curPlanetMenu[planets] = {"Earth", "Moon", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Sun", "Mercury", "Venus"};
int curPlanetMenu = 0;
glm::vec3 curPlanetPos;

// INFO
// std::vector<std::string> planetsInfo[numOfPlanets];
// std::vector<std::vector<std::string>> planetsInfo[10];

std::vector<std::string> earthInfo = readInfo("earth_info.txt");
std::vector<std::string> moonInfo = readInfo("moon_info.txt");
std::vector<std::string> marsInfo = readInfo("mars_info.txt");
std::vector<std::string> jupiterInfo = readInfo("jupiter_info.txt");
std::vector<std::string> saturnInfo = readInfo("saturn_info.txt");
std::vector<std::string> uranusInfo = readInfo("uranus_info.txt");
std::vector<std::string> neptuneInfo = readInfo("neptune_info.txt");
std::vector<std::string> sunInfo = readInfo("sun_info.txt");
std::vector<std::string> mercuryInfo = readInfo("mercury_info.txt");
std::vector<std::string> venusInfo = readInfo("venus_info.txt");

std::vector<std::string> curPlanetInfo = earthInfo;


// render the loaded model
glm::mat4 matrixSpace = glm::mat4(1.0f);
glm::mat4 matrixSun = glm::mat4(1.0f);
// Planets
glm::mat4 matrixMercury = glm::mat4(1.0f);
glm::mat4 matrixVenus = glm::mat4(1.0f);
glm::mat4 matrixEarth = glm::mat4(1.0f);
glm::mat4 matrixMars = glm::mat4(1.0f);
glm::mat4 matrixJupiter = glm::mat4(1.0f);
glm::mat4 matrixSaturn = glm::mat4(1.0f);
glm::mat4 matrixSaturnRing = glm::mat4(1.0f);
glm::mat4 matrixUranus = glm::mat4(1.0f);
glm::mat4 matrixNeptune = glm::mat4(1.0f);

// Satellites
glm::mat4 matrixMoon = glm::mat4(1.0f);
 
// VARS
static int amountOfAsteroids = 250;

// array of matrices
glm::mat4 modelMatrices[10];

// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// void processInput(GLFWwindow* window);
// void RenderText(Shader& shader, std::string text, float x, float y, float
// scale,
//                 glm::vec3 color);

std::map<GLchar, Character> Characters;
unsigned int tqVAO, tqVBO;  // Texture quads
// GLuint crosshairVAO, crosshairVBO, crosshairCBO;

// TODO: 60s = 1 rotation = 1 day

// Earth
float sun_earth_distance = -200.0f;
float r_earth_itself = 0.0f;
float r_earth_sun = 0.0f;
float r_earth_itself_rate = .001;
float r_earth_sun_rate = 0.001;

// Mercury
float sun_mercury_distance = -78.0f;
float r_mercury_itself = 0.0f;
float r_mercury_sun = 0.0f;
float r_mercury_itself_rate = r_earth_itself_rate/58.6;
float r_mercury_sun_rate = 4.15 * r_earth_sun_rate;

// Venus
float sun_venus_distance = -144.0f;
float r_venus_itself = 0.0f;
float r_venus_sun = 0.0f;
float r_venus_itself_rate = r_earth_itself_rate/243;
float r_venus_sun_rate = r_earth_sun_rate * 1.62;

// Moon
float sun_moon_distance = -200.0f;
float earth_moon_distance = -10.0f;
float r_moon_itself = 0.0f;
float r_moon_earth = 0.0f;
float r_moon_itself_rate = 0.001;
float r_moon_earth_rate = 0.05;

// Mars
float sun_mars_distance = -304.0f;
float r_mars_itself = 0.0f;
float r_mars_sun = 0.0f;
float r_mars_itself_rate = r_earth_itself_rate * (1+1/48);
float r_mars_sun_rate = r_earth_sun_rate / 1.9;

// Jupiter
float sun_jupiter_distance = -778.0f;
float r_jupiter_itself = 0.0f;
float r_jupiter_sun = 0.0f;
float r_jupiter_itself_rate = r_earth_itself_rate/0.375;
float r_jupiter_sun_rate = r_earth_sun_rate / 12;

// Saturn
float sun_saturn_distance = -1727.0f;
float r_saturn_itself = 0.0f;
float r_saturn_sun = 0.0f;
float r_saturn_itself_rate = r_earth_itself_rate/0.47;
float r_saturn_sun_rate = r_earth_sun_rate / 29;

// Uranus
float sun_uranus_distance = -2100.0f;
float r_uranus_itself = 0.0f;
float r_uranus_sun = 0.0f;
float r_uranus_itself_rate = r_earth_itself_rate/0.71;
float r_uranus_sun_rate = r_earth_sun_rate / 84;

// Neptune
float sun_neptune_distance = -2500.0f;
float r_neptune_itself = 0.0f;
float r_neptune_sun = 0.0f;
float r_neptune_itself_rate = r_earth_itself_rate/0.67;
float r_neptune_sun_rate = r_earth_sun_rate / 165;

bool first_iteration = true;

// settings
const float SCR_WIDTH = 1600;
const float SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


// get position of planet based on matrix 
glm::vec3 getPlanetPos(glm::mat4 model) {
    glm::vec4 pos = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    printf("pos: %f, %f, %f\n", pos.x, pos.y, pos.z);
    return glm::vec3(pos.x, pos.y, pos.z);
}

// convert cursor position to normalized device coordinates
void getXYZ(float xPos, float yPos, glm::mat4 projection, glm::mat4 view,
            glm::mat4* modelMatrices, Shader* textShader) {
    glm::vec2 ndc = {(2.0f * xPos) / SCR_WIDTH - 1.0f,
                     1.0f - (2.0f * yPos) / SCR_HEIGHT};

    // convert ndc to world space
    glm::mat4 inverse_vp = glm::inverse(projection * view);
    glm::vec4 world_pos = inverse_vp * glm::vec4(ndc, 0.0f, 1.0f);

    // create a ray from the camera and verify that it intersects with some
    // object
    glm::vec3 ray_origin = camera.Position;
    glm::vec3 ray_direction = glm::normalize(glm::vec3(world_pos) - ray_origin);
    std::cout << "Ray origin: " << ray_origin.x << ", " << ray_origin.y << ", "
              << ray_origin.z << std::endl;
    std::cout << "Ray direction: " << ray_direction.x << ", " << ray_direction.y
              << ", " << ray_direction.z << std::endl;
}

unsigned int loadFont() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
                  << std::endl;
        return -1;
    }

    if (arial_font.empty()) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    FT_Face face;
    if (FT_New_Face(ft, arial_font.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    } else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++) {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph"
                          << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                         face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                         face->glyph->bitmap.buffer);
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)};
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        // configure tqVAO/tqVBO for texture quads
        // -----------------------------------
        glGenVertexArrays(1, &tqVAO);
        glGenBuffers(1, &tqVBO);
        glBindVertexArray(tqVAO);
        glBindBuffer(GL_ARRAY_BUFFER, tqVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL,
                     GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

unsigned int loadTexture(char const* path) {
    std::cout << "Path no loadTexture : " << path << "\n";
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = 0;
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

// Function to load planets with light
void loadPlanetWithLight(float ambient, float diffuse, float specular,
                         float shininess, Model planet, glm::mat4 view,
                         glm::mat4 projection, glm::mat4 matrixPlanet,
                         Shader lightingShader) {
    lightingShader.setVec3("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));
    lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    // lightingShader.setVec3("objectColor", glm::vec3(0.1f, 0.0f, 0.0f));
    lightingShader.setVec3("viewPos", camera.Position);
    lightingShader.setFloat("ambientStrength", ambient);
    lightingShader.setFloat("diffuseStrength", diffuse);
    lightingShader.setFloat("specularStrength", specular);
    lightingShader.setFloat("shininessStrength", shininess);
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);
    lightingShader.setMat4("model", matrixPlanet);
    planet.Draw(lightingShader);
}

// Function to rotate planet around sun
void planetRotationSun(float* rotationSun, float rateSun, float distanceSun,
                       glm::mat4* planet) {
    // rotate planet over the sun
    *planet = glm::translate(*planet, glm::vec3(0.0f, 0.0f, abs(distanceSun)));
    *planet = glm::rotate(*planet, *rotationSun, glm::vec3(0.0f, 0.5f, 0.0f));
    *planet = glm::translate(*planet, glm::vec3(0.0f, 0.0f, distanceSun));
    *rotationSun += rateSun;
}

// Function to rotate planet around itself
void planetRotationItself(float* rotationItself, float rateItself,
                          glm::mat4* planet) {
    // *rotationItself += rateItself;
    *planet = glm::rotate(*planet, *rotationItself,
                          glm::vec3(0.0f, 0.5f, 0.2f));  // is this hardcoded??
    *rotationItself += rateItself;
}

// Function to rotate satellite around planet
// clang-format off
void satelliteAroundPlanet(float* rotationPlanet, 
                           float rateAroundPlanet,
                           float distanceToPlanet, 
                           glm::mat4* planet,
                           glm::mat4* satellite) {
    float satelliteX = ((distanceToPlanet) * cos((*rotationPlanet)));
    float satelliteZ = ((distanceToPlanet) * sin((*rotationPlanet)));
    // clang-format off

     *satellite = glm::translate(*satellite,
                                glm::vec3(satelliteZ, 0.0f, satelliteX));
    *rotationPlanet += rateAroundPlanet;
}

// keyboard callback
void keyboardCallbacks(GLFWwindow *window, int key, int scancode, int action,
                       int mods) {

    // restart
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
        if (showPlanetsMenu)
        {
            if (curPlanetMenu + 1 == numOfPlanets)
                curPlanetMenu = 0;
            else
                curPlanetMenu++;
            switch (curPlanetMenu)
            {
            case 0:
                curPlanetInfo = earthInfo;
                curPlanetPos = getPlanetPos(matrixEarth);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 1:
                curPlanetInfo = moonInfo;
                curPlanetPos = getPlanetPos(matrixMoon);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 2:
                curPlanetInfo = marsInfo;
                curPlanetPos = getPlanetPos(matrixMars);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 3:
                curPlanetInfo = jupiterInfo;
                curPlanetPos = getPlanetPos(matrixJupiter);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 4:
                curPlanetInfo = saturnInfo;
                curPlanetPos = getPlanetPos(matrixSaturn);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 5:
                curPlanetInfo = uranusInfo;
                curPlanetPos = getPlanetPos(matrixUranus);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 6:
                curPlanetInfo = neptuneInfo;
                curPlanetPos = getPlanetPos(matrixNeptune);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 7:
                curPlanetInfo = sunInfo;
                curPlanetPos = getPlanetPos(matrixSun);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 8:
                curPlanetInfo = mercuryInfo;
                curPlanetPos = getPlanetPos(matrixMercury);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 9:
                curPlanetInfo = venusInfo;
                curPlanetPos = getPlanetPos(matrixVenus);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            default:
                curPlanetInfo = earthInfo;
                curPlanetPos = getPlanetPos(matrixEarth);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            }
        }
        
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT)) {
        if (showPlanetsMenu)
        {
            if (curPlanetMenu - 1 == -1)
                curPlanetMenu = numOfPlanets - 1;
            else
                curPlanetMenu--;
            switch (curPlanetMenu)
            {
            case 0:
                curPlanetInfo = earthInfo;
                curPlanetPos = getPlanetPos(matrixEarth);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 1:
                curPlanetInfo = moonInfo;
                curPlanetPos = getPlanetPos(matrixMoon);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 2:
                curPlanetInfo = marsInfo;
                curPlanetPos = getPlanetPos(matrixMars);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 3:
                curPlanetInfo = jupiterInfo;
                curPlanetPos = getPlanetPos(matrixJupiter);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 4:
                curPlanetInfo = saturnInfo;
                curPlanetPos = getPlanetPos(matrixSaturn);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 5:
                curPlanetInfo = uranusInfo;
                curPlanetPos = getPlanetPos(matrixUranus);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 6:
                curPlanetInfo = neptuneInfo;
                curPlanetPos = getPlanetPos(matrixNeptune);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 7:
                curPlanetInfo = sunInfo;
                curPlanetPos = getPlanetPos(matrixSun);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 8:
                curPlanetInfo = mercuryInfo;
                curPlanetPos = getPlanetPos(matrixMercury);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 9:
                curPlanetInfo = venusInfo;
                curPlanetPos = getPlanetPos(matrixVenus);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            default:
                curPlanetInfo = earthInfo;
                curPlanetPos = getPlanetPos(matrixEarth);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            }
        }
        
    }
                        
    // text menu
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        showPlanetsMenu = !showPlanetsMenu;
        if (showPlanetsMenu) {
            switch (curPlanetMenu)
            {
            case 0:
                curPlanetInfo = earthInfo;
                curPlanetPos = getPlanetPos(matrixEarth);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 1:
                curPlanetInfo = moonInfo;
                curPlanetPos = getPlanetPos(matrixMoon);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 2:
                curPlanetInfo = marsInfo;
                curPlanetPos = getPlanetPos(matrixMars);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 3:
                curPlanetInfo = jupiterInfo;
                curPlanetPos = getPlanetPos(matrixJupiter);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 4:
                curPlanetInfo = saturnInfo;
                curPlanetPos = getPlanetPos(matrixSaturn);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 5:
                curPlanetInfo = uranusInfo;
                curPlanetPos = getPlanetPos(matrixUranus);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 6:
                curPlanetInfo = neptuneInfo;
                curPlanetPos = getPlanetPos(matrixNeptune);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 7:
                curPlanetInfo = sunInfo;
                curPlanetPos = getPlanetPos(matrixSun);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 8:
                curPlanetInfo = mercuryInfo;
                curPlanetPos = getPlanetPos(matrixMercury);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            case 9:
                curPlanetInfo = venusInfo;
                curPlanetPos = getPlanetPos(matrixVenus);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            default:
                curPlanetInfo = earthInfo;
                curPlanetPos = getPlanetPos(matrixEarth);
                camera.Position = glm::vec3(curPlanetPos.x, 0.0f, curPlanetPos.z);
                break;
            }
        }
    }
    
    // move up on long press
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {

    }
    // move down on long press
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {

    }
    // reset model on r press
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        showPlanetsMenu = false;
        r_mercury_sun = 0;
        r_venus_sun = 0;
        r_earth_sun = 0;
        r_mars_sun = 0;
        r_jupiter_sun = 0;
        r_saturn_sun = 0;
        r_uranus_sun = 0;
        r_neptune_itself = 0;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        r_mercury_sun_rate *= 2;
        r_venus_sun_rate *= 2;
       r_earth_sun_rate *= 2;
       r_mars_sun_rate *= 2;
       r_jupiter_sun_rate *= 2;
       r_saturn_sun_rate *= 2;
       r_uranus_sun_rate *= 2;
       r_neptune_sun_rate *= 2;
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
       r_mercury_sun_rate /= 2;
        r_venus_sun_rate /= 2;
       r_earth_sun_rate /= 2;
       r_mars_sun_rate /= 2;
       r_jupiter_sun_rate /= 2;
       r_saturn_sun_rate /= 2;
       r_uranus_sun_rate /= 2;
       r_neptune_sun_rate /= 2;
    }

}

int solarSystemRender() {
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
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse MOUSE ENABLE
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // blend for text
    // glEnable(GL_CULL_FACE); // was disabled by default
    // glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile shaders
    // -------------------------
    Shader ourShader("model_loading.vs", "model_loading.fs");
    Shader lightingShader("lighting.vs", "lighting.fs");
    Shader sunlightShader("sunlight.vs", "sunlight.fs");
    Shader asteroidShader("asteroid.vs", "asteroid.fs");
    Shader textShader("text.vs", "text.fs");

    // load models
    Model spaceModel(sphere_obj); // skybox sphere

    Model sunModel(sphere_obj);
    Model mercuryModel(sphere_obj);
    Model venusModel(sphere_obj);
    Model earthModel(sphere_obj);
    Model marsModel(sphere_obj);
    Model jupiterModel(sphere_obj);
    Model saturnModel(saturn_obj);
    Model saturnRingModel(saturn_ring_obj);
    Model uranusModel(sphere_obj);
    Model neptuneModel(sphere_obj);
    Model moonModel(sphere_obj);

    // Model asteroidModel(asteroid_obj);
    Model asteroidModels[amountOfAsteroids];
    // Array of asteroid models
    for (int i = 0; i < amountOfAsteroids; i++) {
        asteroidModels[i] = Model(asteroid_obj);
    }

    // Other space elements
    unsigned int spaceMap = loadTexture(space_tex.c_str());
    unsigned int sunMap = loadTexture(sun_tex.c_str());
    unsigned int asteroidMap = loadTexture(asteroid_tex.c_str());
    // Planets
    unsigned int mercuryMap = loadTexture(mercury_tex.c_str());
    unsigned int venusMap = loadTexture(venus_tex.c_str());
    unsigned int earthMap = loadTexture(earth_tex.c_str());
    unsigned int marsMap = loadTexture(mars_tex.c_str());
    unsigned int jupiterMap = loadTexture(jupiter_tex.c_str());
    unsigned int saturnMap = loadTexture(saturn_tex.c_str());
    unsigned int saturnRingMap = loadTexture(saturn_ring_tex.c_str());
    unsigned int uranusMap = loadTexture(uranus_tex.c_str());
    unsigned int neptuneMap = loadTexture(neptune_tex.c_str());
    // Satellites
    unsigned int moonMap = loadTexture(moon_tex.c_str());

    glm::mat4 projection_hud = glm::ortho(0.0f, SCR_WIDTH, 0.0f, SCR_HEIGHT);

    glEnable(GL_CULL_FACE); // was disabled by default
    glEnable(GL_BLEND);
    loadFont();
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

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

        // on mouse click get world position
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            // get mouse position
            double xpos, ypos;
            xpos = lastX;
            ypos = lastY;
            getXYZ(xpos, ypos, projection, view, modelMatrices, &textShader);
        }

        // cout << "Here 3\n";

        // asteroidShader.use();
        // asteroidShader.setMat4("projection", projection);
        // asteroidShader.setMat4("view", view);


        // // render the loaded model
        // glm::mat4 matrixSpace = glm::mat4(1.0f);
        // glm::mat4 matrixSun = glm::mat4(1.0f);
        // // Planets
        // glm::mat4 matrixMercury = glm::mat4(1.0f);
        // glm::mat4 matrixVenus = glm::mat4(1.0f);
        // glm::mat4 matrixEarth = glm::mat4(1.0f);
        // glm::mat4 matrixMars = glm::mat4(1.0f);
        // glm::mat4 matrixJupiter = glm::mat4(1.0f);
        // glm::mat4 matrixSaturn = glm::mat4(1.0f);
        // glm::mat4 matrixSaturnRing = glm::mat4(1.0f);
        // glm::mat4 matrixUranus = glm::mat4(1.0f);
        // glm::mat4 matrixNeptune = glm::mat4(1.0f);

        // // Satellites
        // glm::mat4 matrixMoon = glm::mat4(1.0f);

        // render the loaded model
        matrixSpace = glm::mat4(1.0f);
        matrixSun = glm::mat4(1.0f);
        // Planets
        matrixMercury = glm::mat4(1.0f);
        matrixVenus = glm::mat4(1.0f);
        matrixEarth = glm::mat4(1.0f);
        matrixMars = glm::mat4(1.0f);
        matrixJupiter = glm::mat4(1.0f);
        matrixSaturn = glm::mat4(1.0f);
        matrixSaturnRing = glm::mat4(1.0f);
        matrixUranus = glm::mat4(1.0f);
        matrixNeptune = glm::mat4(1.0f);

        // Satellites
        matrixMoon = glm::mat4(1.0f);

        // clang-format off
        // Space
        matrixSpace = glm::translate(matrixSpace, glm::vec3(0.0f, 0.0f, 0.0f));

        // Sun
        matrixSun =     glm::translate(matrixSun, glm::vec3(0.0f, 0.0f, 0.0f));

        if (first_iteration) {

            int i = 1000;
            while (i > 0) {
            // Mercury
                matrixMercury = glm::translate(matrixMercury, glm::vec3(0.0f, 0.0f, sun_mercury_distance));
                planetRotationSun(&r_mercury_sun, r_mercury_sun_rate, sun_mercury_distance, &matrixMercury);
                planetRotationItself(&r_mercury_itself, r_mercury_itself_rate, &matrixMercury);
                i--;
            }

            i = 5000;
            while (i > 0) {
                // Venus
                matrixVenus =   glm::translate(matrixVenus, glm::vec3(0.0f, 0.0f, -144.0f));
                planetRotationSun(&r_venus_sun, r_venus_sun_rate, sun_venus_distance, &matrixVenus);
                planetRotationItself(&r_venus_itself, r_venus_itself_rate, &matrixVenus);
                i--;
            }

            i = 10000;
            while (i > 0) {
                // Earth
                matrixEarth =   glm::translate(matrixEarth, glm::vec3(0.0f, 0.0f, -200.0f));
                planetRotationSun(&r_earth_sun, r_earth_sun_rate, sun_earth_distance, &matrixEarth);
                planetRotationItself(&r_earth_itself, r_earth_itself_rate, &matrixEarth);
                // Moon
                matrixMoon =    glm::translate(matrixMoon, glm::vec3(0.0f, 0.0f, -200.0f));
                planetRotationSun(&r_earth_sun, r_earth_sun_rate, sun_earth_distance, &matrixMoon);
                satelliteAroundPlanet(&r_moon_earth, r_moon_earth_rate, earth_moon_distance,
                &matrixEarth, &matrixMoon);
                // planetRotationItself(&r_moon_itself, r_moon_itself_rate, &matrixMoon);
                i--;
            }
            i = 7000;
            while (i > 0) {    
                // Mars
                matrixMars =    glm::translate(matrixMars, glm::vec3(0.0f, 0.0f, -304.0f));
                planetRotationSun(&r_mars_sun, r_mars_sun_rate, sun_mars_distance, &matrixMars);
                planetRotationItself(&r_mars_itself, r_mars_itself_rate, &matrixMars);
                i--;
            }
            i = 100;
            while (i > 0) {
                // Jupiter
                matrixJupiter = glm::translate(matrixJupiter, glm::vec3(0.0f, 0.0f, -778.0f));
                planetRotationSun(&r_jupiter_sun, r_jupiter_sun_rate, sun_jupiter_distance, &matrixJupiter);
                planetRotationItself(&r_jupiter_itself, r_jupiter_itself_rate, &matrixJupiter);
                i--;
            }
            i = 2000;
            while (i > 0) {
                // Saturn
                matrixSaturn =  glm::translate(matrixSaturn, glm::vec3(0.0f, 0.0f, -1427.0f));
                planetRotationSun(&r_saturn_sun, r_saturn_sun_rate, sun_saturn_distance, &matrixSaturn);
                planetRotationItself(&r_saturn_itself, r_saturn_itself_rate, &matrixSaturn);

                // Saturn Ring
                matrixSaturnRing = glm::translate(matrixSaturnRing, glm::vec3(0.0f, 0.0f, -1427.0f));
                planetRotationSun(&r_saturn_sun, r_saturn_sun_rate, sun_saturn_distance, &matrixSaturnRing);
                planetRotationItself(&r_saturn_itself, r_saturn_itself_rate, &matrixSaturnRing);
                i--;
            }
            i = 300000;
            while (i > 0) {
                // Uranus
                matrixUranus =  glm::translate(matrixUranus, glm::vec3(0.0f, 0.0f, -2000.0f));
                planetRotationSun(&r_uranus_sun, r_uranus_sun_rate, sun_uranus_distance, &matrixUranus);
                planetRotationItself(&r_uranus_itself, r_uranus_itself_rate, &matrixUranus);
                i--;
            }
            i = 700000;
            while (i > 0) {
                // Neptune
                matrixNeptune = glm::translate(matrixNeptune, glm::vec3(0.0f, 0.0f, -2500.0f));
                planetRotationSun(&r_neptune_sun, r_neptune_sun_rate, sun_neptune_distance, &matrixNeptune);
                planetRotationItself(&r_neptune_itself, r_neptune_itself_rate, &matrixNeptune);
                i--;
            }
        }

        // Mercury
        matrixMercury = glm::translate(matrixMercury, glm::vec3(0.0f, 0.0f, sun_mercury_distance));
        planetRotationSun(&r_mercury_sun, r_mercury_sun_rate, sun_mercury_distance, &matrixMercury);
        planetRotationItself(&r_mercury_itself, r_mercury_itself_rate, &matrixMercury);

        // Venus
        matrixVenus =   glm::translate(matrixVenus, glm::vec3(0.0f, 0.0f, -144.0f));
        planetRotationSun(&r_venus_sun, r_venus_sun_rate, sun_venus_distance, &matrixVenus);
        planetRotationItself(&r_venus_itself, r_venus_itself_rate, &matrixVenus);

        // Earth
        matrixEarth =   glm::translate(matrixEarth, glm::vec3(0.0f, 0.0f, -200.0f));
        planetRotationSun(&r_earth_sun, r_earth_sun_rate, sun_earth_distance, &matrixEarth);
        planetRotationItself(&r_earth_itself, r_earth_itself_rate, &matrixEarth);
        // Moon
        matrixMoon =    glm::translate(matrixMoon, glm::vec3(0.0f, 0.0f, -200.0f));
        planetRotationSun(&r_earth_sun, r_earth_sun_rate, sun_earth_distance, &matrixMoon);
        satelliteAroundPlanet(&r_moon_earth, r_moon_earth_rate, earth_moon_distance,
        &matrixEarth, &matrixMoon);
        // planetRotationItself(&r_moon_itself, r_moon_itself_rate, &matrixMoon);
        
        // Mars
        matrixMars =    glm::translate(matrixMars, glm::vec3(0.0f, 0.0f, -304.0f));
        planetRotationSun(&r_mars_sun, r_mars_sun_rate, sun_mars_distance, &matrixMars);
        planetRotationItself(&r_mars_itself, r_mars_itself_rate, &matrixMars);

        // Jupiter
        matrixJupiter = glm::translate(matrixJupiter, glm::vec3(0.0f, 0.0f, -778.0f));
        planetRotationSun(&r_jupiter_sun, r_jupiter_sun_rate, sun_jupiter_distance, &matrixJupiter);
        planetRotationItself(&r_jupiter_itself, r_jupiter_itself_rate, &matrixJupiter);

        // Saturn
        matrixSaturn =  glm::translate(matrixSaturn, glm::vec3(0.0f, 0.0f, -1427.0f));
        planetRotationSun(&r_saturn_sun, r_saturn_sun_rate, sun_saturn_distance, &matrixSaturn);
        planetRotationItself(&r_saturn_itself, r_saturn_itself_rate, &matrixSaturn);

        // Saturn Ring
        matrixSaturnRing = glm::translate(matrixSaturnRing, glm::vec3(0.0f, 0.0f, -1427.0f));
        planetRotationSun(&r_saturn_sun, r_saturn_sun_rate, sun_saturn_distance, &matrixSaturnRing);
        planetRotationItself(&r_saturn_itself, r_saturn_itself_rate, &matrixSaturnRing);

        // Uranus
        matrixUranus =  glm::translate(matrixUranus, glm::vec3(0.0f, 0.0f, -2000.0f));
        planetRotationSun(&r_uranus_sun, r_uranus_sun_rate, sun_uranus_distance, &matrixUranus);
        planetRotationItself(&r_uranus_itself, r_uranus_itself_rate, &matrixUranus);

        // Neptune
        matrixNeptune = glm::translate(matrixNeptune, glm::vec3(0.0f, 0.0f, -2500.0f));
        planetRotationSun(&r_neptune_sun, r_neptune_sun_rate, sun_neptune_distance, &matrixNeptune);
        planetRotationItself(&r_neptune_itself, r_neptune_itself_rate, &matrixNeptune);
        
        srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
        float radius = 400.0f; // default is 50.0
        float offset = 2.5f;

        // Asteroids
        glm::mat4 matrixAsteroids[amountOfAsteroids];
        // Asteroids
        for (int i = 0; i < amountOfAsteroids; i++) {
            matrixAsteroids[i] = glm::mat4(1.0f);
        }

        for (int i = 0; i < amountOfAsteroids; i++) {
            matrixAsteroids[i] = glm::translate(matrixAsteroids[i], glm::vec3(0.0f, 0.0f, 0.0f));
            // matrixAsteroids[i] = glm::scale(matrixAsteroids[i], glm::vec3(3.0f, 3.0f, 3.0f));
            float angle = (float)i / (float)amountOfAsteroids * 360.0f;
            float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;

            matrixAsteroids[i] = glm::translate(matrixAsteroids[i], glm::vec3(x, y, z));

            // 2. scale: Scale between 0.05 and 0.25f
            // float scale = static_cast<float>((rand() % 50) / 100.0 + 0.25);
            float scale = static_cast<float>((rand() % 180) / 200.0 + 0.50);
            matrixAsteroids[i] = glm::scale(matrixAsteroids[i], glm::vec3(scale));

            // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
            float rotAngle = static_cast<float>((rand() % 360));
            matrixAsteroids[i] = glm::rotate(matrixAsteroids[i], rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        
        }

        matrixSpace = glm::scale(matrixSpace, glm::vec3(70.0f));
        matrixSun =     glm::scale(matrixSun, glm::vec3(1.0f));
        matrixMercury = glm::scale(matrixMercury, glm::vec3(0.033f)); //0.033
        matrixVenus =   glm::scale(matrixVenus, glm::vec3(0.095f));
        matrixEarth =   glm::scale(matrixEarth, glm::vec3(0.095f));
        matrixMars =    glm::scale(matrixMars, glm::vec3(0.053f));
        matrixJupiter = glm::scale(matrixJupiter, glm::vec3(0.8f));
        matrixSaturn =  glm::scale(matrixSaturn, glm::vec3(35.0f));
        matrixSaturnRing = glm::scale(matrixSaturnRing, glm::vec3(35.0f));
        matrixUranus =  glm::scale(matrixUranus, glm::vec3(0.3f));
        matrixNeptune = glm::scale(matrixNeptune, glm::vec3(0.3f));

        matrixMoon = glm::scale(matrixMoon, glm::vec3(0.025f));

        modelMatrices[0] = matrixSun;
        modelMatrices[1] = matrixMercury;
        modelMatrices[2] = matrixVenus;
        modelMatrices[3] = matrixEarth;
        modelMatrices[4] = matrixMars;
        modelMatrices[5] = matrixJupiter;
        modelMatrices[6] = matrixSaturn;
        modelMatrices[7] = matrixUranus;
        modelMatrices[8] = matrixNeptune;
        modelMatrices[9] = matrixMoon;

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

        float ambientStrength = 3.0f;
        float diffuseStrength = 15.0f;
        float specularStrength = 4.0f;
        float shininessStrength = 4;

        
        lightingShader.use();
        glBindTexture(GL_TEXTURE_2D, mercuryMap);
        loadPlanetWithLight(ambientStrength, diffuseStrength, specularStrength, shininessStrength, mercuryModel, view, projection, matrixMercury, lightingShader);

        glBindTexture(GL_TEXTURE_2D, venusMap);
        loadPlanetWithLight(ambientStrength, diffuseStrength, specularStrength, shininessStrength, venusModel, view, projection, matrixVenus, lightingShader);

        glBindTexture(GL_TEXTURE_2D, earthMap);
        loadPlanetWithLight(ambientStrength, diffuseStrength, specularStrength, shininessStrength, earthModel, view, projection, matrixEarth, lightingShader);

        glBindTexture(GL_TEXTURE_2D, marsMap);
        loadPlanetWithLight(ambientStrength, diffuseStrength, specularStrength, shininessStrength, marsModel, view, projection, matrixMars, lightingShader);
        
        glBindTexture(GL_TEXTURE_2D, jupiterMap);
        loadPlanetWithLight(ambientStrength, diffuseStrength, specularStrength, shininessStrength, jupiterModel, view, projection, matrixJupiter, lightingShader);

        glBindTexture(GL_TEXTURE_2D, saturnMap);
        loadPlanetWithLight(ambientStrength, diffuseStrength, specularStrength, shininessStrength, saturnModel, view, projection, matrixSaturn, lightingShader);

        glBindTexture(GL_TEXTURE_2D, saturnRingMap);
        loadPlanetWithLight(ambientStrength*3, diffuseStrength*5*5, specularStrength, shininessStrength *32, saturnRingModel, view, projection, matrixSaturnRing, lightingShader);
        
        glBindTexture(GL_TEXTURE_2D, uranusMap);
        loadPlanetWithLight(ambientStrength, diffuseStrength, specularStrength, shininessStrength, uranusModel, view, projection, matrixUranus, lightingShader);
        
        glBindTexture(GL_TEXTURE_2D, neptuneMap);
        loadPlanetWithLight(ambientStrength, diffuseStrength, specularStrength, shininessStrength, neptuneModel, view, projection, matrixNeptune, lightingShader);

        for (int i = 0; i < amountOfAsteroids; i++) {
            glBindTexture(GL_TEXTURE_2D, asteroidMap);
            loadPlanetWithLight(ambientStrength, diffuseStrength, specularStrength, shininessStrength, asteroidModels[i], view, projection, matrixAsteroids[i], lightingShader);
        }

        // Moon
        glBindTexture(GL_TEXTURE_2D, moonMap);
        loadPlanetWithLight(ambientStrength, diffuseStrength, specularStrength, shininessStrength, moonModel, view, projection, matrixMoon, lightingShader);

        if (showPlanetsMenu) {            
            // text rendering comes last
            //glm::mat4 projection_hud = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
            glEnable(GL_BLEND);
            glEnable(GL_CULL_FACE);
            textShader.use();
            // Name of the planet
            RenderText(textShader, curPlanetInfo[0], 25.0f, SCR_HEIGHT - 60.0f, 1.0f, glm::vec3(1.00, 1.00f, 1.00f)); // Star Wars Yellow
            // Description of the planet
            RenderText(textShader, curPlanetInfo[1], 25.0f, 90.0f, 0.5f, glm::vec3(1.00, 1.00f, 1.00f)); 
            RenderText(textShader, curPlanetInfo[2], 25.0f, 60.0f, 0.5f, glm::vec3(1.00, 1.00f, 1.00f)); 
            RenderText(textShader, curPlanetInfo[3], 25.0f, 30.0f, 0.5f, glm::vec3(1.00, 1.00f, 1.00f)); 
        }

        // Crosshair
        RenderText(textShader, ".", SCR_WIDTH / 2, SCR_HEIGHT / 2, 0.5f, glm::vec3(1.0, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection_hud));
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);

        // clang-format on

        // glfw: swap buffers and poll IO events (keys pressed/released,
        // mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        glFlush();
        first_iteration = false;
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    closeProgram();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released
// this frame and react accordingly
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

    // Cursor callback
    glfwSetKeyCallback(window, keyboardCallbacks);
}

// glfw: whenever the window size changed (by OS or user resize) this
// callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that
    // width and height will be significantly larger than specified on
    // retina displays.
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

// Text rendering
void RenderText(Shader& shader, std::string text, float x, float y, float scale,
                glm::vec3 color) {
    // activate corresponding render state
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y,
                color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(tqVAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, tqVBO);
        glBufferSubData(
            GL_ARRAY_BUFFER, 0, sizeof(vertices),
            vertices);  // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of
        // 1/64 pixels)
        x += (ch.Advance >> 6) *
             scale;  // bitshift by 6 to get value in pixels (2^6 = 64 (divide
                     // amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Load planets info from text file
std::vector<std::string> readInfo(const std::string& filePath) {
    std::vector<std::string> info;
    std::ifstream file(path + "/resources/info/" + filePath);

    if (!file.is_open()) {
        std::cout << "Cannot open file." << std::endl;
        return info;
    }

    // Read file line by line
    std::string line;
    while (std::getline(file, line)) {
        info.push_back(line);
    }

    file.close();

    return info;
}