#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>

#include <iostream>
#include <vector>
#include <math.h>

// structure to hold the info necessary to render an object
struct SceneObject {
    unsigned int VAO;           // vertex array object handle
    unsigned int vertexCount;   // number of vertices in the object
    float r, g, b;              // for object color
    float x, y;                 // for position offset
};

void draw(SceneObject &sceneObject);
void createArrayBuffer(const std::vector<float> &array, unsigned int &VBO);
void setupShape(const unsigned int shaderProgram,unsigned int &VAO, unsigned int &indexCount, unsigned int pointCount);
// declaration of the function you will implement in voronoi 1.1
SceneObject instantiateCone(float r, float g, float b, float offsetX, float offsetY);
// mouse, keyboard and screen reshape glfw callbacks
void button_input_callback(GLFWwindow* window, int button, int action, int mods);
void key_input_callback(GLFWwindow* window, int button, int other,int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

// global variables we will use to store our objects, shaders, and active shader
std::vector<SceneObject> sceneObjects;
std::vector<Shader> shaderPrograms;
Shader* activeShader;


int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment - Voronoi Diagram", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // setup frame buffer size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // setup input callbacks
    glfwSetMouseButtonCallback(window, button_input_callback); // NEW!
    glfwSetKeyCallback(window, key_input_callback); // NEW!

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // NEW!
    // build and compile the shader programs
    shaderPrograms.push_back(Shader("shaders/shader.vert", "shaders/color.frag"));
    shaderPrograms.push_back(Shader("shaders/shader.vert", "shaders/distance.frag"));
    shaderPrograms.push_back(Shader("shaders/shader.vert", "shaders/distance_color.frag"));
    activeShader = &shaderPrograms[0];

    // NEW!
    // set up the z-buffer
    glDepthRange(1,-1); // make the NDC a right handed coordinate system, with the camera pointing towards -z
    glEnable(GL_DEPTH_TEST); // turn on z-buffer depth test
    glDepthFunc(GL_LESS); // draws fragments that are closer to the screen in NDC

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // notice that now we are clearing two buffers, the color and the z-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render the cones
        glUseProgram(activeShader->ID);

        // TODO voronoi 1.3
        std::cout << sceneObjects.size() << "\n";
        for(int i = 0; i < sceneObjects.size(); i++){
            draw(sceneObjects[i]);
        }
        // Iterate through the scene objects, for each object:
        // - bind the VAO; set the uniform variables; and draw.
        // CODE HERE


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void createArrayBuffer(const std::vector<float> &array, unsigned int &VBO){
    // create the VBO on OpenGL and get a handle to it
    glGenBuffers(1, &VBO);
    // bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the content of the VBO (type, size, pointer to start, and how it is used)
    glBufferData(GL_ARRAY_BUFFER, array.size() * sizeof(GLfloat), &array[0], GL_STATIC_DRAW);
}


// creates a cone triangle mesh, uploads it to openGL and returns the VAO associated to the mesh
SceneObject instantiateCone(float r, float g, float b, float offsetX, float offsetY){
    // TODO voronoi 1.1
    // (exercises 1.7 and 1.8 can help you with implementing this function)
    unsigned int VAO, vertexCount;
    // create a vertex array object (VAO) on OpenGL and save a handle to it
    glGenVertexArrays(1, &VAO);

    // bind vertex array object
    glBindVertexArray(VAO);

    setupShape(activeShader->ID, VAO, vertexCount, 360);

    // Create an instance of a SceneObject,
    SceneObject sceneObject = {
        VAO,
        vertexCount,
        r,
        g,
        b,
        offsetX,
        offsetY
    };

    return sceneObject;

    // you will need to store offsetX, offsetY, r, g and b in the object.
    // CODE HERE
    // Build the geometry into an std::vector<float> or float array.
    // CODE HERE
    // Store the number of vertices in the mesh in the scene object.
    // CODE HERE
    // Declare and generate a VAO and VBO (and an EBO if you decide the work with indices).
    // CODE HERE
    // Bind and set the VAO and VBO (and optionally a EBO) in the correct order.
    // CODE HERE
    // Set the position attribute pointers in the shader.
    // CODE HERE
    // Store the VAO handle in the scene object.
    // CODE HERE

    // 'return' the scene object for the cone instance you just created.
    return sceneObject;
}

void draw(SceneObject &sceneObject){
    // set active shader program
    glUseProgram(activeShader->ID);
    // bind vertex array object
    glBindVertexArray(sceneObject.VAO);
    int offsetLocation = glGetUniformLocation(activeShader->ID, "offset");
    glUniform2f(offsetLocation, sceneObject.x, sceneObject.y);
    int fragColorLocation = glGetUniformLocation(activeShader->ID, "ourColor");
    glUniform3f(fragColorLocation, sceneObject.r, sceneObject.g, sceneObject.b);
    // draw geometry
    glDrawElements(GL_TRIANGLES,sceneObject.vertexCount, GL_UNSIGNED_INT, 0);
}

void setupShape(const unsigned int shaderProgram,unsigned int &VAO, unsigned int &indexCount, unsigned int pointCount){

    unsigned int posVBO, EBO;
    auto indices = std::vector<int>();
    auto buffer = std::vector<float>();

    buffer.push_back(0.0f);
    buffer.push_back(0.0f);
    buffer.push_back(1.0f);

    auto iteration = 360 / pointCount;

    for (int i = 1; i <= pointCount+1; i++) {
        buffer.push_back(cos((i * iteration) * 3.14159265358979323846f / 180.0f) * 3);
        buffer.push_back(sin((i * iteration) * 3.14159265358979323846f / 180.0f) * 3);
        buffer.push_back(0.0f);

        indices.push_back(i);
        indices.push_back(i-1);
        indices.push_back(0);
    }

    createArrayBuffer(buffer, posVBO);

    glGenBuffers(1, &EBO);
    // tell how many vertices to draw
    indexCount = indices.size();

    // create a vertex array object (VAO) on OpenGL and save a handle to it
    glGenVertexArrays(1, &VAO);

    // bind vertex array object
    glBindVertexArray(VAO);

    // set vertex shader attribute "aPos"
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);

    int posSize = 3;
    int posAttributeLocation = glGetAttribLocation(shaderProgram, "aPos");

    glEnableVertexAttribArray(posAttributeLocation);
    glVertexAttribPointer(posAttributeLocation, posSize, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GL_UNSIGNED_INT), &indices[0], GL_STATIC_DRAW);
}


// glfw: called whenever a mouse button is pressed
void button_input_callback(GLFWwindow* window, int button, int action, int mods){
    // TODO voronoi 1.2
    // (exercises 1.9 and 2.2 can help you with implementing this function)

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << xpos << " " << ypos << "\n";

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float normalizedX, normalizedY;
        normalizedX = ((xpos - (width / 2))/width) * 2;
        normalizedY = -((ypos - (height / 2))/height) * 2;
        std::cout << normalizedX << " " << normalizedY << "\n";

        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        sceneObjects.push_back(instantiateCone(r,g,b,normalizedX,normalizedY));
        std::cout << "Did it \n";
    }

    // Test button press, see documentation at:
    //     https://www.glfw.org/docs/latest/input_guide.html#input_mouse_button
    // CODE HERE
    // If a left mouse button press was detected, call instantiateCone:
    // - Push the return value to the back of the global 'vector<SceneObject> sceneObjects'.
    // - The click position should be transformed from screen coordinates to normalized device coordinates,
    //   to obtain the offset values that describe the position of the object in the screen plane.
    // - A random value in the range [0, 1] should be used for the r, g and b variables.
    // CODE HERE
}

// glfw: called whenever a keyboard key is pressed
void key_input_callback(GLFWwindow* window, int button, int other,int action, int mods){
    // TODO voronoi 1.4

    if( button == GLFW_KEY_1){
        activeShader = &shaderPrograms[0];
    } else if ( button == GLFW_KEY_2){
        activeShader = &shaderPrograms[1];
    } else if ( button == GLFW_KEY_3){
        activeShader = &shaderPrograms[2];
    }
    // Set the activeShader variable by detecting when the keys 1, 2 and 3 were pressed;
    // see documentation at https://www.glfw.org/docs/latest/input_guide.html#input_keyboard
    // Key 1 sets the activeShader to &shaderPrograms[0];
    //   and so on.
    // CODE HERE
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}