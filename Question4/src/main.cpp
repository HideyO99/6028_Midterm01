#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>



#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3        (x,y,z)
#include <glm/vec4.hpp> // glm::vec4        (x,y,z,w)
#include <glm/mat4x4.hpp> // glm::mat4
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader/cShaderManager.h"
#include "VAOManager/cVAOManager.h"
#include "MeshObj/cMeshObj.h"
#include "Light/cLightManager.h"
#include "GUI/cGUI.h"
#include <chrono>
#include "Ship.h"

#define MODEL_LIST_XML          "asset/model.xml"
#define VERTEX_SHADER_FILE      "src/shader/vertexShader.glsl"
#define FRAGMENT_SHADER_FILE    "src/shader/fragmentShader.glsl"

glm::vec3 g_cameraEye = glm::vec3(0.0, 5.0, 0.0f);
glm::vec3 g_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
bool bIsWalkAround = false;
bool firstMouse = true;
float yaw = -90.0f;	
float pitch = 0.0f;
float lastX = 1280.0f / 2.0;
float lastY = 800.0 / 2.0;
float fov = 45.0f;
std::chrono::milliseconds ms(1000);

cLightManager* g_pTheLightManager = NULL;
static GLFWwindow* window = nullptr;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void updateInstanceObj(cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::mat4x4 matView, glm::mat4x4 matProjection);
void light0Setup();
void light1Setup();
void light2Setup();
void light3Setup();
void light4Setup();

int main(void)
{

    bool result;
    std::cout << "starting..\n";
    //GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    //GLint mvp_location, vpos_location, vcol_location;
    GLuint shaderID = 0;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1280, 800, "6028 Graphic Midterm Q#1", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    std::cout << "created window" << std::endl;

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    //initialize imgui
    cGUI* gui_ = new cGUI(&g_cameraEye,&g_cameraTarget);
    result = gui_->ImGUI_init(window);
    if (!result)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    //create shader program
    cShaderManager* pShaderManager = new cShaderManager();
    cShaderManager::cShader vertexShader;
    cShaderManager::cShader fragmentShader;

    vertexShader.fileName = VERTEX_SHADER_FILE;
    fragmentShader.fileName = FRAGMENT_SHADER_FILE;
    result = pShaderManager->createShaderProgram("Shader01", vertexShader, fragmentShader);
    if (!result)
    {
        std::cout << "error: Shader compilation fail" << std::endl;

        glfwDestroyWindow(window);

        delete pShaderManager;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "shader compilation OK" << std::endl;
    }

    pShaderManager->useShaderPRogram("Shader01");
    shaderID = pShaderManager->getIDfromName("Shader01");
    //glUseProgram(shaderID);

    //todo lighting
    ::g_pTheLightManager = new cLightManager();

    light0Setup();
    light1Setup();
    light2Setup();
    light3Setup();
    light4Setup();
    
    
    ::g_pTheLightManager->loadLightUniformLocation(shaderID);
    for (size_t i = 0; i < MAX_LIGHT_SOURCE; i++)
    {
        gui_->pLight[i] = ::g_pTheLightManager->plight[i];
    }
    //load model
    cVAOManager* pVAOManager = new cVAOManager();

    result = pVAOManager->loadModelList(MODEL_LIST_XML, shaderID);
    if (!result)
    {
        std::cout << "cannot load model list" << std::endl;

        glfwDestroyWindow(window);

        delete pVAOManager;
        delete pShaderManager;

        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    ::g_cameraEye = pVAOManager->cameraEyeFromXML;


    gui_->pVecInstanceMeshObj = &pVAOManager->pVecInstanceMeshObj;
    
    PirateShip* pirate_ship_ = new PirateShip();
    pirate_ship_->lantern = ::g_pTheLightManager;
    pirate_ship_->pShipOBJ = pVAOManager->findMeshObjAddr("pirateShip");
    //setup object
    //result = pVAOManager->setInstanceObjVisible("terrain01", true);
    result = pVAOManager->setInstanceObjRGB("terrain01", glm::vec4(0.1f,0.1f,0.1f,1.f));
    result = pVAOManager->setInstanceObjSpecularPower("terrain01", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));

    //result = pVAOManager->setInstanceObjPosition("smallFactory01", glm::vec4(15.13f, 80.29f, -29.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("smallFactory01", glm::vec4(1.f, 0.f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjScale("smallFactory01", 1.f);
    result = pVAOManager->setInstanceObjSpecularPower("smallFactory01", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));

    //result = pVAOManager->setInstanceObjPosition("lighthouse01", glm::vec4(24.54f, 77.68f, -33.15f, 1.f));
    result = pVAOManager->setInstanceObjRGB("lighthouse01", glm::vec4(1.f, 0.0f, 1.f, 1.f));
    result = pVAOManager->setInstanceObjSpecularPower("lighthouse01", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));

    result = pVAOManager->setInstanceObjRGB("pirateShip", glm::vec4(0.2f, 1.f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjSpecularPower("pirateShip", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));

    result = pVAOManager->setInstanceObjRGB("oldhouse01", glm::vec4(1.0f, 0.0f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("oldhouse02", glm::vec4(0.05f, 0.95f, 1.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("oldhouse03", glm::vec4(0.5f, 1.0f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("oldhouse04", glm::vec4(0.55f, 1.0f, 1.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("oldhouse05", glm::vec4(1.f, 1.f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("oldhouse06", glm::vec4(0.0f, 1.0f, 0.3f, 1.f));
    result = pVAOManager->setInstanceObjSpecularPower("oldhouse01", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("oldhouse02", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("oldhouse03", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("oldhouse04", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("oldhouse05", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("oldhouse06", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));

    result = pVAOManager->setInstanceObjScale("woodhouse01", 0.25f);
    result = pVAOManager->setInstanceObjScale("woodhouse02", 0.25f);
    result = pVAOManager->setInstanceObjScale("woodhouse03", 0.25f);
    result = pVAOManager->setInstanceObjScale("woodhouse04", 0.25f);
    result = pVAOManager->setInstanceObjScale("woodhouse05", 0.25f);
    result = pVAOManager->setInstanceObjScale("woodhouse06", 0.25f);
    result = pVAOManager->setInstanceObjRGB("woodhouse01", glm::vec4(1.f, 0.f, 1.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("woodhouse02", glm::vec4(1.f, 1.f, 1.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("woodhouse03", glm::vec4(0.f, 0.f, 1.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("woodhouse04", glm::vec4(0.f, 1.f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("woodhouse05", glm::vec4(1.f, 1.f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("woodhouse06", glm::vec4(0.3f, 0.7f, 1.f, 1.f));
    result = pVAOManager->setInstanceObjSpecularPower("woodhouse01", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("woodhouse02", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("woodhouse03", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("woodhouse04", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("woodhouse05", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("woodhouse06", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));

    result = pVAOManager->setInstanceObjScale("palm01", 0.003f);
    result = pVAOManager->setInstanceObjScale("palm02", 0.003f);
    result = pVAOManager->setInstanceObjScale("palm03", 0.003f);
    result = pVAOManager->setInstanceObjScale("palm04", 0.003f);
    result = pVAOManager->setInstanceObjScale("palm05", 0.003f);
    result = pVAOManager->setInstanceObjRGB("palm01", glm::vec4(0.f, 1.f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("palm02", glm::vec4(0.f, 1.f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("palm03", glm::vec4(0.f, 1.f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("palm04", glm::vec4(0.f, 1.f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjRGB("palm05", glm::vec4(0.f, 1.f, 0.f, 1.f));
    result = pVAOManager->setInstanceObjSpecularPower("palm01", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("palm02", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("palm03", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("palm04", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("palm05", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));

    result = pVAOManager->setInstanceObjScale("tree1_01", 0.03f);
    result = pVAOManager->setInstanceObjScale("tree1_02", 0.1f);
    result = pVAOManager->setInstanceObjScale("tree1_03", 0.1f);
    result = pVAOManager->setInstanceObjScale("tree1_04", 0.1f);
    result = pVAOManager->setInstanceObjScale("tree1_05", 0.1f);
    result = pVAOManager->setInstanceObjRGB("tree1_01", glm::vec4(0.f, 0.61f, 0.35f, 1.f));
    result = pVAOManager->setInstanceObjRGB("tree1_02", glm::vec4(0.f, 0.61f, 0.35f, 1.f));
    result = pVAOManager->setInstanceObjRGB("tree1_03", glm::vec4(0.f, 0.61f, 0.35f, 1.f));
    result = pVAOManager->setInstanceObjRGB("tree1_04", glm::vec4(0.f, 0.61f, 0.35f, 1.f));
    result = pVAOManager->setInstanceObjRGB("tree1_05", glm::vec4(0.f, 0.61f, 0.35f, 1.f));
    result = pVAOManager->setInstanceObjSpecularPower("tree1_01", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("tree1_02", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("tree1_03", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("tree1_04", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("tree1_05", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));

    result = pVAOManager->setInstanceObjScale("tree3_01", 15.f);
    result = pVAOManager->setInstanceObjScale("tree3_02", 15.f);
    result = pVAOManager->setInstanceObjScale("tree3_03", 15.f);
    result = pVAOManager->setInstanceObjScale("tree3_04", 15.f);
    result = pVAOManager->setInstanceObjRGB("tree3_01", glm::vec4(0.57f, 1.f, 0.56f, 1.f));
    result = pVAOManager->setInstanceObjRGB("tree3_02", glm::vec4(0.57f, 1.f, 0.56f, 1.f));
    result = pVAOManager->setInstanceObjRGB("tree3_03", glm::vec4(0.57f, 1.f, 0.56f, 1.f));
    result = pVAOManager->setInstanceObjRGB("tree3_04", glm::vec4(0.57f, 1.f, 0.56f, 1.f));
    result = pVAOManager->setInstanceObjSpecularPower("tree3_01", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("tree3_02", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("tree3_03", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("tree3_04", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));

    result = pVAOManager->setInstanceObjScale("tree_01", 0.5f);
    result = pVAOManager->setInstanceObjScale("tree_02", 0.5f);
    result = pVAOManager->setInstanceObjRGB("tree_01", glm::vec4(1.f, 0.16f, 0.32f, 1.f));
    result = pVAOManager->setInstanceObjRGB("tree_02", glm::vec4(1.f, 0.16f, 0.32f, 1.f));
    result = pVAOManager->setInstanceObjSpecularPower("tree_01", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    result = pVAOManager->setInstanceObjSpecularPower("tree_02", glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f));
    cMeshObj* pFactoryObject = pVAOManager->findMeshObjAddr("smallFactory01");
    g_cameraTarget = pFactoryObject->position;
    while (!glfwWindowShouldClose(window))
    {
        ::g_pTheLightManager->setLightToShader(shaderID);

        float ratio;
        int width, height;
        //glm::mat4x4 matModel;
        glm::mat4x4 matProjection;
        glm::mat4x4 matView;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (ms.count() == 1000)
        {
            pirate_ship_->updatePos();
            //g_cameraTarget = pirate_ship_->pShipOBJ->position;
            ::g_cameraEye = glm::vec3(pirate_ship_->pShipOBJ->position.x-20, pirate_ship_->pShipOBJ->position.y, pirate_ship_->pShipOBJ->position.z + 30);
        }
       
        glm::vec3 cameraDirection = glm::normalize(g_cameraEye - g_cameraTarget);
        glm::vec3 cameraRight = glm::normalize(glm::cross(g_upVector, cameraDirection));
        if (!bIsWalkAround)
        {

            matView = glm::lookAt(::g_cameraEye, ::g_cameraTarget, ::g_upVector);
        }
        else
        {
            matView = glm::lookAt(::g_cameraEye, ::g_cameraEye+::g_cameraFront, ::g_upVector);
        }
        GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");

        glUniform4f(eyeLocation_UniLoc, ::g_cameraEye.x, ::g_cameraEye.y, ::g_cameraEye.z, 1.0f);

        matProjection = glm::perspective(glm::radians(fov), ratio, 0.1f, 10000.0f);

        updateInstanceObj(pShaderManager, pVAOManager, matView, matProjection);

        gui_->ImGUICreateFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    gui_->ImGUI_shutdown();

    glfwDestroyWindow(window);

    delete pVAOManager;
    delete pShaderManager;

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void updateInstanceObj(cShaderManager* pShaderManager, cVAOManager* pVAOManager, glm::mat4x4 matView, glm::mat4x4 matProjection)
{
    glm::mat4x4 matModel;

    for (std::map<std::string, cMeshObj* >::iterator itCurrentMesh = pVAOManager->mapInstanceNametoMeshObj.begin();
        itCurrentMesh != pVAOManager->mapInstanceNametoMeshObj.end();
        itCurrentMesh++)
    {
        cMeshObj* pCurrentMeshObject = (itCurrentMesh->second);        // * is the iterator access thing

        if (!pCurrentMeshObject->isVisible)
        {
            // Skip the rest of the loop
            continue;
        }

        // Don't draw any "back facing" triangles
        glCullFace(GL_BACK);

        // Turn on depth buffer test at draw time
        glEnable(GL_DEPTH_TEST);
        

        if (pCurrentMeshObject->instanceName == "sphere01")
        {
            pCurrentMeshObject->position = ::g_pTheLightManager->plight[8]->position;
        }
        //if (pCurrentMeshObject->instanceName == "sphere02")
        //{
        //    pCurrentMeshObject->position = ::g_pTheLightManager->plight[7]->position;
        //}
        ////if (pCurrentMeshObject->instanceName == "sphere03")
        ////{
        ////    pCurrentMeshObject->position = ::g_pTheLightManager->plight[2]->position;
        ////}
        ////if (pCurrentMeshObject->instanceName == "sphere04")
        ////{
        ////    pCurrentMeshObject->position = ::g_pTheLightManager->plight[3]->position;
        ////}
        ////if (pCurrentMeshObject->instanceName == "sphere05")
        ////{
        ////    pCurrentMeshObject->position = ::g_pTheLightManager->plight[4]->position;
        ////}

        matModel = glm::mat4x4(1.0f);  // identity matrix

        // Move the object 
        glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f), pCurrentMeshObject->position);
        
        
        //rotate
        glm::mat4 matRoationZ = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 matRoationY = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 matRoationX = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

        // Scale the object
        float uniformScale = pCurrentMeshObject->scale;
        glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(uniformScale, uniformScale, uniformScale));

        matModel = matModel * matTranslation;

        matModel = matModel * matRoationX;
        matModel = matModel * matRoationY;
        matModel = matModel * matRoationZ;

        matModel = matModel * matScale;

        pShaderManager->setShaderUniformM4fv("mModel", matModel);
        pShaderManager->setShaderUniformM4fv("mView", matView);
        pShaderManager->setShaderUniformM4fv("mProjection", matProjection);

        glm::mat4 mModelInverseTransform = glm::inverse(glm::transpose(matModel));
        pShaderManager->setShaderUniformM4fv("mModelInverseTranspose", mModelInverseTransform);

        // Wireframe
        if (pCurrentMeshObject->isWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        pShaderManager->setShaderUniform4f("RGBA_Color",
            pCurrentMeshObject->color_RGBA.r,
            pCurrentMeshObject->color_RGBA.g,
            pCurrentMeshObject->color_RGBA.b,
            pCurrentMeshObject->color_RGBA.w);

        pShaderManager->setShaderUniform4f("specularColour",
            pCurrentMeshObject->specular_colour_and_power.r,
            pCurrentMeshObject->specular_colour_and_power.g,
            pCurrentMeshObject->specular_colour_and_power.b,
            pCurrentMeshObject->specular_colour_and_power.w);

        //uniform bool bDoNotLight;	
        if (pCurrentMeshObject->bDoNotLight)
        {
            pShaderManager->setShaderUniform1f("bDoNotLight", (GLfloat)GL_TRUE);
        }
        else
        {
            pShaderManager->setShaderUniform1f("bDoNotLight", (GLfloat)GL_FALSE);
        }

        cModelDrawInfo drawingInformation;
        if (pVAOManager->FindDrawInfo(pCurrentMeshObject->meshName, drawingInformation))
        {
            glBindVertexArray(drawingInformation.VAO_ID);

            glDrawElements(GL_TRIANGLES, drawingInformation.numberOfIndices, GL_UNSIGNED_INT, (void*)0);

            glBindVertexArray(0);

        }
        else
        {
            // Didn't find that model
            std::cout << "Error: didn't find model to draw." << std::endl;

        }
    }
}

void light0Setup()
{
    //point light in small factory
    ::g_pTheLightManager->plight[0]->type = cLight::LightType::LIGHT_SPOT;
    ::g_pTheLightManager->plight[0]->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLightManager->plight[0]->position = glm::vec4(15.23f, 80.7f, -29.1f, 1.0f);
    ::g_pTheLightManager->plight[0]->attenuation = glm::vec4(1.13f, 0.204f, 0.003f, 1.0f);
    ::g_pTheLightManager->plight[0]->turnON = 1;
    ::g_pTheLightManager->plight[0]->direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLightManager->plight[0]->angle.x = 10.0f;     // Degrees
    ::g_pTheLightManager->plight[0]->angle.y = 20.0f;
}

void light1Setup()
{
    //point light in lighthouse
    ::g_pTheLightManager->plight[1]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[1]->diffuse = glm::vec4(2.f, 2.f, 2.f, 1.0f);
    ::g_pTheLightManager->plight[1]->position = glm::vec4(24.7f, 80.8f, -33.0f, 1.0f);
    ::g_pTheLightManager->plight[1]->attenuation = glm::vec4(0.06f, 0.01f, 0.001f, 1.0f);
    ::g_pTheLightManager->plight[1]->turnON = 1;

}
void light2Setup()
{
    //point light in house - old house 1
    ::g_pTheLightManager->plight[2]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[2]->diffuse = glm::vec4(3.9f, 1.7f, 0.0f, 1.0f);
    ::g_pTheLightManager->plight[2]->position = glm::vec4(-118.83f, 1.863f, 12.93f, 1.0f);
    ::g_pTheLightManager->plight[2]->attenuation = glm::vec4(0.1f, 0.01f, 0.04f, 1.f);
    ::g_pTheLightManager->plight[2]->turnON = 1;
    //point light in house - old house 4
    ::g_pTheLightManager->plight[3]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[3]->diffuse = glm::vec4(3.9f, 1.7f, 0.0f, 1.0f);
    ::g_pTheLightManager->plight[3]->position = glm::vec4(-110.05f, 6.85f, -22.7f, 1.0f);
    ::g_pTheLightManager->plight[3]->attenuation = glm::vec4(0.1f, 0.01f, 0.04f, 1.f);
    ::g_pTheLightManager->plight[3]->turnON = 1;
    //point light in house - old house 6
    ::g_pTheLightManager->plight[4]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[4]->diffuse = glm::vec4(3.9f, 1.7f, 0.0f, 1.0f);
    ::g_pTheLightManager->plight[4]->position = glm::vec4(-118.49f, 0.9f, 0.5f, 1.0f);
    ::g_pTheLightManager->plight[4]->attenuation = glm::vec4(0.1f, 0.01f, 0.04f, 1.f);
    ::g_pTheLightManager->plight[4]->turnON = 1;
    //point light in house - woodhouse04
    ::g_pTheLightManager->plight[5]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[5]->diffuse = glm::vec4(3.9f, 1.7f, 0.0f, 1.0f);
    ::g_pTheLightManager->plight[5]->position = glm::vec4(-119.f, 0.85f, 17.19f, 1.0f);
    ::g_pTheLightManager->plight[5]->attenuation = glm::vec4(0.1f, 0.01f, 0.04f, 1.f);
    ::g_pTheLightManager->plight[5]->turnON = 1;
}

void light3Setup()
{
    //point light on ship
    ::g_pTheLightManager->plight[6]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[6]->diffuse = glm::vec4(3.9f, 1.7f, 0.0f, 1.0f);
    ::g_pTheLightManager->plight[6]->position = glm::vec4(34.0f, 120.0f, 45.0f, 1.0f);
    ::g_pTheLightManager->plight[6]->attenuation = glm::vec4(0.01f, 0.1f, 0.006f, 1.f);
    ::g_pTheLightManager->plight[6]->turnON = 1;

    ::g_pTheLightManager->plight[7]->type = cLight::LightType::LIGHT_POINT;
    ::g_pTheLightManager->plight[7]->diffuse = glm::vec4(3.9f, 1.7f, 0.0f, 1.0f);
    ::g_pTheLightManager->plight[7]->position = glm::vec4(34.0f, 120.0f, 45.0f, 1.0f);
    ::g_pTheLightManager->plight[7]->attenuation = glm::vec4(0.01f, 0.1f, 0.006f, 1.f);
    ::g_pTheLightManager->plight[7]->turnON = 1;
}

void light4Setup()
{
    //spot light at ship
    ::g_pTheLightManager->plight[8]->type = cLight::LightType::LIGHT_SPOT;
    ::g_pTheLightManager->plight[8]->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLightManager->plight[8]->position = glm::vec4(15.23f, 80.7f, -29.1f, 1.0f);
    ::g_pTheLightManager->plight[8]->attenuation = glm::vec4(0.03f, 0.01f, 0.003f, 1.0f);
    ::g_pTheLightManager->plight[8]->turnON = 1;
    ::g_pTheLightManager->plight[8]->direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLightManager->plight[8]->angle.x = 10.0f;     // Degrees
    ::g_pTheLightManager->plight[8]->angle.y = 20.0f;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    //move camera
    // AWSD AD-Left, Right
    //      WS-Forward, Back
    const float CAMERA_MOVE_SPEED = 1.f;
    if (key == GLFW_KEY_A)
    {
        ::g_cameraEye.x -= CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_D)
    {
        ::g_cameraEye.x += CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_W)
    {
        ::g_cameraEye.z -= CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_S)
    {
        ::g_cameraEye.z += CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_Q)
    {
        ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_E)
    {
        ::g_cameraEye.y += CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    {
        // toggle walking mode camera 
        bIsWalkAround = !bIsWalkAround;

    }
}

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
    if (bIsWalkAround)
    {


        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; 
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f; 
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        ::g_cameraFront = glm::normalize(front);
    }
    else
    {
        ::g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

