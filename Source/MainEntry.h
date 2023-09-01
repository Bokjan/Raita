#pragma once

#include <memory>

int MainEntry(int argc, char* argv[]);

class MainLogicAdapater
{
public:
	virtual bool ShouldStopProgram() = 0;
	virtual void FrameStartHook();
	virtual void FrameFinishHook();
	virtual void BackendSetup() = 0;
	virtual void BackendCleanUp() = 0;
	virtual void BackendFrameStart() = 0;
	virtual void BackendRender() = 0;
};

std::unique_ptr<MainLogicAdapater> CreateMainLogicAdapter();
