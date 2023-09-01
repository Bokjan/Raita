#include "MainEntry.h"

#include <chrono>
#include <cstdio>
#include <Windows.h>

#include "imgui.h"

#include "Logic/RaitaGui.h"
#include "Logic/TickDispatcher.h"

using namespace std::chrono_literals;

void MainLogicAdapater::FrameStartHook() { }

void MainLogicAdapater::FrameFinishHook() { }

int MainEntry(int argc, char* argv[])
{
    TickDispatcher oTickDispatcher;

    RaitaGui oRaitaGui;
    oRaitaGui.Initialize();

    auto pMainLogicAdapter = CreateMainLogicAdapter();
    pMainLogicAdapter->BackendSetup();

    oRaitaGui.BeforeMainLoop();

    bool bDone = false;
    auto tLast = std::chrono::high_resolution_clock::now();
    while (!bDone)
    {
        const auto tStart = std::chrono::high_resolution_clock::now();
        const auto tDelta = tStart - tLast;
        tLast = tStart;
        if (pMainLogicAdapter->ShouldStopProgram())
        {
            bDone = true;
        }
        oTickDispatcher.Tick(tDelta);
        pMainLogicAdapter->FrameStartHook();
        pMainLogicAdapter->BackendFrameStart();
        if (!oRaitaGui.FrameTick(tDelta))
        {
            bDone = true;
        }
        pMainLogicAdapter->BackendRender();
        pMainLogicAdapter->FrameFinishHook();
    }

    // Cleanup
    pMainLogicAdapter->BackendCleanUp();
    oRaitaGui.Uninitialize();

    return 0;
}
