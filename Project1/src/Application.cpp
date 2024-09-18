#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

#pragma region Shader related methods
//a basic struct to save the vertex and fragment shader source code in only one variable
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

//reads the shader source code file and divide the file into vertex and fragment to return into the structure
static ShaderProgramSource GetShader(const std::string& filePath)
{
    //read the file from the path
    std::ifstream shaderFileStream(filePath);

    //a simplee enum to control where the code will be written
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    //variables necessary for the code
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(shaderFileStream, line))
    {
        //if found a #shader on the line, check if is vertex or fragment to store in type
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            if (type != ShaderType::NONE)
            {
                //transforms the type into int to write in the array
                ss[(int)type] << line << "\n";
            }
        }     
    }

    //returns the structure as strings
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    //create the shader on the GPU and returns its id
    unsigned int id = glCreateShader(type);

    //get the raw string to use in the next method
    const char* src = source.c_str();

    //attach the source code of the shader to the created id 
    GLCall(glShaderSource(id, 1, &src, nullptr));

    //compile the source code of the shader
    GLCall(glCompileShader(id));

    //error handling

    //get the compile status of the shader into the result variable
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    //if the result is false (0), lets see what happened
    if (result == GL_FALSE)
    {
        //get the log into a variable based on the length of the shader  
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        //some c magic to create a raw string dinamically into the stack
        char* message = (char*)alloca(length * sizeof(char));

        //get the info log based on the shader and its length to store it into the message raw string
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        //print the error log into the screen
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:" << std::endl;
        std::cout << message << std::endl;

        //deletes the shader that didn't worked out and return 0
        GLCall(glDeleteShader(id));
        return 0;
    }

    //returns the shader id on the GPU
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //a program is an object to wich shader objects can be attatched
    unsigned int program = glCreateProgram();

    //creationg and compiling of the shaders objects
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //attach the shaders to the program
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    //do program magic
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    //clear intermediate data without deleting the shaders source codes
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

#pragma endregion Shader related methods

int main(void)
{
#pragma region GLFW app config
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //determinates de framerate 
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
#pragma endregion GLFW app config

#pragma region vertices datas
    {
        //creates an array with the vertex positions of the triangle
        float positions[] = {
            -0.5f, -0.5f, //0
             0.5f, -0.5f, //1
             0.5f,  0.5f, //2
            -0.5f,  0.5f  //3
        };

        //use an array of indices so more than one triangle can share the same vertexes 
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
#pragma endregion vertices data

#pragma region Vertex and Index Buffer creation

        VertexArray va;

        //Creates and bind the vertex buffer on its own class
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        //Creates and bind the index buffer on its own class
        IndexBuffer ib(indices, 6);

#pragma endregion Vertex and Index Buffer creation

#pragma region Shader reading and creation
        //gets the shaders codes from another file
        ShaderProgramSource shaderSource = GetShader("res/shaders/Basic.shader");

        //pass the source codes created to the method made
        unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);

        //bind the shader to the program
        GLCall(glUseProgram(shader));

        //uniforms are varibales that can be setted on the CPU side (C++)
        //gets the id of the uniform variable decalred on the fragment Shader
        GLCall(int location = glGetUniformLocation(shader, "u_Color"));

        //error handling that could not find the location desired
        ASSERT(location != -1);

        //specify the value of a uniform variable for the current program object
        GLCall(glUniform4f(location, 0.8, 0.3f, 0.8f, 1.0f));

        //some color animation attributes
        float rate = 0.0f;
        float increment = 0.05f;

#pragma endregion Shader reading and creation

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            GLCall(glUseProgram(shader));

            //some color animation appliance to the uniform
            GLCall(glUniform4f(location, rate, 0.3f, 0.8f, 1.0f));

            //bind the vertex array and the index buffer
            va.Bind();
            ib.Bind();

            //The method that actually draw the buffers into the screen (nested into Error Handling)
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            //some color animation logic
            if (rate > 1.0f)
                increment = -0.05f;
            else if (rate < 0.0f)
                increment = 0.05f;

            rate += increment;

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

        //delete the entire program object
        GLCall(glDeleteProgram(shader));
    }

    glfwTerminate();
    return 0;
}