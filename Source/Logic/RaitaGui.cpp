#include "RaitaGui.h"

#include "imgui.h"

namespace GuiStatic
{
    bool g_bIsActivated = false;
    bool g_bEnableTimePeriodActivation = true;
    std::chrono::nanoseconds g_tEnabledDurationLast;
    std::chrono::nanoseconds g_tEnabledDurationTotal;
}

static ImVec4 oClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

class RaitaGuiImpl
{
    friend class RaitaGui;

    void Initialize()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& oIO = ImGui::GetIO(); (void)oIO;
        oIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        oIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();
    }

    void Uninitialize()
    {
        ImGui::DestroyContext();
    }

    void BeforeMainLoop()
    {
        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != nullptr);
    }

    bool FrameTick(const std::chrono::nanoseconds tDelta)
    {
        using namespace std::chrono;

        bool bRet = true;
        ImGui::NewFrame();
        ImGuiIO& oIO = ImGui::GetIO(); (void)oIO;

        do
        {
            const ImGuiViewport* pMainViewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(-1, -1), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(330, 130), ImGuiCond_Once);

            ImGui::Begin("Raita", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

            ImGui::Text("Take a break :-)"); 
            if (GuiStatic::g_bIsActivated)
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(255.0f, 0.0f, 0.0f, 255.0f), "%s", "    Activating...");
            }

            ImGui::Text("Relaxed: current %.2lfs, total %.2lfs", 
                duration_cast<milliseconds>(GuiStatic::g_tEnabledDurationLast).count() / 1000.0, 
                duration_cast<milliseconds>(GuiStatic::g_tEnabledDurationTotal).count() / 1000.0);
            
            ImGui::Checkbox("Auto Schedule", &GuiStatic::g_bEnableTimePeriodActivation);

            ImGui::SameLine();

            if (ImGui::Button("Quit"))
            {
                bRet = false;
            }

            ImGui::End();
        } while (false);

        // Rendering
        ImGui::Render();

        return bRet;
    }
};

RaitaGui::RaitaGui()
{
    pImpl = new RaitaGuiImpl;
}

RaitaGui::~RaitaGui()
{
    if (pImpl != nullptr)
    {
        delete pImpl;
        pImpl = nullptr;
    }
}

void RaitaGui::Initialize()
{
    pImpl->Initialize();
}

void RaitaGui::Uninitialize()
{
    pImpl->Uninitialize();
}

void RaitaGui::BeforeMainLoop()
{
    pImpl->BeforeMainLoop();
}

bool RaitaGui::FrameTick(const std::chrono::nanoseconds tDelta)
{
    return pImpl->FrameTick(tDelta);
}
