#pragma once

#include "GUI.h"

class State
{
public:
	virtual ~State() {}

	virtual void init() {}
	virtual void close() {}

	virtual void update(const float dt) {}
	virtual void draw(const float dt) {}

	virtual void pause() {}
	virtual void resume() {}

	virtual void focus_lost() {}
	virtual void focus_gained() {}

	virtual void resized(float width, float height) {}
protected:
	friend class Core;

	GUI m_gui;
};