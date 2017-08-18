#pragma once

#include "Layout.h"

class VerticalLayout : public Layout
{
public:
	VerticalLayout() : Layout() {}
	VerticalLayout(std::shared_ptr<Widget> widget) : Layout(widget) {}
private:
	void update() override;
};