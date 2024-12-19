#pragma once

#pragma region OpenGL related includes

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/im_gui/imgui.h";
#include "vendor/im_gui/imgui_impl_glfw.h";
#include "vendor/im_gui/imgui_impl_opengl3.h"

#pragma endregion

#pragma region Tests includes

#include "Renderer.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestAnimatedShader.h"
#include "tests/TestDvdLogo.h"
#include "tests/SpaceInvaders.h"

#pragma endregion

#pragma region Space Invaders related includes

#include "tests/SpaceInvaders/Utils/InputManager.h"
#include "tests/SpaceInvaders/Utils/TimeUtils.h"

#pragma endregion

std::shared_ptr<InputManager> m_InputManager = std::make_shared<InputManager>();