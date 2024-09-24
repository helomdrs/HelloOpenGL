#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "tests/TestClearColor.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/im_gui/imgui.h";
#include "vendor/im_gui/imgui_impl_glfw.h";
#include "vendor/im_gui/imgui_impl_opengl3.h"

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
    window = glfwCreateWindow(960, 540, "Hello OpenGL!", NULL, NULL);
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
#pragma endregion

#pragma region ImGui setup

    //setup and initialization of imGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontDefault();
    io.Fonts->Build();

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

#pragma endregion

#pragma region vertices datas
    {
        //an array with the vertex coordinates of the triangle and the texture coordinates
        float positions[] = {
            100.f,  100.0f, 0.0f, 0.0f,   //0 - bottom left
            200.0f, 100.0f, 1.0f, 0.0f,   //1
            200.0f, 200.0f, 1.0f, 1.0f,   //2 - uper right
            100.0f, 200.0f, 0.0f, 1.0f    //3
        };

        //use an array of indices so more than one triangle can share the same vertexes 
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
#pragma endregion

#pragma region Vertex and Index Buffer creation

        //Creates and bind the vertex array on its own class
        VertexArray vertexArray;

        //Creates and bind the vertex buffer on its own class
        VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        // the buffer stores 4 floats: 2 for vertices and 2 for texture coodinates
        layout.Push<float>(2); 
        layout.Push<float>(2);
        vertexArray.AddBuffer(vertexBuffer, layout);

        //Creates and bind the index buffer on its own class
        IndexBuffer indexBuffer(indices, 6);

#pragma endregion

#pragma region Matrix creations

        //creates a projection matrix based on an orthographic matrix on a 4:3 aspect ratio
        glm::mat4 projectionMatrix = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

        //a view matrix - the position of the camera and move it to the right (moving the things to the left)
        glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
#pragma endregion   

#pragma region Shader reading and creation

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        
        //some color animation attributes
        float red = 0.0f;
        float increment = 0.05f;

#pragma endregion

#pragma region Texture creationg and binding

        //enable the blend and define how openGL will blend alpha pixels
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        //creates and bind the texture
        Texture texture("res/textures/tiredakali.png");
        texture.Bind();

        //set an uniform on the shader for the texture on the same slot we binded it
        shader.SetUniform1i("u_Texture", 0);
#pragma endregion

#pragma region Unbiding all objects so far

        //unbind everything
        vertexArray.Unbind();
        vertexBuffer.Unbind();
        indexBuffer.Unbind();
        shader.Unbind();

#pragma endregion

#pragma region Test Framework setup

        //initial setup
        TestFramework::Test* currentTest = nullptr;
        TestFramework::TestMenu* testMenu = new TestFramework::TestMenu(currentTest);
        currentTest = testMenu;

        //creates the tests
        testMenu->RegisterTest<TestFramework::TestClearColor>("Clear Color");

#pragma endregion

        Renderer renderer;
        glm::vec3 translation(200, 200, 0);

        /* Render loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
#pragma region Application setup
            /* Render here */
            renderer.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
#pragma endregion

#pragma region Test framework application on screen

            //setup the test framework on screen
            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRenderer();
                ImGui::Begin("Test");
                
                //if the user is not on the tests page and hit the back button
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }

                currentTest->OnImGuiRenderer();
                ImGui::End();
            }
#pragma endregion

#pragma region Model Matrix application

            //apply the model matrix
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 modelViewProjMatrix = projectionMatrix * viewMatrix * modelMatrix;

            //bind (select) the shader so we can send the uniforms to it
            shader.Bind();

            //sends the projection matrix to the vertex shader so it can be used when applying the positions of the vertices
            shader.SetUniformMat4f("u_MVPmatrix", modelViewProjMatrix);
#pragma endregion

            //Drawcall
            renderer.Draw(vertexArray, indexBuffer, shader);

#pragma region ImGui window
            {
                ImGui::Begin("Hello, OpenGL!"); 

                //ImGui will keep displaying the avarege fps
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

                //Use the ImGUI slider to translate the image through the screen
                ImGui::Text("Use the slider to translate the image on the screen");
                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);

                ImGui::End();
            }
#pragma endregion

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
#pragma region Cleanup everything

        //clear the tests
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
#pragma endregion

    return 0;
}