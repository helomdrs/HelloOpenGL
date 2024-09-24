#include "Test.h"

#include "vendor/im_gui/imgui.h"

namespace TestFramework
{
	TestMenu::TestMenu(Test*& currentTestPointer)
		: m_CurrentTest(currentTestPointer)
	{
	}

	void TestMenu::OnImGuiRenderer()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_CurrentTest = test.second();
			}
		}
	}
}


