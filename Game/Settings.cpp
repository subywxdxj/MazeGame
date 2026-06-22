#include "Settings.hpp"

void Settings::S_SetupWindow()
{
    SetTargetFPS(60);
    InitWindow(960, 540, "Game");//0, 0 = windowed full

    int Monitor = GetCurrentMonitor();

    int ScreenWidth = GetMonitorWidth(Monitor);
    int ScreenHeight = GetMonitorHeight(Monitor);
    SetWindowSize(ScreenWidth/2, ScreenHeight/2);

    int FPS = GetMonitorRefreshRate(Monitor);
    SetTargetFPS(FPS);

    //FullScreenToggle();//OFF FOR DEBUG 
}
void Settings::S_CloseWindow()
{
    CloseWindow();
}

void Settings::FullScreenToggle()
{
    int WindowWidth = GetScreenWidth();
    int WindowHeight = GetScreenHeight();

    int mon = GetCurrentMonitor();
    int ScreenWidth = GetMonitorWidth(mon);
    int ScreenHeight = GetMonitorHeight(mon);

    ToggleFullscreen();

    //IsFullscreenResolution()  
    if ((WindowWidth == ScreenWidth) &&
        (WindowHeight == ScreenHeight))
    {
        SetWindowSize(m_WindowedWidth, m_WindowedHeight);
    }   
    else
    {
        m_WindowedWidth = WindowWidth;
        m_WindowedHeight = WindowHeight;
        SetWindowSize(ScreenWidth, ScreenHeight);
    }

}

void Settings::Loc_RelativeSizeModifier()//modifies UI element sizes relative to resolution
{

}

//add centering and move allat to external functions for readablility
void Settings::S_ESCMenu()//overlayed over the game
{
    if (IsCursorHidden())
        EnableCursor();

    int ScreenWidth = GetScreenWidth();
    int ScreenHeight = GetScreenHeight();

    int Width = ScreenWidth * 0.2;//relative to window size
    int Height = ScreenHeight * 0.4;//relative to window size

    //Draw centered pause menu relative to window size
    //DrawRectangle(
    //    ScreenWidth / 2 - (Width / 2),//centered
    //    ScreenHeight / 2 - (Height / 2),//centered
    //    Width,
    //    Height,
    //    BLACK);

    int WidthButtonQ = ScreenWidth * 0.1;//relative to window size
    int HeightButtonQ = ScreenHeight * 0.08;//relative to window size

    //Quit button
    DrawRectangle(
        ScreenWidth / 2 - (WidthButtonQ / 2),//centered
        ScreenHeight / 2 - (HeightButtonQ / 2),//centered
        WidthButtonQ,
        HeightButtonQ,
        RED);

    WidthButtonQ = WidthButtonQ * 0.8;
    HeightButtonQ = HeightButtonQ * 0.7;
    //DrawText("PAUSE", ScreenWidth / 2 - (WidthButtonQ / 2), ScreenHeight / 2 - (HeightButtonQ / 2), WidthButtonQ, WHITE);
    DrawText("QUIT", ScreenWidth / 2 - (WidthButtonQ / 2), ScreenHeight / 2 - (HeightButtonQ / 2), WidthButtonQ * 0.4, WHITE);

}


