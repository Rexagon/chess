#pragma once

#include "Log.h"

class Asset
{
public:
	virtual ~Asset() {}
	virtual void* getData() = 0;
};

class AssetFactory
{
public:
	virtual ~AssetFactory() {}
	virtual Asset* load() = 0;
	virtual void clear() = 0;

	Asset* operator()() {
		return load();
	}
};