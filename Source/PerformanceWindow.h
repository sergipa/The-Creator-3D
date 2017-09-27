#pragma once
#include "Window.h"
#include <vector>

#define FPS_MS_SIZE 100

class PerformanceWindow : public Window
{
public:
	PerformanceWindow();
	~PerformanceWindow();

	void DrawWindow();

	void AddData(float ms, float fps);

private:
	std::vector<float> msvector;
	std::vector<float> fpsvector;

	int data_num;
};