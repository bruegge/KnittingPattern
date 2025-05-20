#include "../h/glad.h"
#include "../h/Window.h"
#include "../h/Logic.h"
#include "../h/GLError.h"
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_impl_glfw.h"
#include "../ImGUI/imgui_impl_opengl3.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <commdlg.h>

static Window* pWindow = nullptr;
static Logic* pLogic = nullptr;

static bool openFileDialog(std::string& fileNameOut)
{
	OPENFILENAMEA ofn = { 0 };
	char Buffer[300];
	std::fill(Buffer, Buffer + 300, '\0');
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	//ofn.hwndOwner = hWnd;
	ofn.lpstrFile = Buffer;
	ofn.nMaxFile = 300;
	ofn.Flags = OFN_EXPLORER;
	ofn.lpstrFilter = NULL;
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	bool result = GetOpenFileNameA(&ofn);
	fileNameOut = Buffer;
	return result;
}

static bool saveFileDialog(std::string& fileNameOut)
{
	OPENFILENAMEA ofn = { 0 };
	char Buffer[300];
	std::fill(Buffer, Buffer + 300, '\0');
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	//ofn.hwndOwner = hWnd;
	ofn.lpstrFile = Buffer;
	ofn.nMaxFile = 300;
	ofn.Flags = OFN_EXPLORER;
	ofn.lpstrFilter = ".bmp";
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	bool result = GetSaveFileNameA(&ofn);
	fileNameOut = Buffer;
	return result;
}

void Init()
{
	pWindow = new Window(1024, 900, "Knitting Pattern");
	
	{ //GUI
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(pWindow->GetWindowID(), true);
		ImGui_ImplOpenGL3_Init("#version 430");
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
}

void LoadContent()
{
	pLogic = new Logic();

}

void GameLoop()
{
	GLError::checkError();
	glClearColor(0.1f, 0.1f, 0.02f, 1.0f);
	GLError::checkError();
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);
	GLError::checkError();
	glViewport(0, 0, pWindow->GetWindowSize().x, pWindow->GetWindowSize().y);
	GLError::checkError();
	bool bClose = false;
	do //render loop
	{
		GLError::checkError();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GLError::checkError();
		pLogic->render();
		GLError::checkError();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bClose = pWindow->ManageInputs();
		GLError::checkError();

		{
			ImGui::Begin("Settings");

			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open"))
					{
						std::string fileName = "";
						if (openFileDialog(fileName))
						{
							pLogic->openImage(fileName);
						}
					}
					if (ImGui::MenuItem("Save", "Ctrl+S"))
					{
						std::string fileName = "";
						if (saveFileDialog(fileName))
						{
							pLogic->saveResult(fileName);
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}						
			GLError::checkError();
			
			//stitchSize
			{
				glm::vec2 stitchSizeVec = pLogic->getStitchSize();
				float stitchSize[2] = { stitchSizeVec.x, stitchSizeVec.y };
				ImGui::SliderFloat2("Stitch size", stitchSize, 0.1, 2.0);
				pLogic->changeStitchSize(stitchSize[0], stitchSize[1]);
			}
			GLError::checkError();

			//numberOfColors
			{
				int numberOfColors = static_cast<int>(pLogic->getNumberOfColors());
				ImGui::SliderInt("Number of colors", &numberOfColors, 2, 100);
				pLogic->changeNumberOfColors(static_cast<unsigned int>(numberOfColors));
			}
			GLError::checkError();

			//numberOfStitchColumns
			{
				int columnStitchCount = static_cast<int>(pLogic->getColumnStitchCount());
				ImGui::SliderInt("Number of stitch columns", &columnStitchCount, 2, 500);
				pLogic->changeColumnStitchCount(static_cast<unsigned int>(columnStitchCount));
			}
			//mirror
			{
				glm::ivec2 mirror = pLogic->getMirror();
				bool vertical = mirror.x == 1 ? true : false;
				bool horizontal = mirror.y == 1 ? true : false;
				ImGui::Checkbox("Mirror vertical", &vertical);
				ImGui::Checkbox("Mirror horizontal", &horizontal);
				mirror.x = vertical ? 1 : 0;
				mirror.y = horizontal ? 1 : 0;
				pLogic->setMirror(mirror);
			}
			GLError::checkError();
			if (ImGui::CollapsingHeader("Grid colors"))
			{
				ImGui::TreePush();
				//gridColorPicker
				{
					glm::vec4 gridColor = pLogic->getGridColor();
					float colors[3] = { gridColor.x, gridColor.y, gridColor.z };
					ImGui::ColorEdit3("Grid color", colors);
					ImGui::SliderFloat("Opacity", &gridColor.a, 0.0f, 1.0f);
					gridColor = glm::vec4(colors[0], colors[1], colors[2], gridColor.a);
					pLogic->setGridColor(gridColor);
				}
				//gridHighlightColorPicker
				{
					glm::vec4 gridColor = pLogic->getGridHighlightColor();
					float colors[3] = { gridColor.x, gridColor.y, gridColor.z };
					ImGui::ColorEdit3("Grid highlight color", colors);
					ImGui::SliderFloat("Highlight opacity", &gridColor.a, 0.0f, 1.0f);
					gridColor = glm::vec4(colors[0], colors[1], colors[2], gridColor.a);
					pLogic->setGridHighlightColor(gridColor);
				}
				ImGui::TreePop();
			}
			{
				//ImGui::ShowDemoWindow();
			}
			ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
			GLError::checkError();
			ImGui::End();
			GLError::checkError();
		}

		GLError::checkError();
		ImGui::Render();
		GLError::checkError();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		GLError::checkError();

		pWindow->SwapBuffers();
		GLError::checkError();
	} while (bClose == false);
}

void FreeContent()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	delete pWindow;
	delete pLogic;
}

void main()
{
	Init();
	LoadContent();
	GameLoop();
	FreeContent();
}