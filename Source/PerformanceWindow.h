#pragma once
#include "Window.h"
#include <vector>
#include <map>
#define FPS_MS_SIZE 100

class PerformanceWindow : public Window
{
public:
	PerformanceWindow();
	virtual ~PerformanceWindow();

	void DrawWindow();

	void AddData(float ms, float fps);
	void AddModuleData(std::string name, float ms);
private:
	std::vector<float> msvector;
	std::vector<float> fpsvector;

	std::map<std::string, std::vector<float>> module_data;
	int data_num;
};