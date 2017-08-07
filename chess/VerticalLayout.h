#pragma once

#include "Layout.h"

class VerticalLayout : public Layout
{
public:
	VerticalLayout() : Layout() {}
	VerticalLayout(widget_ptr widget) : Layout(widget) {}
private:
	void update() override;
};