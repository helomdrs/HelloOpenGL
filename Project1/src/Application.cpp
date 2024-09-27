#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestAnimatedShader.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/im_gui/imgui.h";
#include "vendor/im_gui/imgui_impl_glfw.h";
#include "vendor/im_gui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    {
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

        //initial setup
        TestFramework::Test* currentTest = nullptr;
        TestFramework::TestMenu* testMenu = new TestFramework::TestMenu(currentTest);
        currentTest = testMenu;

        //creates the tests
        testMenu->RegisterTest<TestFramework::TestClearColor>("Clear Color");
        testMenu->RegisterTest<TestFramework::TestTexture2D>("2D Texture");
        testMenu->RegisterTest <TestFramework::TestAnimatedShader>("Test Animated Shader");

        Renderer renderer;

        /* Render loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            renderer.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            //setup the test framework on screen
            if (currentTest)
            {
                currentTest->OnUpdate(0.2f);
                currentTest->OnRenderer();

                ImGui::Begin("Hello, OpenGL! Let's do some testing");

                //ImGui will keep displaying the avarege fps
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

                //if the user is not on the tests page and hit the back button
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }

                currentTest->OnImGuiRenderer();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            GLCall(glfwSwapBuffers(window));
            GLCall(glfwPollEvents());
        }

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

    return 0;
}