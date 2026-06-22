#pragma once

#include "Settings.hpp"
#include "Timer.hpp"

#include <vector>
#include <string>
#include <array>
#include <math.h>

#define MAIN_MENU 0
#define RUN 1
#define PAUSE -1

#define VALID 1
#define INVALID -1

#define FLOOR 0
#define ROOF 1

#define EAST 0
#define SOUTH 1
#define WEST 2
#define NORTH 3


struct Wall
{
	Vector3 pos;
	Vector3 size;
	char State = INVALID;
};

struct Block
{
	std::array<Wall, 4> Walls;
	bool Visited = false;
};

struct Maze
{
	std::vector<Block> Map;
	int MapSizeX;
	int MapSizeZ;
	float BlockSize;
};

class Game
{
public:
	void Setup();
	void Close();

	void SetCamera();
	void SetMaze();
	void FillMaze();
	void CarveMaze();
	void CarvePassage(int BlockIDFrom, int BlockIDTo);


	void GameLoop();
	void UpdateMouse();
	void UpdateCamera();
	void CorrectForCollision2D();
	void RenderScene();

	void DrawBlock(int i);
	void DrawWorld();

private:
	Settings m_Window;

	Maze m_MainMaze;
	const int m_BlockSize = 20;//default value
	const float m_WallThickness = 1.0f;
	int m_GameState;

	Camera3D m_Camera;
	int m_MouseX;
	int m_MouseY;
	const float m_Sensetivity = 4.0f;//higher value = lower sense
	const float m_RunSpeedMultiplier = 2.0f;
	const float m_Speed = 0.20f;
	const float m_FOV = 90.0f;
	const float m_ZoomFOV = 30.0f;
	const float m_PlayerSize = 1.0f;
	const float m_StartX = 1;
	const float m_StartZ = 1;
	float m_SmoothZoom = PAUSE;//just a value for init (default state while not zoomed in)
	const float m_ZoomInSpeed = 0.1;
	const float m_ZoomOutSpeed = 1.1;
};