 

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"

#include "stb_image.h"

#include <iostream>
#include <stdlib.h>
#include<windows.h>  

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

//Declare Object Functions Prototypes
void sea(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void sky(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void ship(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void container(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void port(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void compound(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void crane(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);


void drawCube(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color, float spec, float shininess);
 


void load_texture(unsigned int& texture, string image_name, GLenum format);

// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 800;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 0.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float dzz = 0.0f;
float dyy = 0.0f;
int mov = 0;
int test11 = 0;
int rot = 0;
float dr = 0.0f;
float shipz = 0.0f;
int shipdir = 1;
float rotateAngleTest_Y = 0.0;

// camera               
Camera camera(glm::vec3(8.0f, 1.0f, 18.1f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 2.5;
float lookAtX = 0.0, lookAtY = 1.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


glm::vec3 lightPositions[] = {
        glm::vec3(15.0, 10.0f, 15.0f),         //Directional Light
        glm::vec3(10.1f, 1.28f, 14.37f),
        glm::vec3(6.2f, 1.35f, 7.98f)

};

glm::vec3 lightDirections[] = {
    glm::vec3(0.4f, -0.2f, -1.0f)
};


DirectionalLight directionalLight(-lightPositions[0], glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), glm::vec4(0.2f, 0.2f, 0.2f, 0.2f), 1);
SpotLight spotLight(lightPositions[4], lightDirections[0], 8.5f, 15.5f, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.0014f, 0.000007f, 1);
PointLight pointLight(lightPositions[1], glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1);

// texture
float extra = 4.0f;
float TXmin = 0.0f;
float TXmax = 1.0f + extra;
float TYmin = 0.0f;
float TYmax = 1.0f + extra;



//rotate
bool isRotating = false;
float fanSpeed = 8.0f;

bool lightingOn = true;
float ambientOn = 1.0;
float diffuseOn = 1.0; 
float specularOn = 1.0;
bool dark = false;

float directionalLightOn = 1.0;
float pointLightOn = 0.0;
float spotLightOn = 0.0;


// Textures
unsigned int texture0, texture1, redBrickTex, texture3, texture4, tilesTex, texture6, texture7, skytex, watertex, doortex, shipmetaltex, cargotex, cargotex2, cargotex3, cargotex4, roadtex1, roadtex2, enginetex, cranetex1, ropetex;

int main()
{
    //Scene manual
    std::cout << "Operating Manual" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Press Escape to Close" << std::endl;
    std::cout << "Press W to Move Forward" << std::endl;
    std::cout << "Press S to Move Backward" << std::endl;
    std::cout << "Press A to Move Left" << std::endl;
    std::cout << "Press D to Move Right" << std::endl;
    std::cout << "Press E to Move Up" << std::endl;
    std::cout << "Press R to Move Down" << std::endl;
    std::cout << "Press X to Positive Pitch (+Y)" << std::endl;
    std::cout << "Press J to Negative Pitch (-Y)" << std::endl;
    std::cout << "Press Y to Positive Yaw (+X)" << std::endl;
    std::cout << "Press U to Negative Yaw (-X)" << std::endl;
    std::cout << "Press Z to Positive Roll (+Z)" << std::endl;
    std::cout << "Press Q to Negative Roll (-Z)" << std::endl;
    std::cout << "Press K to Rotate Camera Right" << std::endl;
    std::cout << "Press F to Rotate Camera Left" << std::endl;
    std::cout << "Press L to Switch All Lights" << std::endl;
    std::cout << "Press 1 to Ambient Light On" << std::endl;
    std::cout << "Press 2 to Ambient Light Off" << std::endl;
    std::cout << "Press 3 to Diffuse Light On" << std::endl;
    std::cout << "Press 4 to Diffuse Light Off" << std::endl;
    std::cout << "Press C to Point Light On" << std::endl;
    std::cout << "Press V to Point Light Off" << std::endl;
    std::cout << "Press B to Directional Light On" << std::endl;
    std::cout << "Press N to Directional Light Off" << std::endl;
    std::cout << "Press M to Spot Light On" << std::endl;
    std::cout << "Press P to Spot Light On" << std::endl;
    std::cout << "Press APOSTROPHE (') to Auto Movement Crane" << std::endl;
    std::cout << "Press O to Crane Move Forward" << std::endl;
    std::cout << "Press I to Crane Move Backward" << std::endl;
    std::cout << "Press 8 to Pull Crane Rope" << std::endl;
    std::cout << "Press 9 to Release Crane Rope" << std::endl;
    std::cout << "Press COMMA (,) to Auto Rotate Container with Rope" << std::endl;
    std::cout << "Press Space to Manually Rotate Container with Rope" << std::endl;
    std::cout << "Press Right Bracket ([) to Forward Ship" << std::endl;
    std::cout << "Press Left Bracket ([) to Backward Ship" << std::endl;






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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sea Port - Made by Fuad (1807032)", NULL, NULL);
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
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader lightCubeShader("lightVertexShader.vs", "lightFragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    

    float cube_vertices[] = {
        // positions      // normals         // texture coords
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,

        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmin,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmax,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, TXmin, TYmin,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, TXmin, TYmax,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmax,

        0.0f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,
        0.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
        0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
        0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmin,
        0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
        0.0f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, TXmin, TYmax
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
    glEnableVertexAttribArray(2);

    //light's VAO
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //Texture 0
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("transparent.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture 0" << endl;
    }
    stbi_image_free(data);


    load_texture(texture1, "new_grass.jpg", GL_RGB);
    load_texture(redBrickTex, "brickwall_2.jpg", GL_RGB);
    load_texture(texture3, "brickwall_2.jpg", GL_RGB);
    load_texture(texture4, "pillar.jpg", GL_RGB);
    load_texture(watertex, "water.jpg", GL_RGB);
    load_texture(doortex, "door.jpg", GL_RGB);
    load_texture(shipmetaltex, "shipbody7.jpg", GL_RGB);
    load_texture(cargotex, "cargo1.jpg", GL_RGB);
    load_texture(cargotex2, "cargo2.jpg", GL_RGB);
    load_texture(cargotex3, "cargo3.jpg", GL_RGB);
    load_texture(cargotex4, "cargo4.jpg", GL_RGB);
    load_texture(roadtex1, "road1.jpg", GL_RGB);
    load_texture(roadtex2, "concrete1.png", GL_RGBA);
    load_texture(enginetex, "black.jpg", GL_RGB);
    load_texture(cranetex1, "crane1.jpg", GL_RGB);
    load_texture(ropetex, "rope.jpg", GL_RGB);
    load_texture(skytex, "sky.jpg", GL_RGB);
    load_texture(tilesTex, "tiles.jpg", GL_RGB);


    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        ourShader.use();
        glBindVertexArray(cubeVAO);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = basic_camera.createViewMatrix();
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);


        //**************lighting**************

        //Setting up Directional Light
        directionalLight.setUpLight(ourShader);
        if (!directionalLightOn)
            directionalLight.turnOff();
        if (!ambientOn)
            directionalLight.turnAmbientOff();
        if(!diffuseOn)
            directionalLight.turnDiffuseOff();
        if(!specularOn)
            directionalLight.turnSpecularOff();

        //Setting up Point Light
        pointLight.setUpLight(ourShader);
        if (!pointLightOn)
            pointLight.turnOff();
        if (!ambientOn)
            pointLight.turnAmbientOff();
        if (!diffuseOn)
            pointLight.turnDiffuseOff();
        if (!specularOn)
            pointLight.turnSpecularOff();

        //Setting up Spot Light
        spotLight.setUpLight(ourShader);
        if (!spotLightOn)
            spotLight.turnOff();
        if (!ambientOn)
            spotLight.turnAmbientOff();
        if (!diffuseOn)
            spotLight.turnDiffuseOff();
        if (!specularOn)
            spotLight.turnSpecularOff();

        //Setting up Camera and Others
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setBool("lightingOn", lightingOn);


        // Starting
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, scaleMatrix;
        glm::vec4 color1;

        float chairZ = 0.4f;
        float fixY = 0.45f;
        float chairX = 0.1f;




        //********** Object making ***********

        glActiveTexture(GL_TEXTURE0);
        //Object Function Call
        //Sea
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        sea(ourShader, translateMatrix, color1);
        //Sky
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        sky(ourShader, translateMatrix, color1);
        //ship
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f + shipz));
        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        ship(ourShader, translateMatrix, color1);
        
        //ship containers
        float tt = 0.0f;
        for (int i = 0; i < 20; i++)
        {
            float ttp = 0.0f;
            for (int j = 0; j < 5; j++)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f + ttp, 0.0f + tt + shipz));
                color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                container(ourShader, translateMatrix, color1);
                ttp = ttp + 0.51f;
            }
            tt = tt + 0.60f;
        }
        //ground containers
        float tt4 = 0.0f;
        for (int f1 = 0; f1 < 5; f1++)
        {
            float ttz = 0.0f;
            for (int f = 0; f < 5; f++)
            {
                float tt1 = 0.0f;
                for (int i = 0; i < 5; i++)
                {
                    float ttp1 = 0.0f;
                    for (int j = 0; j < 1; j++)
                    {
                        translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0f + ttz, 0.0f + ttp1, -5.0f + tt1 + tt4));
                        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                        container(ourShader, translateMatrix, color1);
                        ttp1 = ttp1 + 0.51f;
                    }
                    tt1 = tt1 + 0.60f;
                }
                ttz += 5.0f;
            }
            tt4 += 5.0f;
        }
        
        

        //Port
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        port(ourShader, translateMatrix, color1);
        //Compound
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        compound(ourShader, translateMatrix, color1);
        //Crane
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f + dzz));
        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        crane(ourShader, translateMatrix, color1);

        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
//Object Function Create
void sea(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    //Sea
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-250.0f, 0.0, -250.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1000.0f, 0.1f, 1000.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, watertex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

void ship(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix;

    //SHip Body Right Side
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 5.0f, 35.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, shipmetaltex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //SHip Body Left Side
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 5.0f, 35.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, shipmetaltex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //SHip Base
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 0.3f, 43.5f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, shipmetaltex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //SHip Back
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 5.0f, 0.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, shipmetaltex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //SHip front1
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(60.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.975f, 0.0f, 17.45f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.1f, 5.0f, 0.1f));
    model = translateMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, shipmetaltex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //SHip tricover1
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(60.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.03f, 0.0f, 17.45f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(150.1f, 0.32f, 300.0f));
    model = translateMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, watertex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //SHip front2
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.05f, 0.0f, 17.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.05f, 5.0f, 0.1f));
    model = translateMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, shipmetaltex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //SHip tricover2
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(164.55f, 0.0f, 32.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(150.1f, 0.315f, 300.0f));
    model = translateMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, watertex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //SHip Other 1
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.99f, 0.0f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 4.0f, 8.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, enginetex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //SHip Other 1
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 0.1f, 16.98f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 3.0f, 0.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix* model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, doortex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


}

void container(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix;

    //Container1
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.88f, 0.0f, 0.12f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.3f, 1.0f, 1.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, cargotex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Container2
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.68f, 0.0f, 0.12f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.3f, 1.0f, 1.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, cargotex2);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Container3
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.48f, 0.0f, 0.12f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.3f, 1.0f, 1.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, cargotex3);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Container4
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.28f, 0.0f, 0.12f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.45f, 1.0f, 1.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, cargotex4);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void port(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix;

    //SHip Body Left Side
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.1f, 0.0f, -100.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(20.1f, 0.5f, 600.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, roadtex1);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

void compound(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix;

    //SHip Body Left Side
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.15f, 0.0f, -100.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(300.1f, 0.5f, 600.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, roadtex2);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

void crane(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix, rotateYMatrix, translateMatrix2, translateMatrix3;

    //Crane Leg 1
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.1f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 15.0f, 1.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, cranetex1);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Crane Leg 2
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(9.1f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 15.0f, 1.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, cranetex1);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Crane Body
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.1f, 7.5f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(20.0f, 1.0f, 1.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, cranetex1);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Crane Arm 1
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.1f, 7.5f, 0.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.9f, 13.5f));
    model = translateMatrix * rotateZMatrix  * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, cranetex1);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Crane Arm 2
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(9.7f, 7.5f, 0.145f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.91f, 14.08f));
    model = translateMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, cranetex1);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Crane Rope
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + dr), glm::vec3(0.0f, 1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 7.45f, 5.2f));
    translateMatrix2 = glm::translate(identityMatrix, glm::vec3(-0.0125f, 0.0f, -0.0125f));
    translateMatrix3 = glm::translate(identityMatrix, glm::vec3(0.0125f, 0.0f, 0.0125f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, -8.0f + (2*dyy), 0.05f));
    model = translateMatrix * translateMatrix3 * rotateYMatrix * translateMatrix2 * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, ropetex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Container Under Rope
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + dr), glm::vec3(0.0f, 1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.4f, 3.0f + dyy, 4.95f));
    translateMatrix2 = glm::translate(identityMatrix, glm::vec3(-0.575f, 0.0f, -0.25f));
    translateMatrix3 = glm::translate(identityMatrix, glm::vec3(0.575f, 0.0f, 0.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.3f, 1.0f, 1.0f));
    model = translateMatrix * translateMatrix3 * rotateYMatrix * translateMatrix2 * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, cargotex3);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

void sky(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    //Sky
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1000.0f, 50.0, -1000.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4000.0f, 0.1f, 4000.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 1.0f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glBindTexture(GL_TEXTURE_2D, skytex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {                 //Forward
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {                 //Backward
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {                 //Left
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {                 //Right
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    //Start
    //Animation - Crane Movement
    // -------------------------------------
    // --------------------------------------
    // Crane manual Movement
    // ------------------------
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)                  
    {
        dzz += 0.02f;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)                   
    {
        dzz -= 0.02f;
    }
    //Crane Auto Movement
    //--------------------------
    //Auto Movement
    if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
    {
        if (mov == 0)
        {
            mov = 1;
        }
        else if (mov == 1)
        {
            mov = 0;
        }

        /*test11++;
        cout << test11 << endl;*/
    }
    if (mov == 1)
    {
        if (shipdir == 1)                   //Up
        {
            if (dzz >= 12.68f)
            {
                dzz = 12.68f;
                shipdir = 0;
            }
            else if (dzz < 12.7f && dzz > -12.7f)
            {
                dzz += 0.02f;
            }

            //cout << dyy << endl;
        }
        if (shipdir != 1)                   //Up
        {
            if (dzz <= -12.68f)
            {
                dzz = -12.68f;
                shipdir = 1;
            }
            else if (dzz < 12.7f && dzz > -12.7f)
            {
                dzz -= 0.02f;
            }
        }
    }
    //Crane rope Movement
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)                   //Up
    {
        if (dyy >= 2.68f)
        {
            dyy = 2.68f;
            
        }
        else if (dyy < 2.7f && dyy > -2.7f)
        {
            dyy += 0.02f;
        }
        
        //cout << dyy << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)                   
    {
        if (dyy <= -2.68f)
        {
            dyy = -2.68f;
        }
         else if (dyy < 2.7f && dyy > -2.7f)
        {
            dyy -= 0.02f;
        }
        
        //cout << dyy << endl;
    }
    //manual Container rotation
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)                   
    {
        dr += 2.0f;

    }
    //Auto container rotation
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
    {
        if (rot == 0)
        {
            rot = 1;
        }
        else if (rot == 1)
        {
            rot = 0;
        }
        /*test11++;
        cout << test11 << endl;*/
    }
    if (rot == 1)
    {
        
            dr += 2.0f;
        
    }
    if (rot == 0)
    {

        dr += 0.0f;

    }

    //Animation - Ship Movement
    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)                   //Up
    {
        shipz -= 0.02f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)                   //Up
    {
        shipz += 0.02f;
    }
    
        





    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)                   //Down
    {
        /*if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;*/

        camera.ProcessKeyboard(DOWN, deltaTime);


    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)                   //Pitch positive
    {
        /*rotateAngle_X += 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;*/
        camera.ProcessYPR(0.0f, 3.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)                   //Pitch negative
    {
        /*rotateAngle_X += 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;*/
        camera.ProcessYPR(0.0f, -3.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)                   //Yaw positive
    {
        /*rotateAngle_Y += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;*/
        camera.ProcessYPR(3.0f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)                   //Yaw negative
    {
        /*rotateAngle_Y += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;*/
        camera.ProcessYPR(-3.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)                   //Roll positive
    {
        /*rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;*/
        camera.ProcessYPR(0.0f, 0.0f, 0.5f);

    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)                   //Roll negative
    {
        /*rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;*/
        camera.ProcessYPR(0.0f, 0.0f, -0.5f);

    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)                   //Rotate camera around a look at point
    {                                                                   //Left
        /*eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);*/
        camera.RotateAroundLookAt(2.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)                   //Rotate camera around a look at point
    {                                                                   //Right
        /*eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);*/
        camera.RotateAroundLookAt(-2.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)                   //Rotate Fan
    {
        /*eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);*/
        isRotating ^= true;
        cout << isRotating << endl;

        Sleep(100);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)                   //Up
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);

        camera.ProcessKeyboard(UP, deltaTime);

    }


    //***************Lighting***************

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)                   //Lighting On-Off
    {
        lightingOn ^= true;
        cout << lightingOn << endl;
        Sleep(100);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)                   //Ambient On
    {
        ambientOn = 1.0;
        pointLight.turnAmbientOn();
        spotLight.turnAmbientOn();
        directionalLight.turnAmbientOn();
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)                   //Ambient Off
    {
        ambientOn = 0.0;
        pointLight.turnAmbientOff();
        spotLight.turnAmbientOff();
        directionalLight.turnAmbientOff();
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)                   //Diffuse On
    {
        diffuseOn = 1.0;
        pointLight.turnDiffuseOn();
        spotLight.turnDiffuseOn();
        directionalLight.turnDiffuseOn();

    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)                   //Diffuse Off
    {
        diffuseOn = 0.0;
        pointLight.turnDiffuseOff();
        spotLight.turnDiffuseOff();
        directionalLight.turnDiffuseOff();
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)                   //Specular On
    {
        specularOn = 1.0;
        pointLight.turnSpecularOn();
        spotLight.turnSpecularOn();
        directionalLight.turnSpecularOn();
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)                   //Specular Off
    {
        specularOn = 0.0;
        pointLight.turnSpecularOff();
        spotLight.turnSpecularOff();
        directionalLight.turnSpecularOff();
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)                   //Point Light On
    {
        pointLightOn = 1.0;
        pointLight.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)                   //Point Light Off
    {
        pointLightOn = 0.0;
        pointLight.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)                   //Directional Light On
    {
        directionalLightOn = 1.0;
        directionalLight.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)                   //Directional Light Off
    {
        directionalLightOn = 0.0;
        directionalLight.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)                   //Spot Light On
    {
        spotLightOn = 1.0;
        spotLight.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)                   //Spot Light Off
    {
        spotLightOn = 0.0;
        spotLight.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)                   //Dark On-Off
    {
        dark ^= true;
        cout << dark << endl;
        Sleep(100);
    }





    
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        lookAtY += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        lookAtY -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        lookAtZ += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    /*if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        lookAtZ -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 0.0f, 1.0f));
    }*/

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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


//Texture Loading

void load_texture(unsigned int &texture, string image_name, GLenum format)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image_name.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture "<<image_name << endl;
    }
    stbi_image_free(data);
}

//OBJECTS

void drawCube(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), float spec = 1.0f, float shininess = 32.0f)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Y), glm::vec3(0.0f, 1.0f, 0.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
    model = translateMatrix * rotateYMatrix *scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);

    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color * spec);
    ourShader.setFloat("material.shininess", shininess);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
