#include "Game.hpp"


void Game::Setup()
{
	m_Window.S_SetupWindow();

    DisableCursor();    // Limit cursor to relative movement inside the window

    SetMaze();
    SetCamera();
    m_GameState = MAIN_MENU;
}
void Game::Close()
{
    m_Window.S_CloseWindow();
}

void Game::UpdateMouse()
{
    m_MouseX = GetMouseX();
    m_MouseY = GetMouseY();
}

void Game::SetCamera()
{
    m_Camera = { 0 };
    m_Camera.position = { m_MainMaze.BlockSize * m_StartX, 0.0f, m_MainMaze.BlockSize * m_StartZ};
    m_Camera.target = { m_MainMaze.BlockSize * m_StartX, 0.0f, m_MainMaze.BlockSize * m_StartZ * 2};    // Camera looking North
    m_Camera.up = { 0.0f, 1.0f, 0.0f };                  
    m_Camera.fovy = 90.0f;                                
    m_Camera.projection = CAMERA_PERSPECTIVE;

    UpdateMouse();
}
void Game::SetMaze()
{
    m_MainMaze.BlockSize = 20;
    m_MainMaze.MapSizeX = 10 + 1;
    m_MainMaze.MapSizeZ = 10 + 1;//+ 1 is South and East Walls

    FillMaze();
    CarveMaze();
}

void Game::CarvePassage(int BlockIDFrom, int BlockIDTo)
{
    if (BlockIDFrom - BlockIDTo == 1)//x -> x-1
    {
        m_MainMaze.Map[BlockIDTo].Walls[WEST].State = INVALID;
    }
    else if (BlockIDFrom - BlockIDTo == -1)//x -> x+1
    {
        m_MainMaze.Map[BlockIDFrom].Walls[WEST].State = INVALID;
    }
    else if (BlockIDFrom - BlockIDTo == m_MainMaze.MapSizeX)//z -> z-1
    {
        m_MainMaze.Map[BlockIDTo].Walls[NORTH].State = INVALID;
    }
    else if (BlockIDFrom - BlockIDTo == -m_MainMaze.MapSizeX)//z -> z+1
    {
        m_MainMaze.Map[BlockIDFrom].Walls[NORTH].State = INVALID;
    }
}

void Game::CarveMaze()
{
    int mapSize = m_MainMaze.MapSizeX * m_MainMaze.MapSizeZ;
    
    int currentX = m_StartX;
    int currentZ = m_StartZ;

    std::vector<int> pathByIDs;

    m_MainMaze.Map[m_StartZ * m_MainMaze.MapSizeX + m_StartX].Visited = true;

    int blockIDFrom;
    int blockIDTo;

    int DirectionsID[4] = {INVALID, INVALID, INVALID, INVALID};
    char possibleDirections = 0;

    blockIDFrom = currentZ * m_MainMaze.MapSizeX + currentX;
    pathByIDs.push_back(blockIDFrom);

    int branchLength = 0;//for extra stuff in the future

    do
    {
        possibleDirections = 0;

        if (currentX - 1 != 0)
        { 
            blockIDTo = blockIDFrom - 1;
            if (m_MainMaze.Map[blockIDTo].Visited == false)
            {
                DirectionsID[possibleDirections] = blockIDTo;
                possibleDirections++;
            }
        }
        if (currentZ - 1 != 0)
        {
            blockIDTo = blockIDFrom - m_MainMaze.MapSizeX;
            if (m_MainMaze.Map[blockIDTo].Visited == false)
            {
                DirectionsID[possibleDirections] = blockIDTo;
                possibleDirections++;
            }
        }
        if (currentX + 1 != m_MainMaze.MapSizeX)
        {
            blockIDTo = blockIDFrom + 1;
            if (m_MainMaze.Map[blockIDTo].Visited == false)
            {
                DirectionsID[possibleDirections] = blockIDTo;
                possibleDirections++;
            }
        }
        if (currentZ + 1 != m_MainMaze.MapSizeZ)
        {
            blockIDTo = blockIDFrom + m_MainMaze.MapSizeX;
            if (m_MainMaze.Map[blockIDTo].Visited == false)
            {
                DirectionsID[possibleDirections] = blockIDTo;
                possibleDirections++;
            }
        }

        
        if (possibleDirections > 0)
        {
            if (possibleDirections > 1)
            {
                blockIDTo = DirectionsID[GetRandomValue(0, possibleDirections - 1)];
            }
            else
            {
                blockIDTo = DirectionsID[0];
            }

            CarvePassage(blockIDFrom, blockIDTo);
            m_MainMaze.Map[blockIDTo].Visited = true;
            pathByIDs.push_back(blockIDTo);

            currentX = blockIDTo % m_MainMaze.MapSizeX;
            currentZ = blockIDTo / m_MainMaze.MapSizeX;

            blockIDFrom = blockIDTo;

            branchLength++;
        }
        else
        {
            blockIDFrom = pathByIDs[pathByIDs.size()-2];
            pathByIDs.pop_back();
        
            currentX = blockIDFrom % m_MainMaze.MapSizeX;
            currentZ = blockIDFrom / m_MainMaze.MapSizeX;
        }

    } while (!(currentX == m_StartX && currentZ == m_StartZ));//traced back to spawn
}

void Game::FillMaze()
{
    float offCenter = m_MainMaze.BlockSize / 2;

    int mapSize = m_MainMaze.MapSizeX * m_MainMaze.MapSizeZ;
    for (int blockID = 0; blockID < mapSize; blockID++)
    {
        int columnX = blockID % m_MainMaze.MapSizeX;
        int columnZ = blockID / m_MainMaze.MapSizeX;

        Vector3 blockPos;
        blockPos.x = columnX * m_MainMaze.BlockSize;
        blockPos.y = 0;
        blockPos.z = columnZ * m_MainMaze.BlockSize;

        Block block;//West, North, Floor, Roof
        block.Visited = false;


        //x -> west
        //z -> north
        if (columnZ == 0)
        {
            Wall WallNorth;
            WallNorth.State = VALID;
            WallNorth.pos.x = blockPos.x;
            WallNorth.pos.y = blockPos.y;
            WallNorth.pos.z = blockPos.z + offCenter;

            WallNorth.size.x = m_MainMaze.BlockSize;
            WallNorth.size.y = m_MainMaze.BlockSize;
            WallNorth.size.z = m_WallThickness;

            block.Walls[NORTH] = WallNorth;
        }
        else if(columnX == 0)
        {
            Wall WallWest;
            WallWest.State = VALID;
            WallWest.pos.x = blockPos.x + offCenter;
            WallWest.pos.y = blockPos.y;
            WallWest.pos.z = blockPos.z;

            WallWest.size.x = m_WallThickness;
            WallWest.size.y = m_MainMaze.BlockSize;
            WallWest.size.z = m_MainMaze.BlockSize;

            block.Walls[WEST] = WallWest;
        }
        else
        {
            Wall WallNorth;
            WallNorth.State = VALID;
            WallNorth.pos.x = blockPos.x;
            WallNorth.pos.y = blockPos.y;
            WallNorth.pos.z = blockPos.z + offCenter;

            WallNorth.size.x = m_MainMaze.BlockSize;
            WallNorth.size.y = m_MainMaze.BlockSize;
            WallNorth.size.z = m_WallThickness;

            block.Walls[NORTH] = WallNorth;

            Wall WallWest;
            WallWest.State = VALID;
            WallWest.pos.x = blockPos.x + offCenter;
            WallWest.pos.y = blockPos.y;
            WallWest.pos.z = blockPos.z;

            WallWest.size.x = m_WallThickness;
            WallWest.size.y = m_MainMaze.BlockSize;
            WallWest.size.z = m_MainMaze.BlockSize;

            block.Walls[WEST] = WallWest;
        }

        m_MainMaze.Map.push_back(block);
    }
}

void Game::CorrectForCollision2D()
{
    if (m_Camera.position.x > m_MainMaze.MapSizeX * m_MainMaze.BlockSize - m_MainMaze.BlockSize / 2 ||
        m_Camera.position.z > m_MainMaze.MapSizeZ * m_MainMaze.BlockSize - m_MainMaze.BlockSize / 2)
    {
        return;
    }
    int blockID = int(m_Camera.position.x + m_MainMaze.BlockSize/2) / int(m_MainMaze.BlockSize) + int(m_Camera.position.z + m_MainMaze.BlockSize/2  ) / int(m_MainMaze.BlockSize) * m_MainMaze.MapSizeX;

    int neighbourBlocks[9] = { blockID, blockID - 1, blockID + 1,
                               blockID - m_MainMaze.MapSizeX, blockID - m_MainMaze.MapSizeX - 1,  blockID - m_MainMaze.MapSizeX + 1, 
                               blockID + m_MainMaze.MapSizeX, blockID + m_MainMaze.MapSizeX - 1,  blockID + m_MainMaze.MapSizeX + 1, };


    Rectangle Player2D;
    Player2D.x = m_Camera.position.x - m_PlayerSize / 2;
    Player2D.y = m_Camera.position.z - m_PlayerSize / 2;
    Player2D.width = m_PlayerSize;
    Player2D.height = m_PlayerSize;

    int mapSize = m_MainMaze.MapSizeX * m_MainMaze.MapSizeZ - 1;


    for (int num = 0; num < 9; num++)
    {
        if (neighbourBlocks[num] > mapSize || neighbourBlocks[num] < 0)//will check up to 6 unrelated blocks sometimes but it shouldn't be a big deal
            continue;

        for (int side = 2; side < 4; side++)//only walls
        {
            if (m_MainMaze.Map[neighbourBlocks[num]].Walls[side].State == VALID)
            {
                Rectangle Wall2D;
                Wall2D.x = m_MainMaze.Map[neighbourBlocks[num]].Walls[side].pos.x - m_MainMaze.Map[neighbourBlocks[num]].Walls[side].size.x / 2;
                Wall2D.y = m_MainMaze.Map[neighbourBlocks[num]].Walls[side].pos.z - m_MainMaze.Map[neighbourBlocks[num]].Walls[side].size.z / 2;
                Wall2D.width = m_MainMaze.Map[neighbourBlocks[num]].Walls[side].size.x;
                Wall2D.height = m_MainMaze.Map[neighbourBlocks[num]].Walls[side].size.z;

                Rectangle Collision = GetCollisionRec(Wall2D, Player2D);

                if (Collision.height)
                {
                    if (Collision.height > Collision.width)
                    {
                        if (Player2D.x > Wall2D.x)
                        {
                            m_Camera.position.x += Collision.width;
                            m_Camera.target.x += Collision.width;
                        }
                        else
                        {
                            m_Camera.position.x -= Collision.width;
                            m_Camera.target.x -= Collision.width;
                        }
                    }
                    else
                    {
                        if (Player2D.y > Wall2D.y)
                        {
                            m_Camera.position.z += Collision.height;
                            m_Camera.target.z += Collision.height;
                        }
                        else
                        {
                            m_Camera.position.z -= Collision.height;
                            m_Camera.target.z -= Collision.height;
                        }
                    }
                }
            }
        }
    }
}

void Game::UpdateCamera()
{
    //std::cout << "\n" << m_Camera.fovy << "  |  " << m_Camera.position.x << " " << m_Camera.position.z; //debug info
    //std::cout << "  |  " << m_Camera.target.x << " " << m_Camera.target.y << " " << m_Camera.target.z;

    float x = 0;
    float z = 0;
    float y = 0;

    float speed = m_Speed;

    if (IsKeyDown(KEY_W))
    {
        x++;
    }
    else if (IsKeyDown(KEY_S))
    {
        x--;
    }
    if (IsKeyDown(KEY_D))
    {
        z++;
    }
    else if (IsKeyDown(KEY_A))
    {
        z--;
    }

    if (IsKeyDown(KEY_SPACE))
    {
        y++;
    }
    else if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        y--;
    }

    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        speed *= m_RunSpeedMultiplier;
    }

    ////Smooth zoom in effect logic////
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        m_SmoothZoom += m_ZoomInSpeed;//zoom in speed
        m_Camera.fovy = ((m_FOV - m_ZoomFOV) / m_SmoothZoom) + m_ZoomFOV;//some form of limit
    }
    else
    {
        m_SmoothZoom = 1;//reset
        if (m_FOV - m_Camera.fovy > 1)
        {
            m_Camera.fovy *= m_ZoomOutSpeed;
        }
        else
        { 
            m_Camera.fovy = m_FOV; 
        }
    }
    /////////////////////////////////
    
    float localSens = m_Sensetivity / (m_Camera.fovy/ m_FOV);

    UpdateCameraPro(&m_Camera,
        { x * speed, z * speed, y * speed },                                                      //movement
        { (GetMouseX() - m_MouseX) / localSens, (GetMouseY() - m_MouseY) / localSens, 0}, //rotation
        0);

    CorrectForCollision2D();

    UpdateMouse();
}

void Game::GameLoop()
{
    m_GameState = RUN;

    
    int cameraMode = CAMERA_FREE;//DEBUG

    while (!IsKeyPressed(KEY_ENTER))    //DEBUG
    {
        if (IsKeyPressed(KEY_ESCAPE)) 
        { 
            m_GameState *= PAUSE; 
            if (m_GameState == RUN)
            {
                DisableCursor();

                UpdateMouse();
            }
        };//negative is pause

        if (IsKeyPressed(KEY_F)) 
        { 
            m_Window.FullScreenToggle();
        }

        
        if (m_GameState == RUN)
        {
            UpdateCamera();


            //UpdateCamera(&m_Camera, cameraMode);                  // Move camera DEBUG
            //UpdateCamera(&m_Camera, cameraMode);                  // Move camera (2x speed) 

            // Update
            //----------------------------------------------------------------------------------
            // TODO: Update your variables here
            //----------------------------------------------------------------------------------
        }
        RenderScene();

    }
}

void Game::RenderScene()
{
    BeginDrawing();//Start

    ClearBackground(RAYWHITE);

    BeginMode3D(m_Camera);
    DrawWorld();
    EndMode3D();

    DrawText(std::to_string(GetFPS()).c_str(), 5, 5, 20, BLACK);//fps counter (unopt)



    if (m_GameState == PAUSE)
    {
        m_Window.S_ESCMenu();
    }

    EndDrawing();//End
}

void Game::DrawWorld()
{
    Timer time;
    for (int i = 0; i < m_MainMaze.Map.size(); i++)
    {
        DrawBlock(i);
    }
}

void Game::DrawBlock(int i)
{
    for (int side = 0; side < 4; side++)
    {
        if (m_MainMaze.Map[i].Walls[side].State == VALID)
        {
            Color testColor;
            testColor.r = char((m_MainMaze.Map[i].Walls[side].pos.x + m_MainMaze.BlockSize / 2) / (m_MainMaze.MapSizeX * m_MainMaze.BlockSize) * 255);
            testColor.g = char((m_MainMaze.Map[i].Walls[side].pos.z + m_MainMaze.BlockSize / 2) / (m_MainMaze.MapSizeZ * m_MainMaze.BlockSize) * 255);
            testColor.b = char(m_MainMaze.Map[i].Walls[side].pos.y);
            testColor.a = 255;

            DrawCube({ m_MainMaze.Map[i].Walls[side].pos.x, m_MainMaze.Map[i].Walls[side].pos.y, m_MainMaze.Map[i].Walls[side].pos.z }, 
                m_MainMaze.Map[i].Walls[side].size.x, m_MainMaze.Map[i].Walls[side].size.y, m_MainMaze.Map[i].Walls[side].size.z,
                testColor);      // Draw a wall

        }
    }
}



