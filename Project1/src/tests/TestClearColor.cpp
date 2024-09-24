#include "TestClearColor.h"

#include "ErrorHandler.h"
#include "vendor/im_gui/imgui.h"

namespace TestFramework
{
	TestFramework::TestClearColor::TestClearColor()
		: m_ClearColor{ 0.2f, 0.3f, 0.8f, 1.0f }
	{
	}

	TestFramework::TestClearColor::~TestClearColor()
	{
	}

	void TestFramework::TestClearColor::OnUpdate(float deltaTime)
	{
	}

	void TestFramework::TestClearColor::OnRenderer()
	{
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestFramework::TestClearColor::OnImGuiRenderer()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}

}