#include "Window.h"

Window::Window()
{
	active = true;
	window_name = "";
}

Window::~Window()
{
}

void Window::SetActive(bool active)
{
	this->active = active;
}

bool Window::IsActive() const
{
	return active;
}