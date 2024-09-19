#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

        //Creates and bind the vertex array on its own class
        VertexArray vertexArray;

        //Creates and bind the vertex buffer on its own class
        VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        vertexArray.AddBuffer(vertexBuffer, layout);

        //Creates and bind the index buffer on its own class
        IndexBuffer indexBuffer(indices, 6);

#pragma endregion Vertex and Index Buffer creation

#pragma region Shader reading and creation

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        
        vertexArray.Unbind();
        vertexBuffer.Unbind();
        indexBuffer.Unbind();
        shader.Unbind();

        //some color animation attributes
        float red = 0.0f;
        float increment = 0.05f;

#pragma endregion Shader reading and creation

        Renderer renderer;

        /* Render loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            //some color animation appliance to the uniform
            shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);

            //Drawcall
            renderer.Draw(vertexArray, indexBuffer, shader);
            
            //some color animation logic
            if (red > 1.0f)
                increment = -0.05f;
            else if (red < 0.0f)
                increment = 0.05f;

            red += increment;

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }

    glfwTerminate();
    return 0;
}