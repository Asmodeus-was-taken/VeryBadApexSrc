#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "include.hpp"
#include <d3d11.h>
#include <random>
#include <dwmapi.h>
#include "glob.h"
#include "cheatmain.h"
#include "configs.hpp"

bool bMenu = false;
int tab = 1;
bool b_Shutdown = false;
HWND h_Game;
bool is_menu = false;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace draw
{
    HWND h_hWnd;
    WNDCLASSEX wc;

    ID3D11Device* g_pd3dDevice = NULL;
    ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
    IDXGISwapChain* g_pSwapChain = NULL;
    ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

    static const char* options[]{ "Head", "Chest", "Leg" };
    int selected;

    void UpdateWND()
    {
        while (true)
        {
            if (GetAsyncKeyState(VK_UP) != 0)
                is_menu = !is_menu;

            long style = GetWindowLongPtr(h_hWnd, GWL_EXSTYLE);


            if (is_menu)
            {
                style &= ~WS_EX_LAYERED;
                SetWindowLongPtr(h_hWnd, GWL_EXSTYLE, style);
            }
            else
            {
                style |= WS_EX_LAYERED;
                SetWindowLongPtr(h_hWnd, GWL_EXSTYLE, style);
            }
            Sleep(100);
        }
    }

  

    auto CleanupRenderTarget() -> void
    {
        if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
    }

    auto CleanupDeviceD3D() -> void
    {
        CleanupRenderTarget();
        if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    }

    void DrawCorneredBox(int X, int Y, int W, int H, const ImU32& color, int thickness)
    {
        float lineW = (W / 3);
        float lineH = (H / 3);

        ////black outlines

        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);

        //corners
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
        ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
    }
    void CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

    bool CreateDeviceD3D()
    {
        /* setup swap chain */
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 300;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = h_hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
            return false;

        CreateRenderTarget();
        return true;
    }

    void Shutdown()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        CleanupDeviceD3D();
        DestroyWindow(h_hWnd);
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        std::exit(0);
    }

    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg)
        {
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application Render
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        default: break;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    /**
     * initializes the renderer
     */


    int FrameRate()
    {
        static int iFps, iLastFps;
        static float flLastTickCount, flTickCount;
        flTickCount = clock() * 0.001f;
        iFps++;
        if ((flTickCount - flLastTickCount) >= 1.0f) {
            flLastTickCount = flTickCount;
            iLastFps = iFps;
            iFps = 0;
        }
        return iLastFps;
    }
    bool Initialize()
    {
        WINDOWPLACEMENT g_wpPrev;
        DWORD dwStyle = GetWindowLong(h_Game, GWL_STYLE);
        MONITORINFO mi = { sizeof(mi) };
        if (GetWindowPlacement(h_Game, &g_wpPrev) &&
            GetMonitorInfo(MonitorFromWindow(h_Game,
                MONITOR_DEFAULTTOPRIMARY), &mi)) {
            SetWindowLong(h_Game, GWL_STYLE,
                dwStyle & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(h_Game, HWND_TOP,
                mi.rcMonitor.left, mi.rcMonitor.top,
                mi.rcMonitor.right - mi.rcMonitor.left,
                mi.rcMonitor.bottom - mi.rcMonitor.top,
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }

        // Create application window
        wc = { sizeof(WNDCLASSEX), ACS_TRANSPARENT, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, ("Windows Task Assistant"), NULL };
        RegisterClassEx(&wc);
        h_hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_LAYERED, wc.lpszClassName, (""), WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, wc.hInstance, NULL);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UpdateWND, 0, 0, 0);
        MARGINS margins = { -1 };
        DwmExtendFrameIntoClientArea(h_hWnd, &margins);

        SetLayeredWindowAttributes(h_hWnd, 0, 1.0f, LWA_ALPHA);
        SetLayeredWindowAttributes(h_hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);

        // Initialize Direct3D
        if (!CreateDeviceD3D())
        {
            CleanupDeviceD3D();
            UnregisterClass(wc.lpszClassName, wc.hInstance);
            return 1;
        }

        ShowWindow(h_hWnd, SW_SHOWDEFAULT);
        UpdateWindow(h_hWnd);

        SetForegroundWindow(h_hWnd);

        //IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui_ImplWin32_Init(h_hWnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

        MSG msg;
        ZeroMemory(&msg, sizeof(msg));

        while (msg.message != WM_QUIT)
        {
            if (b_Shutdown)
                break;

            if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                continue;
            }

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            bool linesa;
            if (GetAsyncKeyState(VK_INSERT) & 1) bMenu = !bMenu;
            if (bMenu)
            {


                //ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.f - menuSize.x / 2.f, io.DisplaySize.y / 2.f - menuSize.y / 2.f), ImGuiCond_Once);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 13.f);
                ImGui::SetWindowSize(ImVec2(380.0f, 700.0f));
                ImGui::SetNextWindowSize(ImVec2(1000.0f, 625.0f));
                ImGui::Begin((_("Gremlin Apex").c_str()), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
                {

                    ImGui::SetCursorPos(ImVec2(490, 1));
                    ImGui::TextColored(ImVec4(0, 153, 0, 255), "Gremlin Apex");

                    ImGui::BeginChild("##LeftSide", ImVec2(125, 575), true);

                    if (ImGui::Button("Aimbot", ImVec2(100, 50)))
                    {
                        tab = 1;
                    }
                    ImGui::Spacing();
                    if (ImGui::Button("Visuals", ImVec2(100, 50)))
                    {
                        tab = 2;
                    }
                    ImGui::Spacing();
                    if (ImGui::Button("Loot Visuals", ImVec2(100, 50)))
                    {
                        tab = 3;
                    }
                    ImGui::Spacing();
                    if (ImGui::Button("Misc", ImVec2(100, 50)))
                    {
                        tab = 4;
                    }
                    ImGui::EndChild();
                    ImGui::SameLine();


                    if (tab == 1)
                    {
                        ImGui::SameLine();
                        ImGui::BeginChild("##MainTaaaaab", ImVec2(950, 575));

                        ImGui::TextColored(ImVec4(0, 1, 0, 1), "->"); ImGui::SameLine(); ImGui::Text("Aimbot"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 0, 1), "<-"); ImGui::SameLine(); ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Enable").c_str(), &options::aimbot);
                        ImGui::Spacing();
                        ImGui::Combo(_("Key Bind").c_str(), &options::keyBind, options::keyBinds, IM_ARRAYSIZE(options::keyBinds));
                        ImGui::Spacing();
                        if (options::keyBind == 0)
                            options::curKey = 0x01;
                        else if (options::keyBind == 1)
                            options::curKey = 0x02;
                        else if (options::keyBind == 2)
                            options::curKey = 7;
                        else
                            options::curKey = 0x12;
                        ImGui::SliderFloat(_("Fov").c_str(), &options::aimbotFov, 0, 180);
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Draw Circle").c_str(), &options::drawCircle);
                        ImGui::Spacing();
                        ImGui::Combo(_("Hit Box").c_str(), &options::hitbox, options::hitBoxes, IM_ARRAYSIZE(options::hitBoxes));
                        ImGui::Spacing();
                        ImGui::SliderFloat(_("Smoothing").c_str(), &options::aimbotSmoothing, 1, 20);
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Visability Check").c_str(), &options::visCheck);
                        ImGui::Spacing();
                        ImGui::SliderFloat(_("Max Distance").c_str(), &options::maxDist, 1.f, 1000.f);
                        ImGui::Spacing();
                        ImGui::Checkbox(_("No Spread").c_str(), &options::instantBullet);
                        ImGui::Spacing();
                        ImGui::Text(_("Aimbot Recoil Control").c_str());
                        ImGui::Spacing();
                        ImGui::SliderFloat(_("Pitch").c_str(), &options::aimPitchStr, 0, 1);
                        ImGui::SliderFloat(_("Yaw").c_str(), &options::aimYawStr, 0, 1);

                        ImGui::EndChild();
                    }

                    if (tab == 2)
                    {
                        ImGui::BeginChild("##MainTab", ImVec2(950, 575));

                        ImGui::TextColored(ImVec4(0, 1, 0, 1), "->"); ImGui::SameLine(); ImGui::Text("Visuals"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 0, 1), "<-"); ImGui::SameLine(); ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Box").c_str(), &options::box);
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Corner Box").c_str(), &options::cornerBox);
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Distance ").c_str(), &options::drawDist);
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Snaplines").c_str(), &options::linesToPlayer);
                        ImGui::Spacing();
                        ImGui::SliderFloat(_("Max Distance").c_str(), &options::espMaxDist, 0.f, 1000.f);
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Player Info").c_str(), &options::playerinfo);
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Glow").c_str(), &options::glow);
                        ImGui::Spacing();
                        ImGui::Combo(_("Glow Type").c_str(), &options::glowType, options::glowTypes, IM_ARRAYSIZE(options::glowTypes));
                        ImGui::Spacing();
                        ImGui::ColorPicker4(_("Esp Color").c_str(), options::gcol);
                        ImGui::Spacing();
                    }



                    if (tab == 3)
                    {
                        ImGui::BeginChild("##MainTab", ImVec2(950, 575));

                        ImGui::TextColored(ImVec4(0, 1, 0, 1), "->"); ImGui::SameLine(); ImGui::Text("Loot Visuals"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 0, 1), "<-"); ImGui::SameLine(); ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Item Glow").c_str(), &options::itemgl);

                        ImGui::EndChild();

                    }


                    if (tab == 4)
                    {
                        ImGui::BeginChild("##MainTab", ImVec2(950, 575));

                        ImGui::TextColored(ImVec4(0, 1, 0, 1), "->"); ImGui::SameLine(); ImGui::Text("Misc"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 0, 1), "<-"); ImGui::SameLine(); ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Recoil Control").c_str(), &options::noRecoil);
                        ImGui::Spacing();
                        ImGui::SliderFloat(_("Yaw Amount").c_str(), &options::pitchStr, 0.f, 1.f);
                        ImGui::Spacing();
                        ImGui::SliderFloat(_("Pitch Amount").c_str(), &options::yawStr, 0.0f, 1.f);
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Third Person").c_str(), &options::thirdperson);
                        ImGui::Spacing();
                        ImGui::Checkbox(_("Skin Changer").c_str(), &options::changer);
                        ImGui::Spacing();
                        ImGui::Text("Skin ID");
                        ImGui::Spacing();
                        ImGui::SliderInt("ID", &options::ski, 1, 25);
                        ImGui::Spacing();
                        ImGui::Spacing();
                        ImGui::Text("CONFIGS");
                        ImGui::Spacing();
                        if (ImGui::Button("Rage"))
                            options::aimbotSmoothing = 3;
                        ImGui::Spacing();
                        if (ImGui::Button("Semi legit"))
                            options::aimbotSmoothing = 6;
                        ImGui::Spacing();
                        if (ImGui::Button("Legit"))
                            options::aimbotSmoothing = 12;
                        ImGui::Spacing();
                        if (ImGui::Button("Save Config"))
                            configs::saveConfig();
                        ImGui::Spacing();
                        if (ImGui::Button("Load Config"))
                            configs::loadConfig();
                        ImGui::EndChild();
                    }
                }
            }
            noRecoil();
            espone();
            int width = 100;
            float rainbowSpeed = 0.01;
            static float staticHue = 0;
            staticHue -= rainbowSpeed;
            ImColor cRainbow;
            if (staticHue < -1.f) staticHue += 1.f;
            for (int i = 0; i < width; i++)
            {
                float hue = staticHue + (1.f / (float)width) * i;
                if (hue < 0.f) hue += 1.f;
                cRainbow = ImColor::HSV(hue, 1.f, 1.f);
            }


            ImDrawList* draw_list = ImGui::GetOverlayDrawList();

            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            ImGui::EndFrame();

            g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);

            ImVec4 clear_color = ImVec4(0, 0, 0, 0);
            g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);

            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            g_pSwapChain->Present(0, 0);
        }
        Shutdown();
        return 0;
    }
}