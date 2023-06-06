#include <maze.h>
#include <characters.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <random>
#include <path_finding.h>

int Monster::MonsterRow = 6;
int Monster::MonsterColumn = 1;
bool MazeGame::IsNotGameOver = true;

char MazeGame::maze[MAX_ROWS][MAX_COLS] = {
    {'+', '+', '+', '+', '+', '+', '+', '+', '+', '+'},
    {'+', '.', '.', '.', '.', '.', '.', '.', '.', '+'},
    {'+', '.', '+', '.', '+', '+', '+', '+', '.', '+'},
    {'+', '.', '.', '.', '.', '.', '.', '+', '.', '+'},
    {'+', '.', '.', '.', '+', '+', '.', '.', '.', '+'},
    {'+', '.', '+', '+', '+', '+', '.', '+', '.', '+'},
    {'+', '.', '+', '.', '+', '+', '.', '.', '.', '+'},
    {'+', '.', '.', '.', '.', '.', '.', '+', '.', '+'},
    {'E', '.', '+', '+', '+', '+', '+', '+', '.', '+'},
    {'+', '+', '+', '+', '+', '+', '+', '+', '+', '+'}};

void Monster::MovingMonster(Player &player)
{

    std::vector<Cell> BestPath = Dijkstra(MonsterRow, MonsterColumn, player.playerRow, player.playerCol);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(1, 2);
    int random = dist(rng);

    // Define the distribution for generating random numbers

    if (random == 1)
    {

        if (BestPath.size() >= 2)
        {
            Cell secondCell = BestPath[1];
            MonsterRow = secondCell.row;
            MonsterColumn = secondCell.col;
        }
    }
    else
    {
        bool flag = true;
        while (flag)
        {

            std::uniform_int_distribution<int> dist(0, 3);

            random = dist(rng);

            switch (random)
            {
            case 0:
                // north
                if (MonsterRow - 1 >= 0 && MazeGame::maze[MonsterRow - 1][MonsterColumn] != MazeGame::WALL)
                {

                    --MonsterRow;
                    flag = false;
                }
                break;
            case 1:
                if (MonsterRow + 1 >= 0 && MazeGame::maze[MonsterRow + 1][MonsterColumn] != MazeGame::WALL)
                {
                    ++MonsterRow;
                    flag = false;
                }
                break;
            case 2:
                if (MonsterColumn - 1 < MazeGame::MAX_COLS && MazeGame::maze[MonsterRow][MonsterColumn - 1] != MazeGame::WALL)
                {
                    --MonsterColumn;
                    flag = false;
                }
                break;
            case 3:

                if (MonsterColumn + 1 < MazeGame::MAX_COLS && MazeGame::maze[MonsterRow][MonsterColumn + 1] != MazeGame::WALL)
                {
                    ++MonsterColumn;
                    flag = false;
                }
                break;

            default:

                return;
            }
        }
    }
}

bool MazeGame::MoveStatus(Player &player, char c)
{
    int targetRow = player.playerRow;
    int targetCol = player.playerCol;

    if (c == 'w')
        --targetRow;

    else if (c == 's')
        ++targetRow;

    else if (c == 'a')
        --targetCol;

    else if (c == 'd')
        ++targetCol;

    if (targetRow < 0 || targetRow >= MAX_ROWS || targetCol < 0 || targetCol >= MAX_COLS)
    {
        return false; // Target position is outside the maze boundaries
    }

    if (maze[targetRow][targetCol] == MazeGame::WALL || (targetRow == Monster::MonsterRow && targetCol == Monster::MonsterColumn))
    {
        return false; // Target position is a wall or occupied by a monster
    }

    // The move is valid, update the player's position
    player.playerRow = targetRow;
    player.playerCol = targetCol;

    return true;
}

void MazeGame::MovePlayer(Player &player, char c)
{
    c = (char)tolower(c);

    int speed = player.speed;

    while (speed >= MIN_SPEED)
    {
        if (MoveStatus(player, c))
            --speed;

        break;
    }
}

void MazeGame::DisplayFrameConsole(Player &player)
{

    GameStatus(player);
    for (int Row = 0; Row < MAX_ROWS; Row++)
    {

        for (int Column = 0; Column < MAX_COLS; Column++)
        {

            if (!DisplayCharacters(player, Row, Column))
            {

                std::cout << maze[Row][Column];
            }
        }
        std::cout << '\n';
    }
    GameStatus(player);
};

bool MazeGame::DisplayCharacters(Player &player, int Row, int Column)
{

    if (!(Row == player.playerRow && Column == player.playerCol) && !(Row == Monster::MonsterRow && Column == Monster::MonsterColumn))
    {

        return false;
    }

    if (Row == Monster::MonsterRow && Column == Monster::MonsterColumn)
        std::cout << 'M';

    else if (Row == player.playerRow && Column == player.playerCol)
        std::cout << 'P';

    return true;
}

void MazeGame::GameStatus(Player &player)
{
    if (player.playerRow == Monster::MonsterRow && player.playerCol == Monster::MonsterColumn)
    {
        std::cout << "Game Over!\n";
        IsNotGameOver = false;
    }
    if (maze[player.playerRow][player.playerCol] == 'E')
    {
        std::cout << "You Found the Exit! Congratulations!\n";
        IsNotGameOver = false;
    }
}

char MazeGame::ProcessPlayerInput(void)
{

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

    INPUT_RECORD inputRecord;
    DWORD numEventsRead;

    while (true)
    {
        ReadConsoleInput(hInput, &inputRecord, 1, &numEventsRead);

        if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
        {
            WORD keyCode = inputRecord.Event.KeyEvent.wVirtualKeyCode;

            switch (keyCode)
            {
            case VK_UP:
            case 0x57: // 'W' key
                return 'w';
                break;
            case VK_DOWN:
            case 0x53: // 'S' key
                return 's';
                break;
            case VK_LEFT:
            case 0x41: // 'A' key
                return 'a';
                break;
            case VK_RIGHT:
            case 0x44: // 'D' key
                return 'd';
                break;
            case VK_ESCAPE:
                return VK_ESCAPE;
                break;
            }
        }
    }
}

char MazeGame::UserInput(void)
{
    if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('W') & 0x8000)
    {
        return 'w';
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('S') & 0x8000)
    {
        return 's';
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000)
    {
        return 'd';
    }
    else if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000)
    {
        return 'a';
    }

    return 0;
}

void Monster::Move(Player &player)
{
    int tmpRow = MonsterRow;
    int tmpColumn = MonsterColumn;

    auto IsPlayerFound = [&]()
    {
        if (tmpRow == player.playerRow && tmpColumn == player.playerCol)
            return true;

        return false;
    };

    int MonsterVisibility = 4;

    for (int i = 0; i < MonsterVisibility; ++i)
    {
        if (tmpRow - 1 >= 0 && MazeGame::maze[tmpRow - 1][tmpColumn] != MazeGame::WALL)
        {
            if (IsPlayerFound())
            {
                --MonsterRow;
                break;
            }
        }

        --tmpRow;

                // if (tmpColumn - 1 < MazeGame::MAX_COLS && MazeGame::maze[tmpRow][tmpColumn - 1] != MazeGame::WALL)
        // {
        // }
        // if (MtmpColumn + 1 < MazeGame::MAX_COLS && MazeGame::maze[tmpRow][tmpColumn + 1] != MazeGame::WALL)
        // {
        // }
    }
}
