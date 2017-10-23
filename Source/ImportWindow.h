#pragma once

#include "Window.h"

class ImportWindow :
	public Window
{
public:

	ImportWindow();
	~ImportWindow();

	void DrawWindow();

	void SetTotalProgress(int number);
	void SetCurrentProgress(int number);
	void SetProgressFileName(std::string name);
	//current_progress++
	void IncreaseProgress();
	void IncreaseTotalProgressByAmount(int amount);

private:
	int total_progress;
	int current_progress;
	std::string progress_file_name;
};

