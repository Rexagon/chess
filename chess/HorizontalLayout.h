#pragma once

#include "Layout.h"

class HorizontalLayout : public Layout
{
public:
	HorizontalLayout() : Layout() {}
	HorizontalLayout(widget_ptr widget) : Layout(widget) {}
protected:
	void update() override;
};