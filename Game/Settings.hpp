#pragma once

#include <raylib.h>
#include <iostream>

class Settings
{
public:
	void S_SetupWindow();
	void S_CloseWindow();
	void S_ESCMenu();
	void FullScreenToggle();
private:
	void Loc_RelativeSizeModifier();
private:
	int m_WindowedWidth;
	int m_WindowedHeight;
};