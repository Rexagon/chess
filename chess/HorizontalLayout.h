#pragma once

#include "Layout.h"

class HorizontalLayout : public Layout
{
public:
	HorizontalLayout() : Layout() {}
	HorizontalLayout(std::shared_ptr<Widget> widget) : Layout(widget) {}
protected:
	void update() override;
};