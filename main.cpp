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

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;         // Size of glyph
    glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Horizontal offset to advance to next glyph
};

// clang-format on

// PATHS
std::string path = std::filesystem::current_path();

// MODELS
// --SPHERE
std::string sphere_obj = path + "/resources/models/sphere/shpere.obj";
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
// --URANUS
std::string uranus_tex = path + "/resources/textures/uranus_texture.jpg";
// --NEPTUNE
std::string neptune_tex = path + "/resources/textures/neptune_texture.jpg";

// FONTS
// --ARIAL
std::string arial_font = path + "/resources/fonts/arial.ttf";

// VARS
static int amountOfAsteroids = 1000;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;

// TODO: 60s = 1 rotation = 1 day

// Mercury
float sun_mercury_distance = -78.0f;
float r_mercury_itself = 0.0f;
float r_mercury_sun = 0.0f;
float r_mercury_itself_rate = 0.001;
float r_mercury_sun_rate = 0.001;

// Venus
float sun_venus_distance = -144.0f;
float r_venus_itself = 0.0f;
float r_venus_sun = 0.0f;
float r_venus_itself_rate = 0.001;
float r_venus_sun_rate = 0.001;

// Earth
float sun_earth_distance = -200.0f;
float r_earth_itself = 0.0f;
float r_earth_sun = 0.0f;
float r_earth_itself_rate = 0.001;
float r_earth_sun_rate = 0.001;

// Moon
float sun_moon_distance = -230.0f;
float earth_moon_distance = -50.0f;
float r_moon_itself = 0.0f;
float r_moon_earth = 0.0f;
float r_moon_itself_rate = 0.001;
float r_moon_earth_rate = 0.05;

// Mars
float sun_mars_distance = -304.0f;
float r_mars_itself = 0.0f;
float r_mars_sun = 0.0f;
float r_mars_itself_rate = 0.001;
float r_mars_sun_rate = 0.001;

// Jupiter
float sun_jupiter_distance = -778.0f;
float r_jupiter_itself = 0.0f;
float r_jupiter_sun = 0.0f;
float r_jupiter_itself_rate = 0.001;
float r_jupiter_sun_rate = 0.001;

// Saturn
float sun_saturn_distance = -1427.0f;
float r_saturn_itself = 0.0f;
float r_saturn_sun = 0.0f;
float r_saturn_itself_rate = 0.001;
float r_saturn_sun_rate = 0.001;

// Uranus
float sun_uranus_distance = -2000.0f;
float r_uranus_itself = 0.0f;
float r_uranus_sun = 0.0f;
float r_uranus_itself_rate = 0.001;
float r_uranus_sun_rate = 0.001;

// Neptune
float sun_neptune_distance = -2500.0f;
float r_neptune_itself = 0.0f;
float r_neptune_sun = 0.0f;
float r_neptune_itself_rate = 0.001;
float r_neptune_sun_rate = 0.001;

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
                          glm::vec3(0.0f, 0.5f, 0.0f));  // is this hardcoded??
    *rotationItself += rateItself;
}

// Function to rotate satellite around planet
// clang-format off
void satelliteAroundPlanet(float* rotationPlanet, 
                           float rateAroundPlanet,
                           float distanceToPlanet, 
                           glm::mat4* planet,
                           glm::mat4* satellite) {
    float satelliteX = ((distanceToPlanet) * cos(*rotationPlanet)) /2 ;
    float satelliteZ = ((distanceToPlanet) * sin(*rotationPlanet)) /2 ;

    // rotate satellite around planet
    // clang-format off


     *satellite = glm::translate(*satellite,
                                glm::vec3(satelliteX, 0.0f, satelliteZ));
    // *satellite =
    //     glm::rotate(*satellite, *rotationPlanet, glm::vec3(0.0f, 0.05f, 23.5f));
    // *satellite =
    //     glm::translate(*satellite, glm::vec3(0.0f, 0.0f, distanceToPlanet));
    *rotationPlanet += rateAroundPlanet;
    
    // glm::mat4 satelliteTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(satelliteX, 0, satelliteZ));

    // *satellite = (*planet) * glm::rotate(*satellite, glm::radians(*rotationPlanet), glm::vec3(0, 1, 0)) * satelliteTranslation;
}

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
    Shader asteroidShader("asteroid.vs", "asteroid.fs");

    // load models

    Model spaceModel(sphere_obj);

    Model earthModel(sphere_obj);
    Model sunModel(sphere_obj);
    Model mercuryModel(sphere_obj);
    Model venusModel(sphere_obj);
    Model marsModel(sphere_obj);
    Model jupiterModel(sphere_obj);
    Model saturnModel(sphere_obj);
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
    unsigned int uranusMap = loadTexture(uranus_tex.c_str());
    unsigned int neptuneMap = loadTexture(neptune_tex.c_str());
    // Satellites
    unsigned int moonMap = loadTexture(moon_tex.c_str());

    cout << "Here 1\n";

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

        // cout << "Here 3\n";

        // asteroidShader.use();
        // asteroidShader.setMat4("projection", projection);
        // asteroidShader.setMat4("view", view);

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
        glm::mat4 matrixUranus = glm::mat4(1.0f);
        glm::mat4 matrixNeptune = glm::mat4(1.0f);

        // Satellites
        glm::mat4 matrixMoon = glm::mat4(1.0f);

        glm::mat4 matrixAsteroids[amountOfAsteroids];
        // Asteroids
        for (int i = 0; i < amountOfAsteroids; i++) {
            matrixAsteroids[i] = glm::mat4(1.0f);
        }

        // clang-format off
        // Space
        matrixSpace = glm::translate(matrixSpace, glm::vec3(0.0f, 0.0f, 0.0f));

        // Sun
        matrixSun =     glm::translate(matrixSun, glm::vec3(0.0f, 0.0f, 0.0f));

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
        // planetRotationSun(&r_earth_sun, r_earth_sun_rate, sun_earth_distance, &matrixEarth);
        planetRotationItself(&r_earth_itself, r_earth_itself_rate, &matrixEarth);
        // Moon
        matrixMoon =    glm::translate(matrixMoon, glm::vec3(0.0f, 0.0f, -210.0f));
        // planetRotationSun(&r_earth_sun, r_earth_sun_rate, sun_earth_distance, &matrixMoon);
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
            float scale = static_cast<float>((rand() % 50) / 100.0 + 0.25);
            matrixAsteroids[i] = glm::scale(matrixAsteroids[i], glm::vec3(scale));

            // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
            float rotAngle = static_cast<float>((rand() % 360));
            matrixAsteroids[i] = glm::rotate(matrixAsteroids[i], rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        
        }

        matrixSpace = glm::scale(matrixSpace, glm::vec3(50000.0f, 50000.0f, 50000.0f));
        matrixSun =     glm::scale(matrixSun, glm::vec3(1.0f));
        matrixMercury = glm::scale(matrixMercury, glm::vec3(0.033f)); //0.033
        matrixVenus =   glm::scale(matrixVenus, glm::vec3(0.095f));
        matrixEarth =   glm::scale(matrixEarth, glm::vec3(0.095f));
        matrixMars =    glm::scale(matrixMars, glm::vec3(0.053f));
        matrixJupiter = glm::scale(matrixJupiter, glm::vec3(0.5f));
        matrixSaturn =  glm::scale(matrixSaturn, glm::vec3(0.4f));
        matrixUranus =  glm::scale(matrixUranus, glm::vec3(0.3f));
        matrixNeptune = glm::scale(matrixNeptune, glm::vec3(0.3f));

        matrixMoon = glm::scale(matrixMoon, glm::vec3(0.05f));


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
        float specularStrength = 25.0f;
        float shininessStrength = 64;

        
        lightingShader.use();
        glBindTexture(GL_TEXTURE_2D, mercuryMap);
        loadPlanetWithLight(ambientStrength, diffuseStrength, specularStrength, shininessStrength, mercuryModel, view, projection, matrixMercury, lightingShader);
        // rotation around sun


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

        // clang-format on

        // glfw: swap buffers and poll IO events (keys pressed/released,
        // mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
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
