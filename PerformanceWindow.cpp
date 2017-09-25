#include "PerformanceWindow.h"

PerformanceWindow::PerformanceWindow()
{
	msvector.resize(FPS_MS_SIZE);
	fpsvector.resize(FPS_MS_SIZE);
	active = false;
	window_name = "Performance";
	data_num = 0;
}

PerformanceWindow::~PerformanceWindow()
{
	
}

void PerformanceWindow::DrawWindow()
{
	ImGui::Begin(window_name.c_str(), &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	ImGui::PlotHistogram("Frames", &fpsvector[0], fpsvector.size(), 0, NULL, 0.0f, 90.0f, ImVec2(0, 100));
	ImGui::PlotHistogram("Ms", &msvector[0], msvector.size(), 0, NULL, 0.0f, 30.0f, ImVec2(0, 100));

	ImGui::End();
}

void PerformanceWindow::AddData(float ms, float fps)
{
	

	if (data_num == FPS_MS_SIZE)
	{
		for (int i = 0; i < FPS_MS_SIZE - 1; i++)
		{
			fpsvector[i] = fpsvector[i + 1];
			msvector[i] = msvector[i + 1];
		}
	}
	else
		data_num++;


	fpsvector[data_num - 1] = fps;
	msvector[data_num - 1] = ms;
}
